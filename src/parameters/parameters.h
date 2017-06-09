#ifndef _JH_CLI_PARAMETERS_H_
#define _JH_CLI_PARAMETERS_H_

#include "parameters_types.h"

int JH_parameters_initialize
(
   struct JH_parameters param [const restrict static 1],
   int const argc,
   const char * argv [const static argc]
);

const int JH_parameters_get_reply_rate
(
   const struct JH_parameters param [const restrict static 1]
);

const char * JH_parameters_get_socket_name
(
   const struct JH_parameters param [const restrict static 1]
);

const char * JH_parameters_get_dest_socket_name
(
   const struct JH_parameters param [const restrict static 1]
);

#endif
