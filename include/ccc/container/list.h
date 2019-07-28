/*! \brief
 *  \author Shylock Hg
 *  \date 2019-06-29
 *  \email tcath2s@gmail.com
 * */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/compiletime.h"
#include "../utils/oo.h"
#include "../utils/panic.h"
#include "./ownership.h"

typedef enum { FORWARD, BACKWARD, DIRECTIONS } list_iterator_direction_t;
static_assert(DIRECTIONS == 2, "Unexpected directions!");

/*
typedef struct slist_entry {
    struct slist_entry* next[1];
} slist_entry_t;

typedef struct dlist_entry {
    //    slist_entry_r_t* next
    slist_entry_t* next[DIRECTIONS];  // [next, prev]
} dlist_entry_t;
*/

#define DECL_LIST_ENTRY(p, n)        \
    typedef struct p##list_entry {   \
        struct slist_entry* next[n]; \
    } p##list_entry_t

DECL_LIST_ENTRY(s, 1);
DECL_LIST_ENTRY(d, 2);

#define DECL_LIST(p)           \
    typedef struct p##list {   \
        p##list_entry_t* head; \
        p##list_entry_t* tail; \
        size_t len;            \
    } p##list_t;

DECL_LIST(s);
DECL_LIST(d);

static_assert(sizeof(slist_t) == sizeof(dlist_t),
              "Unexpected sizeof(slist_t) != sizeof(dlist_t)");

typedef struct list_iterator {
    slist_entry_t* current;
} list_iterator_t;

typedef struct list_const_iterator {
    const slist_entry_t* current;
} list_const_iterator_t;

/// [begin, end)
#define LIST_BEGIN(l) (l->head->next[0])
#define LIST_END(l) (l->tail)
#define LIST_SIZE(l) (l->len)
#define LIST_HEAD(l) (l->head)
#define LIST_TAIL(l) (l->tail)

#define LIST_ENTRY_NEXT(e) (e->next[FORWARD])
#define LIST_ENTRY_PREV(e) (e->next[BACKWARD])

#define LIST_ITER_CURRENT(i) (i->current)

#define SLIST_ENTRY_INIT(entry, n) \
    do {                           \
        entry->next[0] = n;        \
    } while (0);
#define DLIST_ENTRY_INIT(entry, n, p) \
    do {                              \
        entry->next[FORWARD] = n;     \
        entry->next[BACKWARD] = p;    \
    } while (0);

/// Delare the single linked list node type
/// Such as DEL_SLIST_NODE(node, int a; char c;) will declare a structrue named
/// node_t which derived from single-linked list entry
#define DECL_SLIST_NODE(type, ...) \
    typedef struct type {          \
        slist_entry_t sentry;      \
        __VA_ARGS__;               \
    } type##_t

#define DECL_DLIST_NODE(type, ...) \
    typedef struct type {          \
        dlist_entry_t dentry;      \
        __VA_ARGS__;               \
    } type##_t

/// Create the list single-linked list node
/// \param node_type the type of the node structure
/// \param node_constructor the node constructor
/// \param args of constrcutor
#define SLIST_NODE_NEW(node_type, node_constructor, ...)            \
    DERIVED_NEW(slist_entry_t, sentry, node_type, node_constructor, \
                ##__VA_ARGS__)
/// Release the single-linked list node
/// \param node_destructor the destructor of the node structure
/// \param obj the pointer to node structure
#define SLIST_NODE_RELEASE(node_destructor, obj) \
    do {                                         \
        node_destructor(obj);                    \
    } while (0);
#define DLIST_NODE_NEW(node_type, node_constructor, ...)            \
    DERIVED_NEW(dlist_entry_t, dentry, node_type, node_constructor, \
                ##__VA_ARGS__)
#define DLIST_NODE_RELEASE(node_destructor, obj) \
    SLIST_NODE_RELEASE(node_destructor, obj)

/// Create A list iterator from single-linked list
static inline list_iterator_t* slist_iterator_new(const slist_t* l) {
    assert(l != NULL);
    list_iterator_t* lit = malloc(sizeof(list_iterator_t));
    if (lit == NULL) {
        return lit;
    }
    lit->current = LIST_BEGIN(l);
    return lit;
}

/// Create a list iterator from double-linked list
static inline list_iterator_t* dlist_iterator_new(
    const dlist_t* l, list_iterator_direction_t direction) {
    assert(l != NULL);
    list_iterator_t* lit = malloc(sizeof(list_iterator_t));
    if (lit == NULL) {
        return lit;
    }
    if (FORWARD == direction) {
        lit->current = (slist_entry_t*)(l->head->next + FORWARD);
    } else if (BACKWARD == direction) {
        lit->current = (slist_entry_t*)(l->tail->next + BACKWARD);
    } else {
        panic("Invalid list iterator direction %d!\n", direction);
    }
}

/// Release the list iterator
static inline void list_iterator_release(list_iterator_t* i) {
    assert(i != NULL);
    free(i);
}

static inline void list_iterator_init(list_iterator_t* i, const void* list) {
    assert(i != NULL);
    assert(list != NULL);
    slist_t* l = (slist_t*)list;  // Treate dlist as slist
    i->current = LIST_BEGIN(l);
}

static inline bool list_iterator_valid(const list_iterator_t* i,
                                       const void* list) {
    assert(i != NULL);
    assert(list != NULL);
    slist_t* l = (slist_t*)list;  // Treate dlist as slist
    return i->current != LIST_END(l);
}

static inline void list_iterator_next(list_iterator_t* i) {
    assert(i != NULL);
    i->current = i->current->next[0];
}

/// Iterate the list
/// ! Don't modify the iterator itself
/// Choose your start
#define foreach(iterator, list) \
    for (; list_iterator_valid(iterator, list); list_iterator_next(iterator))

/// Iterate the list from head
#define list_foreach(iterator, list)                                        \
    for (list_iterator_init(iterator); list_iterator_valid(iterator, list); \
         list_iterator_next(iterator))

/// Declare a dummy slist node
DECL_SLIST_NODE(sdummy, const char* desc);
static inline sdummy_t* sdummy_new(void) {
    static const char* a = "Dummy slist node!";
    sdummy_t* sdummy_node = malloc(sizeof(sdummy_t));
    sdummy_node->desc = a;
    sdummy_node->sentry.next[0] = NULL;
    return sdummy_node;
}
static inline void dummy_release(void* dummy) { free(dummy); }


/// Create New single linked list
static inline slist_t* slist_new(void) {
    // Mock dummy nodes
    slist_entry_t* dummy_head = SLIST_NODE_NEW(sdummy_t, sdummy_new);
    slist_entry_t* dummy_tail = SLIST_NODE_NEW(sdummy_t, sdummy_new);
    dummy_head->next[0] = dummy_tail;
    SLIST_ENTRY_INIT(dummy_head, dummy_tail);

    slist_t* s = malloc(sizeof(slist_t));
    assert(s != NULL);
    s->head = dummy_head;
    s->tail = dummy_tail;
    s->len = 0UL;
    return s;
}

/// Create New single linked list from
/// Make sure the head and tail are dummy node !
static inline slist_t* slist_new_c(slist_entry_t* head, slist_entry_t* tail,
                                   size_t len) {
    slist_t* s = malloc(sizeof(slist_t));
    assert(s != NULL);
    s->head = head;
    s->tail = tail;
    s->len = len;
    return s;
}

/// Declare a dummy slist node
DECL_DLIST_NODE(ddummy, const char* desc);
static inline ddummy_t* ddummy_new(void) {
    static const char* a = "Dummy dlist node!";
    ddummy_t* ddummy_node = malloc(sizeof(ddummy_t));
    ddummy_node->desc = a;
    ddummy_node->dentry.next[0] = NULL;
    ddummy_node->dentry.next[0] = NULL;
    return ddummy_node;
}

static inline slist_entry_t* forward_de2se(dlist_entry_t* d) __BORROWER;
static inline slist_entry_t* backward_de2se(dlist_entry_t* d) __BORROWER;
/// Create New double linked list
static inline dlist_t* dlist_new(void) {
    dlist_entry_t* dummy_head = DLIST_NODE_NEW(ddummy_t, ddummy_new);
    dlist_entry_t* dummy_tail = DLIST_NODE_NEW(ddummy_t, ddummy_new);
    DLIST_ENTRY_INIT(dummy_head, forward_de2se(dummy_tail), NULL);
    DLIST_ENTRY_INIT(dummy_tail, NULL, backward_de2se(dummy_head));

    dlist_t* d = malloc(sizeof(dlist_t));
    assert(d != NULL);
    d->head = dummy_head;
    d->tail = dummy_tail;
    d->len = 0UL;
    return d;
}

/// Create New double linked list from
/// Make sure the head and tail are dummy
static inline dlist_t* dlist_new_c(dlist_entry_t* head, dlist_entry_t* tail,
                                   size_t len) {
    dlist_t* d = malloc(sizeof(dlist_t));
    assert(d != NULL);
    d->head = head;
    d->tail = tail;
    d->len = len;
    return d;
}

static inline slist_entry_t* forward_de2se(dlist_entry_t* d) __BORROWER {
    assert(d != NULL);

    return (slist_entry_t*)(d->next + FORWARD);
}

static inline slist_entry_t* backward_de2se(dlist_entry_t* d) __BORROWER {
    assert(d != NULL);

    return (slist_entry_t*)(d->next + BACKWARD);
}

static inline slist_t* forward_d2s(dlist_t* d) __BORROWER {
    assert(d != NULL);

    return slist_new_c(forward_de2se(LIST_HEAD(d)), forward_de2se(LIST_TAIL(d)),
                       LIST_SIZE(d));
}

static inline slist_t* backward_d2s(dlist_t* d) __BORROWER {
    assert(d != NULL);
    return slist_new_c(backward_de2se(LIST_TAIL(d)),
                       backward_de2se(LIST_HEAD(d)), LIST_SIZE(d));
}

/// Transform single-linked list entry to double
/// ! Make sure it's Double-Linked List Node in fact
static inline dlist_entry_t* forward_se2de(slist_entry_t* s) __BORROWER {
    assert(s != NULL);
    return (dlist_entry_t*)(s - FORWARD);
}

static inline dlist_entry_t* backward_se2de(slist_entry_t* s) __BORROWER {
    assert(s != NULL);
    return (dlist_entry_t*)(s - BACKWARD);
}

/// Transform single-linked list to double
/// ! Make sure it's Double-Linked List in fact
static inline dlist_t* forward_s2d(slist_t* s) __BORROWER {
    assert(s != NULL);
    return dlist_new_c(forward_se2de(LIST_HEAD(s)), forward_se2de(LIST_TAIL(s)),
                       LIST_SIZE(s));
}

static inline dlist_t* backward_s2d(slist_t* s) __BORROWER {
    assert(s != NULL);
    return dlist_new_c(forward_se2de(LIST_TAIL(s)), forward_se2de(LIST_HEAD(s)),
                       LIST_SIZE(s));
}


/// Release the single linked list
/// \param s the slist_t*
/// \param node_type the node structure type
/// \param node_destructor the node destructor
#define SLIST_RELEASE(s, node_type, node_destructor)                   \
    do {                                                               \
        assert(s != NULL);                                             \
        slist_entry_t* head = LIST_HEAD(s);                            \
        slist_entry_t* tail = LIST_TAIL(s);                            \
        sdummy_t* dummy_head = DERIVED(head, sdummy_t, sentry);        \
        sdummy_t* dummy_tail = DERIVED(tail, sdummy_t, sentry);        \
        list_iterator_t* it = slist_iterator_new(s);                   \
        assert(it != NULL);                                            \
        while (list_iterator_valid(it)) {                              \
            node_type* node = DERIVED(it->current, node_type, sentry); \
            list_iterator_next(it);                                    \
            node_destructor(node);                                     \
        }                                                              \
        list_iterator_release(it);                                     \
        dummy_release(dummy_head);                                     \
        dummy_release(dummy_tail);                                     \
        free(s);                                                       \
    } while (0);


/// Release a double-lined list
/// \param d the dlist_t*
/// \param node_type the node structure type
/// \param the node desctructor
#define DLIST_RELEASE(d, node_type, node_destructor)                   \
    do {                                                               \
        assert(d != NULL);                                             \
        dlist_entry_t* head = LIST_HEAD(d);                            \
        dlist_entry_t* tail = LIST_TAIL(d);                            \
        ddummy_t* dummy_head = DERIVED(head, ddummy_t, sentry);        \
        ddummy_t* dummy_tail = DERIVED(tail, ddummy_t, sentry);        \
        list_iterator_t* it = dlist_iterator_new(d, FORWARD);          \
        assert(it != NULL);                                            \
        while (list_iterator_valid(it)) {                              \
            node_type* node = DERIVED(it->current, node_type, dentry); \
            list_iterator_next(it);                                    \
            node_destructor(node);                                     \
        }                                                              \
        list_iterator_release(it);                                     \
        dummy_release(dummy_head);                                     \
        dummy_release(dummy_tail);                                     \
        free(d);                                                       \
    } while (0);

static inline void slist_push_front(slist_t* l, slist_entry_t* entry) {
    assert(l != NULL);
    assert(entry != NULL);
    slist_entry_t* head = LIST_HEAD(l);
    slist_entry_t* first = LIST_ENTRY_NEXT(head);
    SLIST_ENTRY_INIT(entry, first);
    SLIST_ENTRY_INIT(head, entry);
    LIST_SIZE(l) += 1;
}

static inline void slist_push_back(slist_t* l, slist_entry_t* entry) {
    assert(l != NULL);
    assert(entry != NULL);
    slist_entry_t* head = LIST_HEAD(l);
    slist_entry_t* tail = LIST_TAIL(l);
    slist_entry_t* last = tail;
    slist_entry_t* tmp = LIST_ENTRY_NEXT(head);
    while (tmp != tail) {
        last = LIST_ENTRY_NEXT(last);
        tmp = LIST_ENTRY_NEXT(tmp);
    }
    SLIST_ENTRY_INIT(entry, tmp);
    SLIST_ENTRY_INIT(last, entry);
    LIST_SIZE(l) += 1;
}

#ifdef __cplusplus
}
#endif
