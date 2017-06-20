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
   int retries;
   int state;

   initialize(&worker, input);

   retries = 0;

   if (JH_limiter_filter_initialize(&filter, worker.params.socket) < 0)
   {
      finalize(&worker);

      return NULL;
   }

   while (JH_server_is_running())
   {
      state = JH_limiter_filter_step(&filter, worker.params.server_params);

      switch (state)
      {
         case 0:
            retries = 0;
            break;

         case 1:
            retries += 1;

            if (retries == JH_SERVER_WORKER_MAX_RETRIES)
            {
               JH_S_ERROR
               (
                  stderr,
                  "Thread's upstream socket timed out too many consequent"
                  " times."
               );

               JH_limiter_filter_finalize(&filter);

               finalize(&worker);

               return NULL;

            }
            else
            {
               sleep(JH_SERVER_WORKER_RETRY_DELAY_SEC);
            }
            break;

         default:
            JH_limiter_filter_finalize(&filter);

            finalize(&worker);

            return NULL;
      }
   }

   JH_limiter_filter_finalize(&filter);

   finalize(&worker);

   return NULL;
}
