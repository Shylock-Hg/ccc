/*! \brief
 *  \author Shylock Hg
 *  \date 2019-06-28
 *  \email tcath2s@gmail.com
 * */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./argc.h"
#include "./tty.h"

size_t __tested_count = 0;
size_t __failed_count = 0;
const char* __testing_case = "Testing";

#define __TESTING_FILE stderr
#define __TESTING_COLOR_FAILED                                           \
    TTY_COLOR_PREFIX TTY_COLOR_BOLD TTY_COLOR_DELIMITER TTY_COLOR_FG_RED \
        TTY_COLOR_SUFFIX
#define __TESTING_COLOR_END TTY_COLOR_RESET_NOW

#define __ON_TESTING_FAILED(fmt, ...)                            \
    do {                                                         \
        fprintf(__TESTING_FILE,                                  \
                __TESTING_COLOR_FAILED "FAILED! in %s:%d, " fmt  \
                                       "\n" __TESTING_COLOR_END, \
                __FILE__, __LINE__, ##__VA_ARGS__);              \
    } while (0);

#define TESTING_REPORT()                                                   \
    do {                                                                   \
        fprintf(__TESTING_FILE, "TESTED! %lu, PASSED! %lu, FAILED! %lu\n", \
                __tested_count, __tested_count - __failed_count,           \
                __failed_count);                                           \
    } while (0);

/// \param prod
/// \param fmt
/// \param varidic parameter
#define TESTING_EXPECT(prod, ...)                    \
    do {                                             \
        __tested_count++;                            \
        if (!(prod)) {                               \
            __failed_count++;                        \
            if (0 == strlen(argsstr(__VA_ARGS__))) { \
                __ON_TESTING_FAILED(#prod);          \
            } else {                                 \
                __ON_TESTING_FAILED(__VA_ARGS__);    \
            }                                        \
        }                                            \
    } while (0);

/// \param prod
/// \param fmt
/// \param varidic parameter
#define TESTING_ASSERT(prod, ...)                    \
    do {                                             \
        __tested_count++;                            \
        if (!(prod)) {                               \
            __failed_count++;                        \
            if (0 == strlen(argsstr(__VA_ARGS__))) { \
                __ON_TESTING_FAILED(#prod);          \
            } else {                                 \
                __ON_TESTING_FAILED(__VA_ARGS__);    \
            }                                        \
            TESTING_REPORT();                        \
            fflush(__TESTING_FILE);                  \
            abort();                                 \
        }                                            \
    } while (0);

#define TESTING_FAILED() (__failed_count)

#define TESTING_CASE(n)                                           \
    do {                                                          \
        __testing_case = n;                                       \
        fprintf(__TESTING_FILE, "Testing %s:\n", __testing_case); \
    } while (0);

#ifdef __cplusplus
}
#endif
