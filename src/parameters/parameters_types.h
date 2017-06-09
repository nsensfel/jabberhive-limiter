#ifndef _JH_CLI_PARAMETERS_TYPES_H_
#define _JH_CLI_PARAMETERS_TYPES_H_

#define JH_PARAMETERS_COUNT 3

struct JH_parameters
{
   int reply_rate;

   /* JH **********************************************************************/
   const char * restrict socket_name;
   const char * restrict dest_socket_name;
};

#endif
