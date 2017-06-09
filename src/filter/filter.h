#ifndef _JH_LIMITER_FILTER_H_
#define _JH_LIMITER_FILTER_H_

#include "../parameters/parameters_types.h"

#include "filter_types.h"

int JH_limiter_filter_initialize
(
   struct JH_limiter_filter filter [const restrict static 1],
   const int upstream_socket
);

int JH_limiter_filter_step
(
   struct JH_limiter_filter filter [const restrict static 1],
   const struct JH_parameters params [const restrict static 1]
);

void JH_limiter_filter_finalize
(
   struct JH_limiter_filter filter [const restrict static 1]
);

#endif
