#include <sys/socket.h>
#include <sys/un.h>

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "../pervasive.h"
#include "../error/error.h"
#include "../parameters/parameters.h"

#include "filter.h"

static const char REQUEST_TEMPLATE[] = {'?', 'R', 'L', 'R', ' '};

static int connect_downstream
(
   struct JH_limiter_filter filter [const restrict static 1],
   const struct JH_parameters params [const restrict static 1]
)
{
   struct sockaddr_un addr;

   const int old_errno = errno;

   errno = 0;

   if ((filter->downstream_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
   {
      JH_FATAL
      (
         stderr,
         "Unable to create socket: %s.",
         strerror(errno)
      );

      errno = old_errno;

      return -1;
   }

   errno = old_errno;

   memset((void *) &addr, (int) 0, sizeof(addr));

   addr.sun_family = AF_UNIX;

   strncpy
   (
      (char *) addr.sun_path,
      JH_parameters_get_dest_socket_name(params),
      (sizeof(addr.sun_path) - ((size_t) 1))
   );

   errno = 0;

   if
   (
      connect
      (
         filter->downstream_socket,
         (struct sockaddr *) &addr,
         sizeof(addr)
      )
      == -1
   )
   {
      JH_FATAL
      (
         stderr,
         "Unable to connect to address: %s.",
         strerror(errno)
      );

      errno = old_errno;

      close(filter->downstream_socket);

      return -1;
   }

   errno = old_errno;

   filter->state = JH_LIMITER_IS_LISTENING_UPSTREAM;

   return 0;
}

static int listen_to_upstream
(
   struct JH_limiter_filter filter [const restrict static 1],
   const struct JH_parameters params [const restrict static 1]
)
{
   ssize_t io_bytes;
   const int old_errno = errno;

   for (;;)
   {
      errno = 0;
      io_bytes =
         read
         (
            filter->upstream_socket,
            (void *) (filter->buffer + filter->buffer_index),
            sizeof(char)
         );

      if (io_bytes == -1)
      {
         JH_ERROR
         (
            stderr,
            "Upstream read error %d (\"%s\").",
            errno,
            strerror(errno)
         );

         errno = old_errno;

         return -1;
      }

      errno = old_errno;

      if
      (
         filter->buffer[filter->buffer_index]
         != REQUEST_TEMPLATE[filter->buffer_index]
      )
      {
         filter->buffer_index += 1;
         filter->state = JH_LIMITER_IS_SENDING_DOWNSTREAM;

         return 0;
      }

      filter->buffer_index += 1;

      if (filter->buffer_index == 5)
      {
         if (rand() >= JH_parameters_get_reply_rate(params))
         {
            strncpy
            (
               filter->buffer,
               "?RL ",
               (((size_t) 4) * sizeof(char))
            );

            filter->buffer_index = 4;
         }

         filter->state = JH_LIMITER_IS_SENDING_DOWNSTREAM;

         return 0;
      }
   }

   return -1;
}

static int send_downstream
(
   struct JH_limiter_filter filter [const restrict static 1]
)
{
   ssize_t io_bytes;
   const int old_errno = errno;

   if (filter->buffer_index > 0)
   {
      errno = 0;

      io_bytes =
         write
         (
            filter->downstream_socket,
            (void *) filter->buffer,
            (((size_t) filter->buffer_index) * sizeof(char))
         );

      filter->buffer_index = 0;

      if (io_bytes == -1)
      {
         JH_ERROR
         (
            stderr,
            "Downstream write error %d (\"%s\").",
            errno,
            strerror(errno)
         );

         errno = old_errno;

         return -1;
      }

      errno = old_errno;
   }

   for (;;)
   {
      errno = 0;
      io_bytes =
         read
         (
            filter->upstream_socket,
            (void *) filter->buffer,
            sizeof(char)
         );

      if (io_bytes == -1)
      {
         JH_ERROR
         (
            stderr,
            "Upstream read error %d (\"%s\").",
            errno,
            strerror(errno)
         );

         errno = old_errno;

         return -1;
      }

      errno = 0;

      io_bytes = write
      (
         filter->downstream_socket,
         (void *) filter->buffer,
         sizeof(char)
      );

      if (io_bytes == -1)
      {
         JH_ERROR
         (
            stderr,
            "Upstream write error %d (\"%s\").",
            errno,
            strerror(errno)
         );

         errno = old_errno;

         return -1;
      }

      errno = old_errno;

      if (filter->buffer[0] == '\n')
      {
         filter->state = JH_LIMITER_IS_SENDING_UPSTREAM;

         return 0;
      }
   }
}

static int send_upstream
(
   struct JH_limiter_filter filter [const restrict static 1]
)
{
   ssize_t io_bytes;
   const int old_errno = errno;

   for (;;)
   {
      errno = 0;

      io_bytes =
         read
         (
            filter->downstream_socket,
            (void *) filter->buffer,
            sizeof(char)
         );

      if (io_bytes == -1)
      {
         JH_ERROR
         (
            stderr,
            "Downstream read error %d (\"%s\").",
            errno,
            strerror(errno)
         );

         errno = old_errno;

         return -1;
      }

      errno = 0;

      io_bytes = write
      (
         filter->upstream_socket,
         (void *) filter->buffer,
         sizeof(char)
      );

      if (io_bytes == -1)
      {
         JH_ERROR
         (
            stderr,
            "Upstream write error %d (\"%s\").",
            errno,
            strerror(errno)
         );

         errno = old_errno;

         return -1;
      }

      errno = old_errno;

      switch (filter->buffer_index)
      {
         case -1:
            if (filter->buffer[0] == '\n')
            {
               filter->buffer_index = 0;
            }
            break;

         case 0:
            if (filter->buffer[0] == '!')
            {
               filter->buffer_index = 1;
            }
            else
            {
               filter->buffer_index = -1;
            }
            break;

         case 1:
            if ((filter->buffer[0] == 'N') || (filter->buffer[0] == 'P'))
            {
               filter->buffer_index = 2;
            }
            else
            {
               filter->buffer_index = -1;
            }
            break;

         case 2:
            if (filter->buffer[0] == ' ')
            {
               filter->buffer_index = 3;
            }
            else
            {
               filter->buffer_index = -1;
            }
            break;

         case 3:
            if (filter->buffer[0] == '\n')
            {
               filter->buffer_index = 0;
               filter->state = JH_LIMITER_IS_LISTENING_UPSTREAM;

               return 0;
            }
            else
            {
               filter->buffer_index = -1;
            }
            break;

         default:
            JH_PROG_ERROR
            (
               stderr,
               "Invalid value for 'filter->buffer_index': %d.",
               filter->buffer_index
            );

            filter->buffer_index = 0;

            return -1;
      }
   }

   return -1;
}

/******************************************************************************/
/** EXPORTED ******************************************************************/
/******************************************************************************/

int JH_limiter_filter_step
(
   struct JH_limiter_filter filter [const restrict static 1],
   const struct JH_parameters params [const restrict static 1]
)
{
   switch (filter->state)
   {
      case JH_LIMITER_IS_CONNECTING:
         JH_DEBUG(stderr, 1, "<CONNECTING> (index: %d)", filter->buffer_index);
         return connect_downstream(filter, params);

      case JH_LIMITER_IS_LISTENING_UPSTREAM:
         JH_DEBUG(stderr, 1, "<LISTENING_UP> (index: %d)", filter->buffer_index);
         return listen_to_upstream(filter, params);

      case JH_LIMITER_IS_SENDING_DOWNSTREAM:
         JH_DEBUG(stderr, 1, "<SENDING_DOWN> (index: %d)", filter->buffer_index);
         return send_downstream(filter);

      case JH_LIMITER_IS_SENDING_UPSTREAM:
         JH_DEBUG(stderr, 1, "<SENDING_UP> (index: %d)", filter->buffer_index);
         return send_upstream(filter);

      default:
         return -1;
   }
}

int JH_limiter_filter_initialize
(
   struct JH_limiter_filter filter [const restrict static 1],
   const int upstream_socket
)
{
   filter->state = JH_LIMITER_IS_CONNECTING;
   filter->buffer_index = 0;
   filter->upstream_socket = upstream_socket;
   filter->downstream_socket = -1;

   return 0;
}

void JH_limiter_filter_finalize
(
   struct JH_limiter_filter filter [const restrict static 1]
)
{

   if (filter->upstream_socket != -1)
   {
      close(filter->upstream_socket);

      filter->upstream_socket = -1;
   }

   if (filter->downstream_socket != -1)
   {
      close(filter->downstream_socket);

      filter->downstream_socket = -1;
   }
}
