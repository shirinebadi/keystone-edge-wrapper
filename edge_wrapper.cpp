#include "edge_wrapper.h"
#include <string.h>

#define OCALL_PRINT_STRING 1
#define OCALL_WAIT_FOR_MESSAGE 2

unsigned long
print_string(char* str) {
  return printf("Enclave said: \"%s\"\n", str);
}

encl_message_t wait_for_message(){

  const char* message = "This is Shirin.";
  size_t len = strlen(message) + 1;  // +1 for null terminator

  void* buffer = malloc(len);
  if (buffer == NULL) {
      // Handle memory allocation failure
      printf("Memory allocation failed\n");
      exit(1);
  }

  memcpy(buffer, message, len);
  printf("Message Received\n");

  encl_message_t message;
  message.host_ptr = buffer;
  message.len = len;
  return message;
}


void
edge_init(Keystone::Enclave* enclave) {
  enclave->registerOcallDispatch(incoming_call_dispatch);

  register_call(OCALL_PRINT_STRING, print_value_wrapper);
  register_call(OCALL_WAIT_FOR_MESSAGE, wait_for_message_wrapper);

  edge_call_init_internals(
      (uintptr_t)enclave->getSharedBuffer(), enclave->getSharedBufferSize());
}

void
print_value_wrapper(void* buffer) {
  
  struct edge_call* edge_call = (struct edge_call*)buffer;
  uintptr_t call_args;
  unsigned long ret_val;
  size_t arg_len;
  if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
    edge_call->return_data.call_status = CALL_STATUS_BAD_OFFSET;
    return;
  }

  ret_val = print_string((char*)call_args);

  uintptr_t data_section = edge_call_data_ptr();
  memcpy((void*)data_section, &ret_val, sizeof(unsigned long));
  if (edge_call_setup_ret(
          edge_call, (void*)data_section, sizeof(unsigned long))) {
    edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
  } else {
    edge_call->return_data.call_status = CALL_STATUS_OK;
  }

  return;
}

void wait_for_message_wrapper(void* buffer)
{

  struct edge_call* edge_call = (struct edge_call*)buffer;

  uintptr_t call_args;
  unsigned long ret_val;
  size_t args_len;
  if(edge_call_args_ptr(edge_call, &call_args, &args_len) != 0){
    edge_call->return_data.call_status = CALL_STATUS_BAD_OFFSET;
    return;
  }

  encl_message_t host_msg = wait_for_message();

  if( edge_call_setup_wrapped_ret(edge_call, host_msg.host_ptr, host_msg.len)){
    edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
  }
  else{
    edge_call->return_data.call_status = CALL_STATUS_OK;
  }

  return;
}

