#include <sys/socket.h>
#include <sys/un.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "error/error.h"
#include "parameters/parameters.h"
#include "server/server.h"

#include "pervasive.h"

static void print_help (const char runnable [const restrict static 1])
{
   printf
   (
      "JabberHive - Limiter\n"
      "Software Version %d\n"
      "Protocol Version %d\n"
      "\nUsages:\n"
      "   JH GATEWAY:\t%s SOCKET_NAME DESTINATION REPLY_RATE\n"
      "   SHOW HELP:\tAnything else.\n"
      "\nParameters:\n"
      "   SOCKET_NAME:\tValid UNIX socket.\n"
      "   DESTINATION:\tValid UNIX socket.\n"
      "   REPLY_RATE:\tInteger [0,100].\n",
      JH_PROGRAM_VERSION,
      JH_PROTOCOL_VERSION,
      runnable
   );
}


int main (int const argc, const char * argv [const static argc])
{
   struct JH_parameters params;

   if (JH_parameters_initialize(&params, argc, argv) < 0)
   {
      print_help(argv[0]);

      return -1;
   }

   if (JH_server_main(&params) < 0)
   {
      return -1;
   }

   return 0;
}
