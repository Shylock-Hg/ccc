/*! \brief
 *  \author Shylock Hg
 *  \date 2019-04-26
 *  \email tcath2s@gmail.com
 * */

#include <ccc/utils/panic.h>
#include <ccc/utils/testing.h>

int main(int argc, char* argv[]) {
    //    panic();
    TESTING_CASE("Panic empty input");
    TESTING_REPORT();
    return TESTING_FAILED();
}
