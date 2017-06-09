#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "../error/error.h"

#include "parameters.h"

static int parse_reply_rate
(
   struct JH_parameters param [const restrict static 1],
   const char argv [const restrict]
)
{
   long long int input;
   const int old_errno = errno;

   errno = 0;

   input = strtoll(argv, (char **) NULL, 10);

   if
   (
      (errno != 0)
      || (input > (long long int) 100)
      || (input < 0)
   )
   {
      JH_FATAL
      (
         stderr,
         "Invalid or value for parameter 'reply_rate', accepted "
         "range is "
         "[0, %hu] (integer).",
         100
      );

      errno = old_errno;

      return -1;
   }

   param->reply_rate =
      (int)
      (
         ((double) RAND_MAX)
         * (((double) input) * ((double) 0.01))
      );

   errno = old_errno;

   return 0;
}

static void set_default_to_all_fields
(
   struct JH_parameters param [const restrict static 1]
)
{
   param->reply_rate = -1;
   param->socket_name = (const char *) NULL;
   param->dest_socket_name = (const char *) NULL;
}

static int is_valid
(
   struct JH_parameters param [const restrict static 1]
)
{
   int valid;

   valid = 1;

   if (param->socket_name == (const char *) NULL)
   {
      JH_S_FATAL(stderr, "Missing parameter: This entity's socket name.");

      valid = 0;
   }

   if (param->dest_socket_name == (const char *) NULL)
   {
      JH_S_FATAL(stderr, "Missing parameter: The destination's socket name.");

      valid = 0;
   }

   if (param->reply_rate == -1)
   {
      JH_S_FATAL(stderr, "Missing parameter: The reply rate.");

      valid = 0;
   }

   return valid;
}

static void set_parameters
(
   struct JH_parameters param [const restrict static 1],
   int const argc,
   const char * argv [const static argc]
)
{
   if (argc < 2)
   {
      return;
   }

   param->socket_name = argv[1];

   if (argc < 3)
   {
      return;
   }

   param->dest_socket_name = argv[2];

   if (argc < 4)
   {
      return;
   }

   parse_reply_rate(param, argv[3]);
}

int JH_parameters_initialize
(
   struct JH_parameters param [const restrict static 1],
   int const argc,
   const char * argv [const static argc]
)
{
   set_default_to_all_fields(param);

   set_parameters(param, argc, argv);

   if (!is_valid(param))
   {
      return -1;
   }

   return 0;
}
