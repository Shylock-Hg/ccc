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

#if defined(unix) && unix
#include <sys/cdefs.h>
#endif

#ifndef __cplusplus
#if defined(unix) && unix
#define static_assert(expr, desc) _Static_assert(expr, desc)
#else
#define static_assert(expr, desc)
#endif
#endif

#if defined(unix) && unix
#define UNUSED __attribute__((unused))
#define UNUSED_F(v)
#else
#define UNUSED
#define UNUSED_F(v) (void)(v)
#endif

/// \brief the memory aligned size
#if defined(__GNUC__) || defined(__clang__)
#define PACKED __attribute__((__packed__))
#else
#define PACKED
#endif

/*! \brief check the validation of variadic parameters
 *  \param dummy required before variadic parameters
 *  \param ... the variadic parameters
 * */
static inline int check(int dummy, ...) { return dummy; }

#ifdef __cplusplus
}
#endif
