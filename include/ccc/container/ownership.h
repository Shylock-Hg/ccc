/*! \brief
 *  \author Shylock Hg
 *  \date 2019-06-25
 *  \email tcath2s@gmail.com
 * */

#pragma once

#include <assert.h>
#include <stddef.h>

/// Just the hint, no mechanism in fact

/// Remember to aquire and release resource
#define __OWNER
/// You are just borrower, don't mind resource aquire or release
#define __BORROWER

#ifdef __cplusplus
extern "C" {
#endif

/// The common owner and borrowner container

#define OWNER_INITIALIZER \
    { NULL, NULL, NULL }

#define BORROWER_INITIALIZER \
    { NULL }

typedef void (*destructor_t)(void* o);
typedef void* (*copy_t)(void* o);

typedef struct owner __OWNER {
    void* obj;
    copy_t copier;
    destructor_t destructor;
} owner_t;

typedef struct borrower __BORROWER {
    void* obj;
} borrower_t;

/// Take the ownership from raw pointer
static inline owner_t take(void** raw, copy_t copier, destructor_t destructor) {
    assert(raw != NULL);
    assert(*raw != NULL);

    owner_t o = {.obj = *raw, .copier = copier, .destructor = destructor};
    *raw = NULL;
    return o;
}

/// \brief Move ownership
static inline void move(owner_t* dest, owner_t* src) {
    assert(dest->obj == NULL);
    assert(src->obj != NULL);
    assert(src->destructor != NULL);

    dest->obj = src->obj;
    dest->copier = src->copier;
    dest->destructor = src->destructor;
    src->obj = NULL;
    src->copier = NULL;
    src->destructor = NULL;
}

/// \brief Copy ownership
static inline void copy(owner_t* dest, owner_t* src) {
    assert(dest != NULL);
    assert(dest->obj == NULL);
    assert(src->obj != NULL);
    assert(src->copier != NULL);

    dest->obj = src->copier(src->obj);
    dest->copier = src->copier;
    dest->destructor = src->destructor;
}

/// \brief release ownership
static inline void release(owner_t* o) {
    assert(o != NULL);
    assert(o->obj != NULL);

    if (NULL != o->destructor) {
        o->destructor(o->obj);
    }
}

/// Borrow the obj from owner
static inline borrower_t borrow(owner_t src) {
    assert(src.obj);

    borrower_t b = {src.obj};
    return b;
}

#ifdef __cplusplus
}
#endif
