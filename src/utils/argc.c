#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include <ccc/utils/argc.h>

/*! \brief check the validation of variadic parameters
 *  \param dummy required before variadic parameters
 *  \param ... the variadic parameters
 * */
int check(int dummy, ...) { return dummy; }

/// \brief count the arguments by string
/// \param args arguments string
/// \return count of arguments
size_t count_args_by_str(const char* args) {
    bool in_str = false;
    bool in_char = false;
    size_t count = 0;

    if (NULL == args || '\0' == *args) {
        return count;
    }

    while ('\0' != *args) {
        if ('\"' == *args && '\\' != *(args - 1)) {
            in_str = !in_str;
        }
        if ('\'' == *args && '\\' != *(args - 1)) {
            in_char = !in_char;
        }

        if (*args == ',' && !in_str && !in_char) {
            count++;
        }
        args++;
    }

    return count + 1;
}
