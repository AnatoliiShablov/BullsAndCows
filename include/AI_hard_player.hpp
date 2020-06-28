#ifndef AI_HARD_PLAYER_HPP
#define AI_HARD_PLAYER_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "player.hpp"

class AI_hard_player: public player {
public:
    AI_hard_player(size_t alphabet_size, size_t word_length);

    AI_hard_player();

    [[nodiscard]] std::string get_variant();

    void answer(bulls_cows const& answer);

private:
    [[nodiscard]] std::string_view get_value() const noexcept;
    std::string value_;
    std::vector<std::string> all;
    std::vector<std::string> current_variants;
    std::string last_asked;
};

#endif  // AI_HARD_PLAYER_HPP
