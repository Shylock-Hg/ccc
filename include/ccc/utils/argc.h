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

/// \brief count the arguments by string
/// \param args arguments string
/// \return count of arguments
size_t count_args_by_str(const char* args);

/*! \brief transfer the variadic parameters to string literal
 * */
#define argsstr(...) (#__VA_ARGS__)

#define count_args(...) \
    (check(0, ##__VA_ARGS__), count_args_by_str(argsstr(__VA_ARGS__)))

#ifdef __cplusplus
}
#endif
