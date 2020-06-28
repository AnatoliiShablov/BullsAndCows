#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cstddef>
#include <cstdint>
#include <string_view>

#include "bulls_cows.hpp"

class player {
public:
    player(size_t alphabet_size_, size_t word_length);

    [[nodiscard]] bulls_cows get_match(std::string_view check);

    [[nodiscard]] virtual std::string get_variant() = 0;

    virtual void answer(bulls_cows const&) = 0;

    virtual ~player() = default;

protected:
    virtual constexpr std::string_view get_value() const = 0;
    bulls_cows helper;
};

#endif  // PLAYER_HPP
