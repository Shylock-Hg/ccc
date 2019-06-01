/*! \brief
 *  \author Shylock Hg
 *  \date 2019-04-23
 *  \email tcath2s@gmail.com
 * */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/*! \brief check the validation of variadic parameters
 *  \param dummy required before variadic parameters
 *  \param ... the variadic parameters
 * */
static inline int check(int dummy, ...) { return dummy; }

#ifdef __cplusplus
}
#endif
