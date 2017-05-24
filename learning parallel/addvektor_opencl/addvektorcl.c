
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __APPLE__
    #include "OpenCL/opencl.h"
#else
    #include "CL/cl.h"
#endif

#include "addvektorcl.h"

/**
 * Read the content of the file given by the filename.
 *
 * @param filename the name of the file which should be read.
 *
 * @return the contents or NULL on an error. The returned
 *  value must be freed with free() when no longer in use.
 */
static char *read_file_contents(const char *filename)
{
	long size;
	char *ret = NULL;
	char *contents = NULL;
	FILE *fh;

	if (!(fh = fopen(filename,"r")))
		return NULL;

	fseek(fh,0,SEEK_END);
	size = ftell(fh);
	if (size < 1)
		goto out;
	fseek(fh,0,SEEK_SET);

	if (!(contents = malloc(size+1)))
		goto out;
	if ((fread(contents, 1, size, fh) != size))
		goto out;

	contents[size] = 0;

	ret = contents;
	contents = NULL;
out:
	fclose(fh);
	free(contents);
	return ret;
}

/**
 * Our main program. What we are doing here is following.
 *
 * 1) Initialize OpenCL (find platform and device)
 * 2) Create and initialize the context
 *   a) Create the context
 *   b) Create a command queue and attach it to the context
 *   c) Load the kernel and compile it for the context
 *   d) Create the device buffers on which the kernel operates
 *   e) Plug the device buffers to the proper argument positions
 *      of the kernel
 * 3) Prepare and run the calculation
 *   a) Insert a command to copy the data from host buffer to
 *      above-created device buffer
 *   b) Insert a command to invoke the kernel computation. This
 *      operates on the device buffer we plugged into the kernel
 *      and produces something in the second device buffers also
 *      created above.
 *   c) Insert a command to copy the contents of the second device
 *      buffer into a second host buffer
 *   d) Wait for all the action to be completed
 * 4) Print out the contents of the second host buffer
 * 5) Cleanup
 */
int vadd(float *a, float *b, float *dest, int len)
{
	/* Defines the number of work items*/
	size_t worksteps = len;
	size_t memworksize = len * sizeof(float);

	/* The error variable. We need it everywhere */
	cl_int error;

	/* Phase 1 variables */
	cl_platform_id platform;
	cl_device_id device;
	cl_uint platforms, devices;
	char name[128];

	/* Phase 2 variables */
	char *k_src = NULL; /* kernel source */
	cl_context context = NULL;

	/* Exit value, we assume that we fail */
	int rc = EXIT_FAILURE;

	/**** Phase 1: Find platform and device (will OpenCL work at all?) *****/

	/* Fetch available platform; we only want the first one. */
	error = clGetPlatformIDs(1, &platform, &platforms);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't get platform ids\n");
		goto out;
	}
	printf("Number of available platforms = %d\n",platforms);
	
	/* Get the name of the platform and print it */
	error = clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeof(name), name, NULL);
	if (error != CL_SUCCESS) 
	{
		fprintf(stderr,"Couldn't get platform name\n");
		goto out;
	}
	printf("Platform = %s\n",name);

	/* Now get the first device of our platform */
	error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, &devices);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't get device ids\n");
		goto out;
	}

	/**** Phase 2: Create the context and all of its associates *****/

	/* Create a CL context for the selected device, note that nVidia's OpenCL requires the platform property */
	cl_context_properties properties[]=
	{
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
		0
	};
	context = clCreateContext(properties, 1, &device, NULL, NULL, &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create the context\n");
		goto out;
	}
	
	/* Now create the command queue */
	cl_command_queue cq = clCreateCommandQueue(context, device, 0, &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create the command queue\n");
		goto out;                
	}

	/* Now read in the file with the kernel */
    if (!(k_src = read_file_contents("kernel.cl")))
    {
		fprintf(stderr,"Couldn't read the contents of the kernel file!\n");
		goto out;
    }

    /* Array of all sources, we only have one source file */
	const char *srcptr[]={k_src};

	/* Submit the just loaded source code to OpenCL. The resulting
	 * program/kernel is associated with the given context */
	cl_program prog = clCreateProgramWithSource(context, 1, srcptr, NULL, &error);
	if (error != CL_SUCCESS) 
	{
		fprintf(stderr,"Couldn't create the kernel from source\n");
		goto out;
	}

	/* Now compile */
	error = clBuildProgram(prog, 0, NULL, "", NULL, NULL);
	if (error != CL_SUCCESS )
	{
		char build_c[4096];

		fprintf(stderr, "Error on buildProgram\n");
		fprintf(stderr, "\nRequestingInfo\n");
		clGetProgramBuildInfo( prog, device, CL_PROGRAM_BUILD_LOG, sizeof(build_c), build_c, NULL );
		fprintf(stderr, "Build Log for %s_program:\n%s\n", "kernel", build_c );
		goto out;
	}

	/* Allocate the input memory for the kernel. In the kernel, we only read from it,
	 * hence we declare it as read only. */
	cl_mem cl_srcmem1, cl_srcmem2, cl_destmem;
	cl_srcmem1 = clCreateBuffer(context, CL_MEM_READ_ONLY, memworksize, NULL, &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create read only buffer!\n");
		goto out;
	}
	cl_srcmem2 = clCreateBuffer(context, CL_MEM_READ_ONLY, memworksize, NULL, &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create read only buffer!\n");
		goto out;
	}
    
	/* In the kernel, we actually copy the contents of the input buffer another buffer.
	 * Hence, we declare this buffer as write only.
	 */
	cl_destmem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, memworksize, NULL, &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create write only buffer!\n");
		goto out;
	}

	/* We now get access to a kernel handle */
	cl_kernel k_example = clCreateKernel(prog, "add", &error);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't create kernel!\n");
		goto out;
	}
	/* We declare that the cl_srcmem1 is the first parameter */
	error = clSetKernelArg(k_example, 0, sizeof(cl_srcmem1), &cl_srcmem1);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't set first argument for kernel!\n");
		goto out;
	}
	/* We declare that the cl_srcmem2 is the second parameter */
	error = clSetKernelArg(k_example, 1, sizeof(cl_srcmem2), &cl_srcmem2);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't set second argument for kernel!\n");
		goto out;
	}
	/* And we declare that cl_destmem is the third parameter */
	error = clSetKernelArg(k_example, 2, sizeof(cl_destmem), &cl_destmem);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't set third argument for kernel!\n");
		goto out;
	}

	/**** Phase 3: Fill and run the command queue *****/

	/* Send input data to OpenCL (async, don't alter the buffer!) */
	error = clEnqueueWriteBuffer(cq, cl_srcmem1, CL_FALSE, 0, memworksize, a, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't transfer source to target mem!\n");
		goto out;
	}
	error = clEnqueueWriteBuffer(cq, cl_srcmem2, CL_FALSE, 0, memworksize, b, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't transfer source to target mem!\n");
		goto out;
	}
	/* Start the "calculation" */
	error = clEnqueueNDRangeKernel(cq, k_example, 1, NULL, &worksteps, NULL, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't enqueue the kernel!\n");
		goto out;
	}
	/* Read the result back into dest */
	error = clEnqueueReadBuffer(cq, cl_destmem, CL_FALSE, 0, memworksize, dest, 0, NULL, NULL);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Couldn't read buffer back!\n");
		goto out;
	}
	/* We have now submitted all commands into the queue. As this was done asynch, we have to
	 * wait for completion of all the commands */
	error = clFinish(cq);
	if (error != CL_SUCCESS)
	{
		fprintf(stderr,"Unable to finish calculation!\n");
		goto out;
	}

	rc = EXIT_SUCCESS;
out:
	if (error != CL_SUCCESS)
		fprintf(stderr,"Error number %d\n", error);

	/**** Phase 4: Clean up  *****/
	if (cl_srcmem1) clReleaseMemObject(cl_srcmem1);
	if (cl_srcmem2) clReleaseMemObject(cl_srcmem2);
	if (cl_destmem) clReleaseMemObject(cl_destmem);
	if (context) clReleaseContext(context);

	free(k_src);

	return rc;
}
