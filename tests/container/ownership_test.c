/*! \brief
 *  \author Shylock Hg
 *  \date 2019-06-25
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>

#include <ccc/container/ownership.h>
#include <ccc/container/sdb.h>

int main(int argc, char* argv[]) {
    // take ownership
    sdb_t s = sdb_new(10);
    owner_t o = take((void**)&s, (copy_t)sdb_dup, (destructor_t)sdb_release);
    assert(o.obj != NULL);
    assert(o.destructor != NULL);
    assert(s == NULL);

    // move ownership
    owner_t o2 = OWNER_INITIALIZER;
    move(&o2, &o);
    assert(o2.obj != NULL);
    assert(o2.destructor != NULL);
    assert(o.obj == NULL);
    assert(o.destructor == NULL);

    // copy ownership
    owner_t o3 = OWNER_INITIALIZER;
    copy(&o3, &o2);
    assert(o3.obj != NULL);
    assert(o3.copier != NULL);
    assert(o3.destructor != NULL);
    assert(o2.obj != NULL);
    assert(o2.copier != NULL);
    assert(o2.destructor != NULL);

    // Borrow from owner
    borrower_t b = borrow(o2);
    assert(b.obj != NULL);
    assert(o2.obj != NULL);

    // Release now
    //    sdb_release(o2.obj);
    release(&o2);
    release(&o3);
    return 0;
}
