#include <signal.h>
#include <string.h>
#include <stdio.h>

#include "server.h"

static volatile char JH_SERVER_IS_RUNNING = (char) 1;

static void request_termination (int const signo)
{
   if ((signo == SIGINT) || (signo == SIGTERM))
   {
      JH_server_request_termination();
   }
}

void JH_server_request_termination (void)
{
   JH_SERVER_IS_RUNNING = (char) 0;
}

int JH_server_is_running (void)
{
   return (int) JH_SERVER_IS_RUNNING;
}

int JH_server_set_signal_handlers (void)
{
   /*
   struct sigaction act;

      act.sa_handler = request_termination;
      act.sa_mask =
      act.sa_flags =
      act.sa_restorer =
   */

   /* TODO */

   return -1;
}
