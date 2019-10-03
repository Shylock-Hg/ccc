/*! \brief
 *  \author Shylock Hg
 *  \date 2019-04-26
 *  \email tcath2s@gmail.com
 * */

#include <c/utils/clog.h>

int main(int argc, char* argv[]) {
    LOGD("Hello World!\n");
    LOGI("Hello World!\n");
    LOGW("Hello World!\n");
    LOGE("Hello World!\n");

    LOGD("The string is %s, the number is %d, the pointer is %p!\n", "test",
         233, NULL);
    LOGI("The string is %s, the number is %d, the pointer is %p!\n", "test",
         233, NULL);
    LOGW("The string is %s, the number is %d, the pointer is %p!\n", "test",
         233, NULL);
    LOGE("The string is %s, the number is %d, the pointer is %p!\n", "test",
         233, NULL);

    return 0;
}
