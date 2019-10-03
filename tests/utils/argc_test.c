/*! \brief
 *  \author Shylock Hg
 *  \date 2019-04-23
 *  \email tcath2s@gmail.com
 * */

#include <c/utils/argc.h>
#include <c/utils/testing.h>

int main(int argc, char* argv[]) {
    //    TESTING_ASSERT(0 == count_args());
    TESTING_CASE("Argc");

    TESTING_ASSERT(1 == count_args(1));
    TESTING_ASSERT(1 == count_args(""));
    TESTING_ASSERT(1 == count_args('\''));

    TESTING_ASSERT(2 == count_args('\'', "Hello, World!"));

    TESTING_ASSERT(3 == count_args(-1.1, ',', ",,,"));

    // invalid call like count_args(3, ), count_args(,) and count_args(, 3) will
    // invoke compile error
    TESTING_REPORT();
    return TESTING_FAILED();
}