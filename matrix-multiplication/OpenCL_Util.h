#define CL_TARGET_OPENCL_VERSION 220

#include <CL/cl.h>

#define OPENCL_CHECK(ans) { opencl_assert((ans), __FILE__, __LINE__); }

void opencl_assert(cl_int err, const char *file, int line);
cl_device_id opencl_init();
cl_context opencl_create_context(cl_device_id);
cl_command_queue opencl_create_command_queue(cl_device_id, cl_context);
cl_program opencl_compile_program(cl_device_id , cl_context, const char* source);
cl_kernel opencl_create_kernel(cl_program, const char* name);
cl_mem opencl_create_buffer(cl_context, cl_mem_flags, size_t size);
void opencl_set_kernel_cl_mem_arg(cl_kernel, cl_mem arg_value, int arg_index);
void opencl_set_kernel_int_arg(cl_kernel, int arg_value, int arg_index);
