/*! \brief
 *  \author Shylock Hg
 *  \date 2019-04-23
 *  \email tcath2s@gmail.com
 * */

#include <assert.h>

#include <ccc/utils/argc.h>

int main(int argc, char* argv[]) {
    assert(0 == count_args());

    assert(1 == count_args(1));
    assert(1 == count_args(""));
    assert(1 == count_args('\''));

    assert(2 == count_args('\'', "Hello, World!"));

    assert(3 == count_args(-1.1, ',', ",,,"));

    // invalid call like count_args(3, ), count_args(,) and count_args(, 3) will
    // invoke compile error
    return 0;
}