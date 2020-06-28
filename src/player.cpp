#include "player.hpp"

player::player(size_t alphabet_size_, size_t word_length) : helper{alphabet_size_, word_length} {}

bulls_cows player::get_match(std::string_view check) {
    return helper.get_match(get_value(), check);
}
