#ifndef _EDGE_WRAPPER_H_
#define _EDGE_WRAPPER_H_

#include "edge_call.h"
#include "keystone.h"

typedef struct encl_message_t {
  void* host_ptr;
  size_t len;
} encl_message_t;

typedef unsigned char byte;

typedef struct network_message_t {
  size_t data_len;
  edge_data_offset offset;
} network_message_t;

typedef struct data_message_t {
  unsigned int msg_type;

  size_t data_len;

  unsigned char data[];

} edge_data_message_t;

void edge_init(Keystone::Enclave* enclave);

void print_value_wrapper(void* buffer);
unsigned long print_value(char* str);

void wait_for_message_wrapper(void* buffer);
encl_message_t wait_for_message();

#endif /* _EDGE_WRAPPER_H_ */
