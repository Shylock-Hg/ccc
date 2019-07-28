/*! \brief
 *  \author Shylock Hg
 *  \date 2019-06-29
 *  \email tcath2s@gmail.com
 * */

#pragma once

#include <stddef.h>

/// Get the derived object from base pointer
#define DERIVED(base_ptr, derived_type, field)        \
    ((derived_type*)((const volatile char*)base_ptr - \
                     offsetof(derived_type, field)))

/// Construct the derived type and reture the base pointer
#define DERIVED_NEW(base_type, base_field, derived_type, derived_constructor, \
                    ...)                                                      \
    ({                                                                        \
        derived_type* d = derived_constructor(__VA_ARGS__);                   \
        (base_type*)(&(d->base_field));                                       \
    })
