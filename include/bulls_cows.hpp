#ifndef BULLS_COWS_HPP
#define BULLS_COWS_HPP

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

class bulls_cows;

[[nodiscard]] bool operator==(bulls_cows const&, bulls_cows const&) noexcept;

class bulls_cows {
public:
    bulls_cows(size_t alphabet_size, size_t word_length);

    bulls_cows& get_match(std::string_view lhs, std::string_view rhs);
    bulls_cows& set_match(size_t bulls, size_t cows);

    [[nodiscard]] size_t bulls() const noexcept;
    [[nodiscard]] size_t cows() const noexcept;

    [[nodiscard]] bool is_correct() const noexcept;

    [[nodiscard]] bool is_full() const noexcept;

    [[nodiscard]] bool is_empty() const noexcept;

    friend bool operator==(bulls_cows const&, bulls_cows const&) noexcept;

private:
    void count_letters(std::string_view str);
    size_t bulls_;
    size_t cows_;
    size_t word_length_;
    std::vector<size_t> tmp_storage_;
};

#endif  // BULLS_COWS_HPP
