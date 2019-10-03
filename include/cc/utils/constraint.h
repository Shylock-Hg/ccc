/*! \brief
 *  \author Shylock Hg
 *  \date 2019-10-03
 *  \email tcath2s@gmail.com
 * */

#pragma once

namespace cc {
namespace utils {

struct non_copyable {
   protected:
    non_copyable() = default;
    ~non_copyable() = default;

    // disable copy
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;
};

static_assert(sizeof(non_copyable) == 1UL, "Unexpected sizeof(non_copyable)!");

struct non_movable {
   protected:
    non_movable() = default;
    ~non_movable() = default;

    // disable move
    non_movable(non_movable&&) = delete;
    non_movable& operator=(non_movable&&) = delete;
};

static_assert(sizeof(non_movable) == 1UL, "Unexpected sizeof(non_movable)!");

}  // namespace utils
}  // namespace cc
