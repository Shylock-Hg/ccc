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
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

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

/// !! New proposal
/// Single-Linked List
/// struct {
///     chained_t* forward[2];  // head, tail
///     size_t len;
/// }

/// Single-Linked List Node
/// struct {
///     chained_t next;
///     ...
/// }

/// forward[HEAD] --> next0 --> next1 --> ... nextn  <- forward[TAIL]

/// Double-Linked List Node
/// struct {
///     chained_t next;
///     chained_t prev;
///     ...
/// }

/// Double-Linked List
/// struct {
///     chained_t* forward[2];  // head, tail
///     chained_t* backward[2];  // head, tail
///     size_t len;
/// }

/// forward[HEAD] -> next0 -> next1 -> ... nextn <- forward[TAIL]
/// backward[HEAD] -> prev0 <- prev1 <- ... prevn <- backward[TAIL]

typedef enum { HEAD, TAIL, LIST_HANDLES } list_handle;
static_assert(LIST_HANDLES == 2, "Unexpected list handles!");

typedef struct slist {
    chained_t* forward[2];  // head, tail
    size_t len;
} slist_t;

typedef struct dlist {
    chained_t* forward[2];   // head, tail
    chained_t* backward[2];  // head, tail
    size_t len;
} dlist_t;

typedef struct list_iterator {
    chained_t* current;
} list_iterator_t;

typedef struct list_const_iterator {
    const chained_t* current;
} list_const_iterator_t;

/// [begin, end)
//#define LIST_BEGIN(l) (l->forward[HEAD]->to)
#define LIST_BEGIN(l) (chained_to(l->forward[HEAD]))
#define LIST_END(l) (l->forward[TAIL])
#define LIST_SIZE(l) (l->len)
#define LIST_HEAD(l) (l->forward[HEAD])
#define LIST_TAIL(l) (l->forward[TAIL])
#define LIST_BACKWARD_HEAD(l) (l->backward[HEAD])
#define LIST_BACKWARD_TAIL(l) (l->backward[TAIL])

#define LIST_ENTRY_NEXT(e) (chained_to(e))
#define LIST_ENTRY_PREV(e) (chained_to(e))

#define LIST_ITER_CURRENT(i) (i->current)

#define LIST_ENTRY_INIT(entry, to) \
    do {                           \
        chained_set(entry, to);    \
    } while (0);

#define SLIST_ENTRY_INIT(entry, to) \
    do {                            \
        chained_set(entry, to);     \
    } while (0);

#define DLIST_ENTRY_INIT(entry_next, next, entry_prev, prev) \
    do {                                                     \
        chained_set(entry_next, next);                       \
        chained_set(entry_prev, prev);                       \
    } while (0);

/// Delare the single linked list node type
/// Such as DEL_SLIST_NODE(node, int a; char c;) will declare a structrue named
/// node_t which derived from single-linked list entry
#define DECL_SLIST_NODE(type, ...) \
    typedef struct type {          \
        chained_t next;            \
        __VA_ARGS__;               \
    } type##_t

#define DECL_DLIST_NODE(type, ...) \
    typedef struct type {          \
        chained_t next;            \
        chained_t prev;            \
        __VA_ARGS__;               \
    } type##_t

/// Create the list single-linked list node
/// \param node_type the type of the node structure
/// \param node_constructor the node constructor
/// \param args of constrcutor
#define SLIST_NODE_NEW(node_type, node_constructor, ...) \
    CHAINED_NEW(next, node_type, node_constructor, ##__VA_ARGS__)
/// Release the single-linked list node
/// \param node_destructor the destructor of the node structure
/// \param obj the pointer to node structure
#define SLIST_NODE_RELEASE(node_destructor, obj) \
    do {                                         \
        node_destructor(obj);                    \
    } while (0);
#define DLIST_NODE_NEW(node_type, node_constructor, ...) \
    CHAINED_NEW(next, node_type, node_constructor, ##__VA_ARGS__)
/// Get prev pointer from next pointer
#define DLIST_NEXT_PREV(p, node_type) MEMBER(p, node_type, next, prev)
#define DLIST_NODE_RELEASE(node_destructor, obj) \
    SLIST_NODE_RELEASE(node_destructor, obj)

/// Very evil !!
/// Need assume the dlist `prev` just follow the `next`
static inline chained_t* dlist_next_prev(chained_t* next) {
    assert(next != NULL);
    return next + 1;
}
static inline chained_t* dlist_prev_next(chained_t* prev) {
    assert(prev != NULL);
    return prev - 1;
}

/// Create A list iterator from single-linked list
static inline list_iterator_t* slist_iterator_new(const slist_t* l) {
    assert(l != NULL);
    list_iterator_t* lit = malloc(sizeof(list_iterator_t));
    if (lit == NULL) {
        return lit;
    }
    LIST_ITER_CURRENT(lit) = LIST_BEGIN(l);
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
        LIST_ITER_CURRENT(lit) = LIST_HEAD(l);  // Forward iterate
    } else if (BACKWARD == direction) {
        LIST_ITER_CURRENT(lit) = LIST_TAIL(l);  // Backward iterate
    } else {
        panic("Invalid list iterator direction %d!\n", direction);
    }
}

/// Release the list iterator
static inline void list_iterator_release(list_iterator_t* i) {
    assert(i != NULL);
    free(i);
}

#define LIST_ITERATOR_INIT(i, l)              \
    do {                                      \
        LIST_ITER_CURRENT(i) = LIST_BEGIN(l); \
    } while (0);

#define LIST_ITERATOR_VALID(i, l) (LIST_ITER_CURRENT(i) != LIST_END(l))

static inline void list_iterator_next(list_iterator_t* i) {
    assert(i != NULL);
    LIST_ITER_CURRENT(i) = chained_to(i->current);
}

/// Iterate the list
/// ! Don't modify the iterator itself
/// Choose your start
#define foreach(iterator, list) \
    for (; LIST_ITERATOR_VALID(iterator, list); list_iterator_next(iterator))

/// Iterate the list from head
#define list_foreach(iterator, list)                                       \
    for (LIST_ITER_CURRENT(iterator); LIST_ITERATOR_VALID(iterator, list); \
         list_iterator_next(iterator))

/// Declare a dummy slist node
DECL_SLIST_NODE(sdummy, const char* desc);
static inline sdummy_t* sdummy_new(void) {
    static const char* a = "Dummy slist node!";
    sdummy_t* sdummy_node = malloc(sizeof(sdummy_t));
    sdummy_node->desc = a;
    SLIST_ENTRY_INIT(&(sdummy_node->next), NULL);
    return sdummy_node;
}
static inline sdummy_t* sdummy_new_c(chained_t* next) {
    static const char* a = "Dummy slist node!";
    sdummy_t* sdummy_node = malloc(sizeof(sdummy_t));
    sdummy_node->desc = a;
    SLIST_ENTRY_INIT(&(sdummy_node->next), next);
    return sdummy_node;
}
static inline void dummy_release(void* dummy) { free(dummy); }

/// Create New single linked list from
/// Make sure the head and tail are dummy node !
static inline slist_t* slist_new_c(chained_t* head, chained_t* tail,
                                   size_t len) {
    slist_t* s = malloc(sizeof(slist_t));
    assert(s != NULL);
    s->forward[HEAD] = head;
    s->forward[TAIL] = tail;
    LIST_SIZE(s) = len;
    return s;
}

/// Create New single linked list
static inline slist_t* slist_new(void) {
    // Mock dummy nodes
    chained_t* dummy_tail = SLIST_NODE_NEW(sdummy_t, sdummy_new);
    chained_t* dummy_head = SLIST_NODE_NEW(sdummy_t, sdummy_new_c, dummy_tail);

    return slist_new_c(dummy_head, dummy_tail, 0UL);
}

/// Declare a dummy slist node
DECL_DLIST_NODE(ddummy, const char* desc);
static inline ddummy_t* ddummy_new(void) {
    static const char* a = "Dummy dlist node!";
    ddummy_t* ddummy_node = malloc(sizeof(ddummy_t));
    ddummy_node->desc = a;
    DLIST_ENTRY_INIT(&(ddummy_node->next), NULL, &(ddummy_node->prev), NULL);
    return ddummy_node;
}
static inline ddummy_t* ddummy_new_c(chained_t* next, chained_t* prev) {
    static const char* a = "Dummy dlist node!";
    ddummy_t* ddummy_node = malloc(sizeof(ddummy_t));
    ddummy_node->desc = a;
    DLIST_ENTRY_INIT(&(ddummy_node->next), next, &(ddummy_node->prev), prev);
    return ddummy_node;
}
static inline void ddummy_release(ddummy_t* d) {
    assert(d != NULL);
    free(d);
}

/// Create New double linked list from
/// Make sure the head and tail are dummy
static inline dlist_t* dlist_new_c(chained_t* head_next, chained_t* head_prev,
                                   chained_t* tail_next, chained_t* tail_prev,
                                   size_t len) {
    dlist_t* d = malloc(sizeof(dlist_t));
    assert(d != NULL);
    d->forward[HEAD] = head_next;
    d->forward[TAIL] = tail_next;
    d->backward[HEAD] = head_prev;
    d->backward[TAIL] = tail_prev;
    LIST_SIZE(d) = len;
    return d;
}

// static inline slist_entry_t* forward_de2se(dlist_entry_t* d) __BORROWER;
// static inline slist_entry_t* backward_de2se(dlist_entry_t* d) __BORROWER;
/// Create New double linked list
static inline dlist_t* dlist_new(void) {
    chained_t* dummy_head_next = DLIST_NODE_NEW(ddummy_t, ddummy_new);
    chained_t* dummy_head_prev = DLIST_NEXT_PREV(dummy_head_next, ddummy_t);
    chained_t* dummy_tail_next =
        DLIST_NODE_NEW(ddummy_t, ddummy_new_c, NULL, dummy_head_prev);
    chained_t* dummy_tail_prev = DLIST_NEXT_PREV(dummy_tail_next, ddummy_t);
    DLIST_ENTRY_INIT(dummy_head_next, dummy_tail_next, dummy_head_prev, NULL);

    return dlist_new_c(dummy_head_next, dummy_head_prev, dummy_tail_next,
                       dummy_tail_prev, 0UL);
}

/*
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
*/

/// Release the single linked list
/// \param s the slist_t*
/// \param node_type the node structure type
/// \param node_destructor the node destructor
#define SLIST_RELEASE(s, node_type, node_destructor)                           \
    do {                                                                       \
        assert(s != NULL);                                                     \
        chained_t* head = LIST_HEAD(s);                                        \
        chained_t* tail = LIST_TAIL(s);                                        \
        sdummy_t* dummy_head = DERIVED(head, sdummy_t, next);                  \
        sdummy_t* dummy_tail = DERIVED(tail, sdummy_t, next);                  \
        list_iterator_t* it = slist_iterator_new(s);                           \
        assert(it != NULL);                                                    \
        while (LIST_ITERATOR_VALID(it)) {                                      \
            node_type* node = DERIVED(LIST_ITER_CURRENT(it), node_type, next); \
            list_iterator_next(it);                                            \
            node_destructor(node);                                             \
        }                                                                      \
        list_iterator_release(it);                                             \
        dummy_release(dummy_head);                                             \
        dummy_release(dummy_tail);                                             \
        free(s);                                                               \
    } while (0);

/// Release a double-lined list
/// \param d the dlist_t*
/// \param node_type the node structure type
/// \param the node desctructor
#define DLIST_RELEASE(d, node_type, node_destructor)                           \
    do {                                                                       \
        assert(d != NULL);                                                     \
        chained_t* head = LIST_HEAD(d);                                        \
        chained_t* tail = LIST_TAIL(d);                                        \
        ddummy_t* dummy_head = DERIVED(head, ddummy_t, next);                  \
        ddummy_t* dummy_tail = DERIVED(tail, ddummy_t, next);                  \
        list_iterator_t* it = dlist_iterator_new(d, FORWARD);                  \
        assert(it != NULL);                                                    \
        while (LIST_ITERATOR_VALID(it)) {                                      \
            node_type* node = DERIVED(LIST_ITER_CURRENT(it), node_type, next); \
            list_iterator_next(it);                                            \
            node_destructor(node);                                             \
        }                                                                      \
        list_iterator_release(it);                                             \
        dummy_release(dummy_head);                                             \
        dummy_release(dummy_tail);                                             \
        free(d);                                                               \
    } while (0);

static inline void slist_push_front(slist_t* l, chained_t* entry) {
    assert(l != NULL);
    assert(entry != NULL);
    chained_t* head = LIST_HEAD(l);
    chained_t* first = LIST_BEGIN(l);
    SLIST_ENTRY_INIT(entry, first);
    SLIST_ENTRY_INIT(head, entry);
    LIST_SIZE(l) += 1;
}

static inline void slist_push_back(slist_t* l, chained_t* entry) {
    assert(l != NULL);
    assert(entry != NULL);
    chained_t* tail = LIST_TAIL(l);
    chained_t* last = LIST_HEAD(l);
    chained_t* tmp = LIST_BEGIN(l);
    while (tmp != tail) {
        last = LIST_ENTRY_NEXT(last);
        tmp = LIST_ENTRY_NEXT(tmp);
    }
    SLIST_ENTRY_INIT(entry, tmp);
    SLIST_ENTRY_INIT(last, entry);
    LIST_SIZE(l) += 1;
}

static inline int slist_insert(slist_t* l, chained_t* entry, size_t i) {
    assert(l != NULL);
    assert(entry != NULL);
    if (i > LIST_SIZE(l)) {
        return -1;  // Out of range
    }
    chained_t* tail = LIST_TAIL(l);
    chained_t* tmp = LIST_HEAD(l);
    while (tmp != tail && i--) {
        tmp = LIST_ENTRY_NEXT(tmp);
    }
    chained_t* next = LIST_ENTRY_NEXT(tmp);
    SLIST_ENTRY_INIT(entry, next);
    SLIST_ENTRY_INIT(tmp, entry);
    LIST_SIZE(l) += 1;
}

static inline void dlist_push_front(dlist_t* l, chained_t* entry_next,
                                    chained_t* entry_prev) {
    assert(l != NULL);
    assert(entry_next != NULL);
    assert(entry_prev != NULL);
    chained_t* head_next = LIST_HEAD(l);
    chained_t* head_prev = LIST_BACKWARD_HEAD(l);
    chained_t* first_next = LIST_BEGIN(l);
    chained_t* first_prev = dlist_next_prev(first_next);
    DLIST_ENTRY_INIT(entry_next, first_next, entry_prev, head_prev);
    SLIST_ENTRY_INIT(head_next, entry_next);
    SLIST_ENTRY_INIT(first_prev, entry_prev);
    LIST_SIZE(l) += 1;
}

static inline void dlist_push_back(dlist_t* l, chained_t* entry_next,
                                   chained_t* entry_prev) {
    assert(l != NULL);
    assert(entry_next != NULL);
    assert(entry_prev != NULL);
    chained_t* tail_next = LIST_TAIL(l);
    chained_t* tail_prev = LIST_BACKWARD_TAIL(l);
    chained_t* last_prev = chained_to(tail_prev);
    chained_t* last_next = dlist_prev_next(last_prev);
    DLIST_ENTRY_INIT(entry_next, tail_next, entry_prev, last_prev);
    SLIST_ENTRY_INIT(tail_prev, entry_next);
    SLIST_ENTRY_INIT(last_next, entry_prev);
    LIST_SIZE(l) += 1;
}

static inline int dlist_insert(dlist_t* l, chained_t* entry_next,
                               chained_t* entry_prev, size_t i) {
    assert(l != NULL);
    assert(entry_next != NULL);
    assert(entry_prev != NULL);
    if (i > LIST_SIZE(l)) {
        return -1;  // Out of range
    }
    chained_t* tail = LIST_TAIL(l);
    chained_t* tmp = LIST_HEAD(l);
    while (tmp != tail && i--) {
        tmp = LIST_ENTRY_NEXT(tmp);
    }
    chained_t* next = LIST_ENTRY_NEXT(tmp);
    chained_t* tmp_prev = dlist_next_prev(tmp);
    chained_t* next_prev = dlist_next_prev(next);
    DLIST_ENTRY_INIT(entry_next, next, entry_prev, tmp_prev);
    SLIST_ENTRY_INIT(tmp, entry_next);
    SLIST_ENTRY_INIT(next_prev, entry_prev);
    LIST_SIZE(l) += 1;
}

#ifdef __cplusplus
}
#endif
