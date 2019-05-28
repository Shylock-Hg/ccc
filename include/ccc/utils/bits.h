///
///

#pragma once

/// \brief Get the one bit seted number
/// \param x the position to set
#define BIT(x) (1U << (x))

/// \brief set on bit in word
#define BIT_SET(word, x) \
    do {                 \
        word |= BIT(x);  \
    } while (0);

/// \brief clear one bit in word
#define BIT_CLEAR(word, x) \
    do {                   \
        word &= (~BIT(x)); \
    } while (0);

/// \brief modify the bits
/// \param clear clear mask to clear bits which setted in mask
/// \param set set mask to set bits which setted in mask
#define BITS_MODIFY(word, clear, set)     \
    do {                                  \
        word = ((word & (~clear)) | set); \
    } while (0);

/// \brief evaluate the value of substitute word
/// \param word the bits
/// \param mask the value mask
/// \param offset the value offset in word
#define BITS_EVAL(word, mask, offset) (((word) & (mask)) >> (offset))
