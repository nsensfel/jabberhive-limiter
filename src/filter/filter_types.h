#ifndef _JH_LIMITER_FILTER_TYPES_H_
#define _JH_LIMITER_FILTER_TYPES_H_

#define JH_LIMITER_FILTER_BUFFER_SIZE 5
enum JH_limiter_filter_state
{
   JH_LIMITER_IS_CONNECTING,
   JH_LIMITER_IS_LISTENING_UPSTREAM,
   JH_LIMITER_IS_SENDING_DOWNSTREAM,
   JH_LIMITER_IS_SENDING_UPSTREAM
};

struct JH_limiter_filter
{
   enum JH_limiter_filter_state state;
   char buffer[JH_LIMITER_FILTER_BUFFER_SIZE];
   int buffer_index;
   int upstream_socket;
   int downstream_socket;
};

#endif
