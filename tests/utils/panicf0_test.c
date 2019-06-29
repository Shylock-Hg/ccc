/*! \brief
 *  \author Shylock Hg
 *  \date 2019-04-26
 *  \email tcath2s@gmail.com
 * */

#include <ccc/utils/panic.h>
#include <ccc/utils/testing.h>

int main(int argc, char* argv[]) {
    //    panic("Oh My God!\n");
    TESTING_CASE("Panic message");
    TESTING_REPORT();
    return TESTING_FAILED();
}
