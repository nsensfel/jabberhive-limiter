#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "../filter/filter.h"

#include "server.h"

static int initialize
(
   struct JH_server_worker worker [const restrict static 1],
   void * input
)
{
   const int old_errno = errno;

   memcpy
   (
      (void *) &(worker->params),
      (const void *) input,
      sizeof(struct JH_server_thread_parameters)
   );

   pthread_barrier_wait(&(worker->params.thread_collection->barrier));

   return 0;
}

static void finalize
(
   struct JH_server_worker worker [const restrict static 1]
)
{
   close(worker->params.socket);

   pthread_mutex_lock(&(worker->params.thread_collection->mutex));

   worker->params.thread_collection->threads[worker->params.thread_id].state =
      JH_SERVER_JOINING_THREAD;

   pthread_mutex_unlock(&(worker->params.thread_collection->mutex));
}

void * JH_server_worker_main (void * input)
{
   struct JH_limiter_filter filter;
   struct JH_server_worker worker;

   initialize(&worker, input);

   if (JH_limiter_filter_initialize(&filter, worker.params.socket) < 0)
   {
      finalize(&worker);

      return NULL;
   }

   while (JH_server_is_running())
   {
      if (JH_limiter_filter_step(&filter, worker.params.server_params) < 0)
      {
         break;
      }
   }

   JH_limiter_filter_finalize(&filter);

   finalize(&worker);

   return NULL;
}
