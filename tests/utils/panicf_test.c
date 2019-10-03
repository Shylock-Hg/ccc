/*! \brief
 *  \author Shylock Hg
 *  \date 2019-04-26
 *  \email tcath2s@gmail.com
 * */

#include <c/utils/panic.h>
#include <c/utils/testing.h>

int main(int argc, char* argv[]) {
    //    panic("Oh My God!--%u\n", 233);
    TESTING_CASE("Panic format message");
    TESTING_REPORT();
    return TESTING_FAILED();
}
