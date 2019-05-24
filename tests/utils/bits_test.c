/*! \brief
 *  \author Shylock Hg
 *  \date 2019-05-22
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>

#include <ccc/utils/bits.h>

int main(int argc, char* argv[]) {
    assert(1 == BIT(0));
    assert(2 == BIT(1));

    unsigned int c = 0x1234;
    BIT_SET(c, 6);
    assert(c == 0x1274);
    BIT_CLEAR(c, 6);
    assert(c == 0x1234);

    unsigned int b = 0x1234;
    BITS_MODIFY(b, 0x00F0, 0xF000);
    assert(b == 0xF204);

    unsigned int a = 0x1234;
    assert(BITS_EVAL(a, 0xF000, 12) == 0x1);
    assert(BITS_EVAL(a, 0x4, 0) == 0x4);
    return 0;
}
