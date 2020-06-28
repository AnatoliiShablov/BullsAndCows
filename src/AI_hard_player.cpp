#include "AI_hard_player.hpp"

#include <random>

namespace {
void fill_with_recursion(std::vector<bool>& used, std::string& current_string,
                         std::string_view alphabet, size_t length_left,
                         std::vector<std::string>& all) {
    if (length_left == 0) {
        all.push_back(current_string);
    }
    for (size_t ch = 0; ch < used.size(); ++ch) {
        if (used[ch]) {
            continue;
        }
        used[ch] = true;
        fill_with_recursion(used, current_string += alphabet[ch], alphabet, length_left - 1, all);
        current_string.pop_back();
        used[ch] = false;
    }
}
}  // namespace

AI_hard_player::AI_hard_player(size_t alphabet_size, size_t word_length)
    : player{alphabet_size, word_length}
    , value_{std::string("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").substr(0, alphabet_size)}
    , last_asked{} {
    std::vector<bool> used(alphabet_size, false);
    std::string tmp{};
    tmp.reserve(word_length + 1);
    fill_with_recursion(used, tmp, value_, word_length, all);
    current_variants = all;
    std::shuffle(value_.begin(), value_.end(), std::mt19937());
    value_.erase(word_length);
}

AI_hard_player::AI_hard_player() : AI_hard_player{10, 4} {}

std::string AI_hard_player::get_variant() {
    if (all.size() == current_variants.size()) {
        last_asked = all.front();
    } else if (current_variants.size() == 1) {
        last_asked = current_variants.front();
    } else if (current_variants.size() == 0) {
        last_asked = "error";
    } else {
        size_t min = current_variants.size() + 1;
        for (std::string const& rhs : all) {
            std::vector<std::vector<size_t>> counter(
                all.front().length() + 1, std::vector<size_t>(all.front().length() + 1, 0));
            size_t tmp_max = 0;
            for (std::string const& lhs : current_variants) {
                helper.get_match(lhs, rhs);
                if (++counter[helper.bulls()][helper.cows()] > tmp_max) {
                    tmp_max = counter[helper.bulls()][helper.cows()];
                    if (tmp_max >= min) {
                        break;
                    }
                }
            }
            if (tmp_max < min) {
                min = tmp_max;
                last_asked = rhs;
            }
        }
    }
    return last_asked;
}

void AI_hard_player::answer(const bulls_cows& answer) {
    std::vector<std::string> new_variants;
    for (std::string const& lhs : current_variants) {
        helper.get_match(lhs, last_asked);
        if (helper == answer) {
            new_variants.push_back(lhs);
        }
    }
    current_variants = new_variants;
}

std::string_view AI_hard_player::get_value() const noexcept {
    return value_;
}
