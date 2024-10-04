#ifndef _EDGE_WRAPPER_H_
#define _EDGE_WRAPPER_H_
#include "edge_call.h"

void edge_init();
unsigned long ocall_print_value(char* string);
void ocall_wait_for_message(struct edge_data *msg);

#endif /* _EDGE_WRAPPER_H_ */
