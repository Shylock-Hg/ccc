/*! \brief
 *  \author Shylock Hg
 *  \date 2019-04-23
 *  \email tcath2s@gmail.com
 * */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// \brief print the location of panic
#define _print_panic_loc()                                      \
    do {                                                        \
        fprintf(stderr, "Panic in %s-%d:", __FILE__, __LINE__); \
    } while (0);

/// \brief print the message of panic
/// \param msg Is there massage in fact
/// \param the empty or (fmt, ...)
static inline void _print_panic(const int dummy, ...) {
    va_list ap;
    va_start(ap, dummy);
    // Unsafe if not a format string here!!
    const char* fmt = va_arg(ap, char*);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

/// \brief panic
/// \param empty or (fmt, ...)
#define panic(...)                          \
    do {                                    \
        _print_panic_loc();                 \
        if (0 < strlen(#__VA_ARGS__)) {     \
            _print_panic(0, ##__VA_ARGS__); \
        }                                   \
        fflush(stderr);                     \
        abort();                            \
    } while (0);

#ifdef __cplusplus
}
#endif
