/*! \brief
 *  \author Shylock Hg
 *  \date 2019-06-30
 *  \email tcath2s@gmail.com
 * */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stdbool.h>

#include "./ownership.h"
#include "./utils/oo.h"

/// 0. Continus
/// 1. Chained

/// Declare a iterator of type
#define DECL_ITERATOR(type)          \
    typedef struct type##_iterator { \
        type##_t* current;           \
    } type##_iterator_t;

#define ITERATOR_NODE_NEW(node_type, node_constructor, ...) \
    DERIVED_NEW(iterator_entry_t, node_type, node_constructor, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
