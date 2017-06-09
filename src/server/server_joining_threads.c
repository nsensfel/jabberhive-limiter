#include <sys/socket.h>

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "../parameters/parameters.h"

#include "server.h"

void JH_server_handle_joining_threads
(
   struct JH_server server [const restrict static 1]
)
{
   JH_index i;

   pthread_mutex_lock(&(server->workers.mutex));

   for (i = 0; i < server->workers.threads_capacity; ++i)
   {
      if (server->workers.threads[i].state == JH_SERVER_JOINING_THREAD)
      {
         JH_DEBUG(stderr, 1, "Joining thread %u", i);

         pthread_join(server->workers.threads[i].posix_id, (void **) NULL);

         server->workers.threads[i].state = JH_SERVER_NO_THREAD;

         server->workers.currently_running -= 1;
      }
   }

   pthread_mutex_unlock(&(server->workers.mutex));
}
