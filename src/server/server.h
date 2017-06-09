#ifndef _JH_SERVER_SERVER_H_
#define _JH_SERVER_SERVER_H_

#include "../parameters/parameters_types.h"

#include "server_types.h"

int JH_server_initialize
(
   struct JH_server server [const restrict static 1],
   const struct JH_parameters params [const restrict static 1]
);

int JH_server_socket_open
(
   struct JH_server_socket server_socket [const restrict static 1],
   const char socket_name [const restrict static 1]
);

void JH_server_request_termination (void);
int JH_server_is_running (void);
int JH_server_set_signal_handlers (void);

int JH_server_main
(
   const struct JH_parameters params [const restrict static 1]
);

void JH_server_finalize (struct JH_server [const restrict static 1]);

int JH_server_wait_for_event
(
   struct JH_server server [const restrict static 1]
);

void JH_server_handle_joining_threads
(
   struct JH_server server [const restrict static 1]
);

int JH_server_handle_new_connection
(
   struct JH_server server [const restrict static 1]
);

void * JH_server_worker_main (void * input);

int JH_server_worker_receive
(
   struct JH_server_worker worker [const restrict static 1]
);

int JH_server_worker_handle_request
(
   struct JH_server_worker worker [const restrict static 1]
);

int JH_server_worker_send_confirm_pipelining_support
(
   struct JH_server_worker worker [const restrict static 1]
);

int JH_server_worker_send_confirm_protocol_version
(
   struct JH_server_worker worker [const restrict static 1]
);

int JH_server_worker_send_positive
(
   struct JH_server_worker worker [const restrict static 1]
);

int JH_server_worker_send_negative
(
   struct JH_server_worker worker [const restrict static 1]
);

int JH_server_worker_send_generated_reply
(
   struct JH_server_worker worker [const restrict static 1]
);

#endif
