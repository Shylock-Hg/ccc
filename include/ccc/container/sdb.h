/*! \brief
 *  \author Shylock Hg
 *  \date 2019-05-12
 *  \email tcath2s@gmail.com
 * */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "./ownership.h"

/// the stb_t meas the pointer to Simple Dynamic Buffer
/// compatiable to c-style string with the NULL ending
typedef uint8_t* __OWNER sdb_t;

/// \brief create the empty sdb_t
/// \param cap the capacity
/// \return the sdb_t
sdb_t sdb_new(const size_t cap);

/// \brief duplicate the plain buffer to sdb
/// \param buf the pointer to buffer
/// \param len the length of the buffer
/// \return Ok to pointer to sdb, Error to NULL
sdb_t sdb_dupraw(const void* buf, size_t len);

/// \brief duplicate the sdb
/// \param sdb the origin sdb poiner
/// \return Ok to pointer to new sdb, Error to NULL
sdb_t sdb_dup(const sdb_t sdb);

/// \brief duplicate the c string to sdb
/// \param str the c string
/// \return Ok to pointer to sdb, Error to NULL
sdb_t sdb_dupstr(const char* str);

/// \brief release the sdb
/// \param sdb to release
void sdb_release(sdb_t sdb);

/// \brief get the length of buffer
/// \param sdb the sdb pointer
/// \return the sdb buffer length
size_t sdb_len(sdb_t sdb);

/// \brief Concatenate the raw buffer
/// \param sdb the sdb pointer
/// \param raw the raw buffer
/// \param len the length of the buffer
/// \return sdb when ok, NULL when error
sdb_t sdb_catraw(sdb_t sdb, const uint8_t* raw, size_t len);

/// \brief Concatenate the raw string
/// \param sdb the sdb pointer
/// \param str the string
/// \return sdb when ok, NULL when error
sdb_t sdb_catstr(sdb_t sdb, const char* str);

/// \brief Concatenate the sdb
/// \param sdb the sdb pointer
/// \param a the appended sdb
/// \return sdb when ok, NULL when error
sdb_t sdb_catsdb(sdb_t sdb, const sdb_t a);

#ifdef __cplusplus
}
#endif
