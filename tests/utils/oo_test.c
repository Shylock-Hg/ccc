/*! \brief
 *  \author Shylock Hg
 *  \date 2019-06-29
 *  \email tcath2s@gmail.com
 * */

#include <ccc/utils/oo.h>
#include <ccc/utils/testing.h>

struct base {
    int c;
};

struct derived {
    struct base b;
};

int main(int argc, char* argv[]) {
    TESTING_CASE("OO");

    struct derived d;
    struct base* b = &d.b;
    TESTING_EXPECT(DERIVED(b, struct derived, b) == &d);

    TESTING_REPORT();
    return TESTING_FAILED();
}
