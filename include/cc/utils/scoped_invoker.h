/*! \brief
 *  \author Shylock Hg
 *  \date 2019-10-03
 *  \email tcath2s@gmail.com
 * */

#pragma once

#include <functional>

#include "./constraint.h"

namespace cc {
namespace utils {

template <typename... Args>
class scoped_invoker : public cc::utils::non_copyable, cc::utils::non_movable {
   public:
    using bind_type = decltype(std::bind(
        std::declval<std::function<void(Args...)>>(), std::declval<Args>()...));

    scoped_invoker(std::function<void(Args...)> f, Args... args)
        : bind_(f, std::forward<Args>(args)...) {}

    ~scoped_invoker() { bind_(); }

   private:
    bind_type bind_;
};

}  // namespace utils
}  // namespace cc
