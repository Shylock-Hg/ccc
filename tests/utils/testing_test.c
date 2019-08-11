#include <ccc/utils/testing.h>

#define MIN(a, b) ((a < b) ? (a) : (b))

int main(int argc, char* argv[]) {
    int a = 1;
    int b = 2;
    TESTING_CASE("Testing");
    //    TESTING_ASSERT(MIN(a, b) == 2, "The min of %d, %d is %d\n", a, b, 2);
    //    TESTING_ASSERT(MIN(a, b) == 2);
    //    TESTING_ASSERT(MIN(a, b) == 2, "Some Thing Error!");
    TESTING_ASSERT(MIN(a, b) == a, "The min of %d, %d is %d\n", a, b, a);
    TESTING_ASSERT(MIN(a, b) == a);
    TESTING_ASSERT(MIN(a, b) == a, "Some Thing Error!");
    //    TESTING_EXPECT(MIN(a, b) == 2 , "The min of %d, %d is %d", a, b, 2);
    //    TESTING_EXPECT(MIN(a, b) == 2);
    //    TESTING_EXPECT(MIN(a, b) == 2, "Some Thing Error!");
    TESTING_EXPECT(MIN(a, b) == a, "The min of %d, %d is %d", a, b, a);
    TESTING_EXPECT(MIN(a, b) == a);
    TESTING_EXPECT(MIN(a, b) == a, "Some Thing Error!");
    TESTING_REPORT();
    return TESTING_FAILED();
}
