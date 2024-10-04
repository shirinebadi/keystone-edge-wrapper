#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"
#include "edge_wrapper.h"

#define OCALL_PRINT_STRING 1
#define OCALL_WAIT_FOR_MESSAGE 2

void edge_init(){
}

unsigned long ocall_print_value(char* string){
  unsigned long retval;
  ocall(OCALL_PRINT_STRING, string, strlen(string)+1, &retval ,sizeof(unsigned long));
  return retval;
}

void ocall_wait_for_message(struct edge_data *msg){
  ocall(OCALL_WAIT_FOR_MESSAGE, NULL, 0, msg, sizeof(struct edge_data));
}

