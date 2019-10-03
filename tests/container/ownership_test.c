/*! \brief
 *  \author Shylock Hg
 *  \date 2019-06-25
 *  \email tcath2s@gmail.com
 * */

#include <c/container/ownership.h>
#include <c/container/sdb.h>
#include <c/utils/testing.h>

int main(int argc, char* argv[]) {
    TESTING_CASE("Ownership");
    // take ownership
    sdb_t s = sdb_new(10);
    owner_t o = take((void**)&s, (copy_t)sdb_dup, (destructor_t)sdb_release);
    TESTING_ASSERT(o.obj != NULL);
    TESTING_ASSERT(o.destructor != NULL);
    TESTING_ASSERT(s == NULL);

    // move ownership
    owner_t o2 = OWNER_INITIALIZER;
    move(&o2, &o);
    TESTING_ASSERT(o2.obj != NULL);
    TESTING_ASSERT(o2.destructor != NULL);
    TESTING_ASSERT(o.obj == NULL);
    TESTING_ASSERT(o.destructor == NULL);

    // copy ownership
    owner_t o3 = OWNER_INITIALIZER;
    copy(&o3, &o2);
    TESTING_ASSERT(o3.obj != NULL);
    TESTING_ASSERT(o3.copier != NULL);
    TESTING_ASSERT(o3.destructor != NULL);
    TESTING_ASSERT(o2.obj != NULL);
    TESTING_ASSERT(o2.copier != NULL);
    TESTING_ASSERT(o2.destructor != NULL);

    // Borrow from owner
    borrower_t b = borrow(o2);
    TESTING_ASSERT(b.obj != NULL);
    TESTING_ASSERT(o2.obj != NULL);

    // Release now
    //    sdb_release(o2.obj);
    release(&o2);
    release(&o3);
    TESTING_REPORT();
    return TESTING_FAILED();
}
