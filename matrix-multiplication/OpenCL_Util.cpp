#include <cstring>
#include <iostream>

#include "OpenCL_Util.h"

using namespace std;

static const char *opencl_error_string(cl_int error)
{
  switch (error) {
  // run-time and JIT compiler errors
  case 0:
    return "CL_SUCCESS";
  case -1:
    return "CL_DEVICE_NOT_FOUND";
  case -2:
    return "CL_DEVICE_NOT_AVAILABLE";
  case -3:
    return "CL_COMPILER_NOT_AVAILABLE";
  case -4:
    return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
  case -5:
    return "CL_OUT_OF_RESOURCES";
  case -6:
    return "CL_OUT_OF_HOST_MEMORY";
  case -7:
    return "CL_PROFILING_INFO_NOT_AVAILABLE";
  case -8:
    return "CL_MEM_COPY_OVERLAP";
  case -9:
    return "CL_IMAGE_FORMAT_MISMATCH";
  case -10:
    return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
  case -11:
    return "CL_BUILD_PROGRAM_FAILURE";
  case -12:
    return "CL_MAP_FAILURE";
  case -13:
    return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
  case -14:
    return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
  case -15:
    return "CL_COMPILE_PROGRAM_FAILURE";
  case -16:
    return "CL_LINKER_NOT_AVAILABLE";
  case -17:
    return "CL_LINK_PROGRAM_FAILURE";
  case -18:
    return "CL_DEVICE_PARTITION_FAILED";
  case -19:
    return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

  // compile-time errors
  case -30:
    return "CL_INVALID_VALUE";
  case -31:
    return "CL_INVALID_DEVICE_TYPE";
  case -32:
    return "CL_INVALID_PLATFORM";
  case -33:
    return "CL_INVALID_DEVICE";
  case -34:
    return "CL_INVALID_CONTEXT";
  case -35:
    return "CL_INVALID_QUEUE_PROPERTIES";
  case -36:
    return "CL_INVALID_COMMAND_QUEUE";
  case -37:
    return "CL_INVALID_HOST_PTR";
  case -38:
    return "CL_INVALID_MEM_OBJECT";
  case -39:
    return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
  case -40:
    return "CL_INVALID_IMAGE_SIZE";
  case -41:
    return "CL_INVALID_SAMPLER";
  case -42:
    return "CL_INVALID_BINARY";
  case -43:
    return "CL_INVALID_BUILD_OPTIONS";
  case -44:
    return "CL_INVALID_PROGRAM";
  case -45:
    return "CL_INVALID_PROGRAM_EXECUTABLE";
  case -46:
    return "CL_INVALID_KERNEL_NAME";
  case -47:
    return "CL_INVALID_KERNEL_DEFINITION";
  case -48:
    return "CL_INVALID_KERNEL";
  case -49:
    return "CL_INVALID_ARG_INDEX";
  case -50:
    return "CL_INVALID_ARG_VALUE";
  case -51:
    return "CL_INVALID_ARG_SIZE";
  case -52:
    return "CL_INVALID_KERNEL_ARGS";
  case -53:
    return "CL_INVALID_WORK_DIMENSION";
  case -54:
    return "CL_INVALID_WORK_GROUP_SIZE";
  case -55:
    return "CL_INVALID_WORK_ITEM_SIZE";
  case -56:
    return "CL_INVALID_GLOBAL_OFFSET";
  case -57:
    return "CL_INVALID_EVENT_WAIT_LIST";
  case -58:
    return "CL_INVALID_EVENT";
  case -59:
    return "CL_INVALID_OPERATION";
  case -60:
    return "CL_INVALID_GL_OBJECT";
  case -61:
    return "CL_INVALID_BUFFER_SIZE";
  case -62:
    return "CL_INVALID_MIP_LEVEL";
  case -63:
    return "CL_INVALID_GLOBAL_WORK_SIZE";
  case -64:
    return "CL_INVALID_PROPERTY";
  case -65:
    return "CL_INVALID_IMAGE_DESCRIPTOR";
  case -66:
    return "CL_INVALID_COMPILER_OPTIONS";
  case -67:
    return "CL_INVALID_LINKER_OPTIONS";
  case -68:
    return "CL_INVALID_DEVICE_PARTITION_COUNT";

  // extension errors
  case -1000:
    return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
  case -1001:
    return "CL_PLATFORM_NOT_FOUND_KHR";
  case -1002:
    return "CL_INVALID_D3D10_DEVICE_KHR";
  case -1003:
    return "CL_INVALID_D3D10_RESOURCE_KHR";
  case -1004:
    return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
  case -1005:
    return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
  default:
    return "Unknown OpenCL error";
  }
}

void opencl_assert(cl_int err, const char *file, int line)
{
  if (err < 0) {
    cerr << "opencl_assert: " << opencl_error_string(err) << " " << file << ":" << line;
    exit(1);
  }
}

cl_device_id opencl_init()
{
  cl_platform_id platform;
  cl_device_id device;

  // obtain the list of platforms available
  cl_int err = clGetPlatformIDs(1, &platform, NULL);
  if (err < 0) {
    cerr << "Failed to identify a platform: " << opencl_error_string(err) << endl;
    exit(1);
  }

  // obtain the list of GPU devices available on a platform
  err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
  if (err == CL_DEVICE_NOT_FOUND) {
    cerr << "Warning: GPU not found; falling back to CPU" << endl;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
  }

  if (err < 0) {
    cerr << "Failed to access any devices: " << opencl_error_string(err) << endl;
    exit(1);
  }

  return device;
}

cl_context opencl_create_context(cl_device_id device)
{
  cl_int err;
  cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
  if (err < 0) {
    cerr << "Failed to create a context: " << opencl_error_string(err) << endl;
    exit(1);
  }

  return context;
}

cl_command_queue opencl_create_command_queue(cl_device_id device, cl_context context)
{
  cl_int err;
  cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
  if (err < 0) {
    cerr << "Failed to create a command queue: " << opencl_error_string(err) << endl;
    exit(1);
  }

  return queue;
}

cl_program opencl_compile_program(cl_device_id device, cl_context context, const char* source)
{
  cl_program program;
  size_t size = strlen(source);

  cl_int err;
  program = clCreateProgramWithSource(context, 1, &source, &size, &err);
  if (err < 0) {
    cerr << "Failed to create program with source: " << opencl_error_string(err) << endl;
    exit(1);
  }


  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (err < 0) {
    cerr << "Failed to build program: " << opencl_error_string(err) << endl;
    exit(1);
  }

  return program;
}

cl_kernel opencl_create_kernel(cl_program program, const char* name)
{
  cl_int err;
  cl_kernel kernel = clCreateKernel(program, name, &err);
  if (err < 0) {
    cerr << "Failed to create kernel: " << opencl_error_string(err) << endl;
    exit(1);
  }

  return kernel;
}

cl_mem opencl_create_buffer(cl_context context, cl_mem_flags flags, size_t size)
{
  cl_int err;
  cl_mem mem = clCreateBuffer(context, flags, size, nullptr, &err);
  if (err < 0) {
    cerr << "Failed to create buffer: " << opencl_error_string(err) << endl;
    exit(1);
  }

  return mem;
}

void opencl_set_kernel_cl_mem_arg(cl_kernel kernel, cl_mem arg_value, int arg_index)
{
  cl_int err = clSetKernelArg(kernel, arg_index, sizeof(cl_mem), &arg_value);
  if (err < 0) {
    cerr << "Failed to set kernel cl_mem arg: " << opencl_error_string(err) << endl;
    exit(1);
  }
}

void opencl_set_kernel_int_arg(cl_kernel kernel, int arg_value, int arg_index)
{
  cl_int err = clSetKernelArg(kernel, arg_index, sizeof(int), (void*)&arg_value);
  if (err < 0) {
    cerr << "Failed to set kernel int arg: " << opencl_error_string(err) << endl;
    exit(1);
  }
}
