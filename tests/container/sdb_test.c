/*! \brief
 *  \author Shylock Hg
 *  \date 2019-05-24
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <ccc/container/sdb.h>

const char* hello = "Hello World!";

int main(int argc, char* argv[]) {
    sdb_t sdb = sdb_new(0);
    assert(sdb_len(sdb) == 0);
    sdb_t newsdb = sdb_catstr(sdb, hello);
    assert(newsdb);
    assert(sdb_len(newsdb) == strlen(hello));

    sdb_t sdb2 = sdb_dup(newsdb);
    sdb_release(newsdb);
    assert(sdb_len(sdb) == sdb_len(sdb2));
    sdb_t newsdb2 = sdb_catstr(sdb, hello);
    assert(sdb_len(newsdb2) == 2 * strlen(hello));
    sdb_release(newsdb2);

    sdb_t sdb3 = sdb_dupstr(hello);
    assert(sdb_len(sdb3) == strlen((char*)sdb3));
    assert(sdb_len(sdb3) == strlen(hello));
    sdb_t newsdb3 = sdb_catstr(sdb3, hello);
    assert(sdb_len(newsdb3) == 2 * strlen(hello));
    sdb_release(newsdb3);

    sdb_t sdb4 = sdb_dupraw(hello, strlen(hello));
    assert(sdb_len(sdb4) == strlen((char*)sdb4));
    assert(sdb_len(sdb4) == strlen(hello));
    sdb_t newsdb4 = sdb_catstr(sdb4, hello);
    assert(sdb_len(newsdb4) == 2 * strlen(hello));
    sdb_release(newsdb4);

    return 0;
}
