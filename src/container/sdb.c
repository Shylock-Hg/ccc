/*! \brief
 *  \author Shylock Hg
 *  \date 2019-05-12
 *  \email tcath2s@gmail.com
 * */

#include <limits.h>

#include <c/container/sdb.h>
#include <c/utils/compiletime.h>
#include <c/utils/panic.h>

#define SDB_TYPE_5 0U
#define SDB_TYPE_8 1U
#define SDB_TYPE_16 2U
#define SDB_TYPE_32 3U
#define SDB_TYPE_64 4U

#define SDB_FLAGS_TYPE_MASK 0x07
#define SDB_FLAGS_TYPE_OFFSET 0
#define SDB_FLAGS_TYPE(sdb) ((*(sdb - 1)) & SDB_FLAGS_TYPE_MASK)

#define SDB_FLAGS_UNUSED_MASK 0xF8
#define SDB_FLAGS_UNUSED_OFFSET 3
#define SDB_FLAGS_UNUSED(sdb) \
    (((*(sdb - 1)) & SDB_FLAGS_UNUSED_MASK) >> SDB_FLAGS_UNUSED_OFFSET)

#define SDB_FLAGS_SET_TYPE(sdb, type)                          \
    do {                                                       \
        *(sdb - 1) = (((*(sdb - 1)) & SDB_FLAGS_UNUSED_MASK) | \
                      (SDB_FLAGS_TYPE_MASK & type));           \
    } while (0);
#define SDB_FLAGS_SET_UNUSED(sdb, unused)                                  \
    do {                                                                   \
        *(sdb - 1) =                                                       \
            ((SDB_FLAGS_TYPE(sdb)) | (unused << SDB_FLAGS_UNUSED_OFFSET)); \
    } while (0);

// typedef struct sdb_flags {
//    uint8_t type : 3;  /// Type
//    uint8_t len : 5;   /// Length without NULL ending and same as capcity
//} sdb_flags_t;

// static_assert(1 == sizeof(sdb_flags_t), "Unexpected sizeof(sdb_flags_t)");

/// \note The sdb is capcitable to c string, but consider it as sdb first

/// The sdb5
/// \field type the struct type
/// \field len the length of buffer same as capacity
/// \field buf the pointer to buffer data
typedef struct sdb5 {
    uint8_t type : 3;  /// Type
    uint8_t len : 5;   /// Length without NULL ending and same as capcity
    uint8_t buf[];     /// pointer to data memory
} sdb5_t;

static_assert(1 == sizeof(sdb5_t), "Unexpected sizeof(sdb5_t)");

/// The sdbX_t definition
/// \field len the length without NULL ending
/// \field cap the capcity of sdb_t data buffer
/// \field type the struct type
/// \field unused
/// \field buf the pointer to data memory
/// \note cap = len + 1 + free
#define sdbX(n)                    \
    typedef struct PACKED sdb##n { \
        uint##n##_t len;           \
        uint##n##_t cap;           \
        uint8_t type : 3;          \
        uint8_t unused : 5;        \
        uint8_t buf[];             \
    } sdb##n##_t;

#ifdef _MSC_VER
#pragma pack(1)
#endif  // _MSC_VER
sdbX(8);
sdbX(16);
sdbX(32);
sdbX(64);
#ifdef _MSC_VER
#pragma pack()
#endif  // _MSC_VER

static_assert(2 * 1 + 1 == sizeof(sdb8_t), "Unexpected sizeof(sdb8_t)");
static_assert(2 * 2 + 1 == sizeof(sdb16_t), "Unexpected sizeof(sdb16_t)");
static_assert(2 * 4 + 1 == sizeof(sdb32_t), "Unexpected sizeof(sdb32_t)");
static_assert(2 * 8 + 1 == sizeof(sdb64_t), "Unexpected sizeof(sdb64_t)");

/// \brief get the pointer to sdb header
/// \param sdb the sdb pointer
/// \param X the sdb variant
#define SDB_HDR(sdb, X) ((sdb##X##_t*)((sdb) - sizeof(sdb##X##_t)))

/// \brief get sdb type from capcity
/// \param cap capacity
/// \return sdb type
static inline uint8_t sdb_cap2type(const size_t cap) {
    const uint8_t UINT5_MAX = 0x1F;
    if (cap > UINT64_MAX) {
        panic("Too large capacity of sdb!\n");
    } else if (cap > UINT32_MAX) {
        return SDB_TYPE_64;
    } else if (cap > UINT16_MAX) {
        return SDB_TYPE_32;
    } else if (cap > UINT8_MAX) {
        return SDB_TYPE_16;
    } else if (cap > UINT5_MAX) {
        return SDB_TYPE_8;
    }
    return SDB_TYPE_5;
}

/// \brief get hdrlen from type id
/// \param type the type id
/// \return  the sdb header length
static inline size_t sdb_type2hdrlen(const uint8_t type) {
    switch (type) {
        case SDB_TYPE_5:
            return sizeof(sdb5_t);
        case SDB_TYPE_8:
            return sizeof(sdb8_t);
        case SDB_TYPE_16:
            return sizeof(sdb16_t);
        case SDB_TYPE_32:
            return sizeof(sdb32_t);
        case SDB_TYPE_64:
            return sizeof(sdb64_t);
        default:
            panic("Invalid sdb type - %u !", type);
    }
}

/// \brief get the header size
/// \param sdb the sdb pointer
/// \return the sbb header size
// static inline size_t sdb_hdrlen(sdb_t sdb) {
static inline size_t sdb_hdrlen(sdb_t sdb) {
    switch (SDB_FLAGS_TYPE(sdb)) {
        case SDB_TYPE_5:
            return sizeof(sdb5_t);
        case SDB_TYPE_8:
            return sizeof(sdb8_t);
        case SDB_TYPE_16:
            return sizeof(sdb16_t);
        case SDB_TYPE_32:
            return sizeof(sdb32_t);
        case SDB_TYPE_64:
            return sizeof(sdb64_t);
        default:
            panic("Invalid sdb type!\n");
            break;
    }
}

/// \brief get the capcity of sdb buffer
/// \param sdb the sdb pointer
/// \return the sdb buffer capcity
static inline size_t sdb_cap(sdb_t sdb) {
    uint8_t type = SDB_FLAGS_TYPE(sdb);
    switch (type) {
        case SDB_TYPE_5: {
            return SDB_FLAGS_UNUSED(sdb);
        }

        case SDB_TYPE_8: {
            return SDB_HDR(sdb, 8)->cap;
        }

        case SDB_TYPE_16: {
            return SDB_HDR(sdb, 16)->cap;
        }

        case SDB_TYPE_32: {
            return SDB_HDR(sdb, 32)->cap;
        }

        case SDB_TYPE_64: {
            return SDB_HDR(sdb, 64)->cap;
        }

        default:
            panic("Invalid sdb type - %u !\n", type);
            break;
    }
}

/// \brief set sdb new capcity
/// \param sdb the pointer to sdb
/// \param newcap the new capcity of sdb buffer
static inline void sdb_set_cap(sdb_t sdb, size_t newcap) {
    uint8_t type = SDB_FLAGS_TYPE(sdb);
    switch (type) {
        case SDB_TYPE_5: {
            SDB_FLAGS_SET_UNUSED(sdb, newcap);
        } break;

        case SDB_TYPE_8: {
            SDB_HDR(sdb, 8)->cap = newcap;
        } break;

        case SDB_TYPE_16: {
            SDB_HDR(sdb, 16)->cap = newcap;
        } break;

        case SDB_TYPE_32: {
            SDB_HDR(sdb, 32)->cap = newcap;
        } break;

        case SDB_TYPE_64: {
            SDB_HDR(sdb, 64)->cap = newcap;
        } break;

        default:
            panic("Invalid sdb type - %u !\n", type);
            break;
    }
}

/// \brief get the length of buffer
/// \param sdb the sdb pointer
/// \return the sdb buffer length
size_t sdb_len(sdb_t sdb) {
    uint8_t type = SDB_FLAGS_TYPE(sdb);
    switch (type) {
        case SDB_TYPE_5: {
            return SDB_FLAGS_UNUSED(sdb);
        }

        case SDB_TYPE_8: {
            return SDB_HDR(sdb, 8)->len;
        }

        case SDB_TYPE_16: {
            return SDB_HDR(sdb, 16)->len;
        }

        case SDB_TYPE_32: {
            return SDB_HDR(sdb, 32)->len;
        }

        case SDB_TYPE_64: {
            return SDB_HDR(sdb, 16)->len;
        }

        default:
            panic("Invalid sdb type - %u !\n", type);
            break;
    }
}

/// \brief set sdb length
/// \param sdb the sdb pointer
/// \param newlen the new length
static inline void sdb_set_len(sdb_t sdb, size_t newlen) {
    uint8_t type = SDB_FLAGS_TYPE(sdb);
    switch (type) {
        case SDB_TYPE_5: {
            SDB_FLAGS_SET_UNUSED(sdb, newlen);
        } break;

        case SDB_TYPE_8: {
            SDB_HDR(sdb, 8)->len = newlen;
        } break;

        case SDB_TYPE_16: {
            SDB_HDR(sdb, 16)->len = newlen;
        } break;

        case SDB_TYPE_32: {
            SDB_HDR(sdb, 32)->len = newlen;
        } break;

        case SDB_TYPE_64: {
            SDB_HDR(sdb, 64)->len = newlen;
        } break;

        default:
            panic("Invalid sdb type - %u !\n", type);
            break;
    }
}

/// \brief create the empty sdb_t
/// \param cap the capacity
/// \return the sdb_t
sdb_t sdb_new(const size_t cap) {
#define SDB_INIT(sdb, sdbX, cap)                                  \
    do {                                                          \
        sdb##sdbX##_t* hdr = malloc(sizeof(sdb##sdbX##_t) + cap); \
        if (NULL == hdr) {                                        \
            return NULL;                                          \
        }                                                         \
        hdr->type = SDB_TYPE_##sdbX;                              \
        hdr->len = 0;                                             \
        hdr->cap = cap;                                           \
        sdb = hdr->buf;                                           \
    } while (0);

    uint8_t type = sdb_cap2type(cap);
    sdb_t sdb = NULL;
    switch (type) {
        case SDB_TYPE_5: {
            sdb5_t* hdr = malloc(sizeof(sdb5_t) + cap);
            if (NULL == hdr) {
                return NULL;
            }
            hdr->type = SDB_TYPE_5;
            hdr->len = cap;
            sdb = hdr->buf;
        } break;

        case SDB_TYPE_8: {
            SDB_INIT(sdb, 8, cap);
        } break;
        case SDB_TYPE_16: {
            SDB_INIT(sdb, 16, cap);
        } break;
        case SDB_TYPE_32: {
            SDB_INIT(sdb, 32, cap);
        } break;

        case SDB_TYPE_64: {
            SDB_INIT(sdb, 64, cap);
        } break;

        default:
            panic("Invalid sdb type - %u !\n", type);
            break;
    }
    return sdb;

#undef SDB_INIT
}

/// \brief duplicate the plain buffer to sdb
/// \param buf the pointer to buffer
/// \param len the length of the buffer
/// \return Ok to pointer to sdb, Error to NULL
sdb_t sdb_dupraw(const void* buf, size_t len) {
    sdb_t sdb = sdb_new(len + 1);
    if (NULL == sdb) {
        return NULL;
    }
    memcpy(sdb, buf, len);
    sdb_set_len(sdb, len);
    sdb[len + 1] = '\0';
    return sdb;
}

/// \brief duplicate the sdb
/// \param sdb the origin sdb poiner
/// \return Ok to pointer to new sdb, Error to NULL
sdb_t sdb_dup(const sdb_t sdb) {
    size_t len = sdb_len(sdb);
    return sdb_dupraw(sdb, len);
}

/// \brief duplicate the c string to sdb
/// \param str the c string
/// \return Ok to pointer to sdb, Error to NULL
sdb_t sdb_dupstr(const char* str) {
    size_t len = strlen(str);
    return sdb_dupraw(str, len);
}

/// \brief release the sdb
/// \param sdb to release
void sdb_release(sdb_t sdb) {
    if (sdb == NULL) {
        return;
    }
    size_t hdrlen = sdb_hdrlen(sdb);
    free(sdb - hdrlen);
}

/// \brief resize the sdb
/// \param sdb the sdb pointer
/// \param newcap the new capcity of buffer
/// \return Ok return new sdb pointer, error return NULL
sdb_t sdb_recap(sdb_t sdb, size_t newcap) {
    uint8_t type = SDB_FLAGS_TYPE(sdb);
    size_t len = sdb_len(sdb);
    size_t hdrlen = sdb_hdrlen(sdb);
    void* sh = sdb - hdrlen;

    sdb_t newsdb = NULL;
    if (newcap <= len) {
        return NULL;  // not allowed
    }
    uint8_t newtype = sdb_cap2type(newcap);
    if (newtype == SDB_TYPE_5) {
        // Will cause inconsistent(Length VS Capcity) if stay sdb5_t
        newtype = SDB_TYPE_8;
    }
    size_t newhdrlen = sdb_type2hdrlen(newtype);
    if (newtype == type) {
        void* newsh = realloc(sh, newcap + hdrlen);
        if (newsh == NULL) {
            return NULL;
        }
        newsdb = (sdb_t)newsh + hdrlen;
    } else {
        // Need to move data since header resize
        sdb_release(sdb);
        void* newsh = malloc(newhdrlen + newcap);
        if (newsh == NULL) {
            return NULL;
        }
        newsdb = (sdb_t)newsh + newhdrlen;
        SDB_FLAGS_SET_TYPE(newsdb, newtype);
        sdb_set_len(newsdb, len);
        memcpy(newsdb, sdb, len);
        newsdb[len] = '\0';
    }
    sdb_set_cap(newsdb, newcap);
    return newsdb;
}

/// \brief increase the capcity of sdb
/// \param sdb the sdb pointer
/// \param inc the length increased
/// \return sdb for Ok, NULL for error
sdb_t sdb_inccap(sdb_t sdb, size_t inc) {
    assert(sdb);
    assert(inc);
    return sdb_recap(sdb, sdb_cap(sdb) + inc);
}

/// \brief Concatenate the raw buffer
/// \param sdb the sdb pointer
/// \param raw the raw buffer
/// \param len the length of the buffer
/// \return sdb when ok, NULL when error
sdb_t sdb_catraw(sdb_t sdb, const uint8_t* raw, size_t len) {
    assert(raw);
    assert(len);
    size_t sdblen = sdb_len(sdb);
    size_t sdbcap = sdb_cap(sdb);
    sdb_t newsdb = NULL;
    if (sdbcap > sdblen + len) {
        newsdb = sdb;
    } else {
        // need resize
        newsdb = sdb_recap(sdb, sdbcap + len * 2);
        if (newsdb == NULL) {
            return NULL;
        }
    }
    sdb_set_len(newsdb, sdblen + len);
    memcpy(newsdb + sdblen, raw, len);
    newsdb[sdblen + len] = '\0';
    return newsdb;
}

/// \brief Concatenate the raw string
/// \param sdb the sdb pointer
/// \param str the string
/// \return sdb when ok, NULL when error
sdb_t sdb_catstr(sdb_t sdb, const char* str) {
    assert(sdb);
    assert(str);
    size_t len = strlen(str);
    return sdb_catraw(sdb, (uint8_t*)str, len);
}

/// \brief Concatenate the sdb
/// \param sdb the sdb pointer
/// \param a the appended sdb
/// \return sdb when ok, NULL when error
sdb_t sdb_catsdb(sdb_t sdb, const sdb_t a) {
    assert(sdb);
    assert(a);
    size_t len = sdb_len(a);
    return sdb_catraw(sdb, (uint8_t*)a, len);
}
