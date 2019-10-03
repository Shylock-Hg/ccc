#include <gtest/gtest.h>

#include <cc/utils/scoped_invoker.h>

namespace helper {
void set(int* a, int b) { *a = b; }

class sample {
   public:
    void set(int b) { a_ = b; }
    bool eq(int b) { return a_ == b; }

   private:
    int a_;
};
}  // namespace helper

TEST(scoped_invoker, simple) {
    int b = 0;
    int a = -1;
    { cc::utils::scoped_invoker<int*, int> c(helper::set, &b, a); }
    ASSERT_EQ(b, a);

    int d = -2;
    helper::sample s;
    {
        cc::utils::scoped_invoker<helper::sample*, int> c(&helper::sample::set,
                                                          &s, d);
    }
    ASSERT_TRUE(s.eq(d));
}
