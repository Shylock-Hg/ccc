/*! \brief
 *  \author Shylock Hg
 *  \date 2019-05-22
 *  \email tcath2s@gmail.com
 * */

#include <ccc/utils/bits.h>
#include <ccc/utils/testing.h>

int main(int argc, char* argv[]) {
    TESTING_CASE("Bit operations");

    TESTING_ASSERT(1 == BIT(0));
    TESTING_ASSERT(2 == BIT(1));

    unsigned int c = 0x1234;
    BIT_SET(c, 6);
    TESTING_ASSERT(c == 0x1274);
    BIT_CLEAR(c, 6);
    TESTING_ASSERT(c == 0x1234);

    unsigned int b = 0x1234;
    BITS_MODIFY(b, 0x00F0, 0xF000);
    TESTING_ASSERT(b == 0xF204);

    unsigned int a = 0x1234;
    TESTING_ASSERT(BITS_EVAL(a, 0xF000, 12) == 0x1);
    TESTING_ASSERT(BITS_EVAL(a, 0x4, 0) == 0x4);
    TESTING_REPORT();
    return TESTING_FAILED();
}
