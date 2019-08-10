/*! \brief
 *  \author Shylock Hg
 *  \date 2019-06-29
 *  \email tcath2s@gmail.com
 * */

#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "./compiletime.h"

/// Get the derived object from base pointer
#define DERIVED(base_ptr, derived_type, field)        \
    ((derived_type*)((const volatile char*)base_ptr - \
                     offsetof(derived_type, field)))

/// Construct the derived type and reture the base pointer
#define DERIVED_NEW(base_type, field, derived_type, derived_constructor, ...) \
    ({                                                                        \
        derived_type* d = derived_constructor(__VA_ARGS__);                   \
        &(d->field);                                                          \
    })

#define MEMBER(base_ptr, derived_type, field, member)                  \
    ({                                                                 \
        derived_type* drived = DERIVED(base_ptr, derived_type, field); \
        &(drived->member);                                             \
    })

/// Traits

/// Chained Trait
/// The trait for all chained structure such as list, tree, graph etc.
typedef struct chained {
    struct chained* to;
} chained_t;

static inline chained_t* chained_to(const chained_t* c) {
    assert(c != NULL);
    return c->to;
}

static inline void chained_set(chained_t* c, chained_t* to) {
    assert(c != NULL);
    c->to = to;
}

#define CHAINED_DERIVED(base_ptr, derived_type, field) \
    DERIVED(baset_ptr, derived_type, field)
#define CHAINED_NEW(field, derived_type, derived_constructor, ...)   \
    DERIVED_NEW(chained_t, field, derived_type, derived_constructor, \
                ##__VA_ARGS__)

/// Primitive Trait
typedef union primitive {
    uint8_t u8;
    int8_t i8;
    uint16_t u16;
    int16_t i16;
    uint32_t u32;
    int32_t i32;
    uint64_t u64;
    int64_t i64;
    float f;
    double d;
    void* p;
} primitive_t;
static_assert(sizeof(primitive_t) == sizeof(double),
              "Unexpected sizeof(primitive_t)");
static_assert(sizeof(primitive_t) == 8, "Unexpected sizeof(primitive_t)");
