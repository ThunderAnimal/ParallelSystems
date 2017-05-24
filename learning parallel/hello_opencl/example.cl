/**
 * This is the simple kernel. For the current work item,
 * which can be addressed using the value returned by
 * get_global_id(0), copy the contents from src to dest.
 */
__kernel void example( __global char *src, __global char *dest)
{
       int x = get_global_id(0);

       dest[x] = src[x];
}
