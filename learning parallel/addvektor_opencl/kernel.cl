/**
 * This is the simple kernel. For the current work item,
 * which can be addressed using the value returned by
 * get_global_id(0), copy the contents from src to dest.
 */
__kernel void add( __global float *src_a, __global float *src_b, __global float *dest)
{		
       int x = get_global_id(0);

       dest[x] = src_a[x] + src_b[x];
}
