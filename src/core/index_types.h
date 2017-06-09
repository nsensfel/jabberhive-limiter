#ifndef _JH_CORE_INDEX_TYPES_H_
#define _JH_CORE_INDEX_TYPES_H_

#include "../pervasive.h"

/*
 * JH_index is a replacement for size_t. As many indices are stored for every
 * word learned, having control over which type of variable is used to represent
 * those indices lets us scale the RAM usage.
 */

#include <limits.h>
#include <stdint.h>

/* Must be unsigned. */
typedef unsigned int JH_index;

/* Must be > 0. */
#define JH_INDEX_MAX UINT_MAX

#ifndef JH_RUNNING_FRAMA_C
   #if (JH_INDEX_MAX > SIZE_MAX)
      #error "JH_index should not be able to go higher than a size_t variable."
   #endif
#endif

#endif
