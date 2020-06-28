#include "bulls_cows.hpp"

#include <cctype>
#include <iostream>

namespace {
bool correct(std::string_view str, size_t word_length, size_t alphabet_size) {
    return (str.length() == word_length) &&
           std::all_of(str.begin(), str.end(), [alphabet_size](char c) {
               return std::isalnum(c) &&
                      (std::isdigit(c)
                           ? static_cast<size_t>(c - '0')
                           : static_cast<size_t>(std::tolower(c) - 'a')) < alphabet_size;
           });
}

template <typename InputIt1, typename InputIt2, typename BinaryPredicate>
constexpr size_t count_if(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                          BinaryPredicate predicate) {
    size_t count = 0;
    while (first1 != last1) {
        if (predicate(*first1++, *first2++)) {
            ++count;
        }
    }
    return count;
}

}  // namespace

bulls_cows::bulls_cows(size_t alphabet_size, size_t word_length)
    : bulls_{0}, cows_{0}, word_length_{word_length} {
    tmp_storage_.resize(alphabet_size);
}

size_t bulls_cows::bulls() const noexcept {
    return bulls_;
}

size_t bulls_cows::cows() const noexcept {
    return cows_;
}

bool bulls_cows::is_correct() const noexcept {
    return bulls_ == word_length_;
}

bool bulls_cows::is_full() const noexcept {
    return bulls_ + cows_ == word_length_;
}

bool bulls_cows::is_empty() const noexcept {
    return bulls_ + cows_ == 0;
}

bool operator==(const bulls_cows &lhs, const bulls_cows &rhs) noexcept {
    return (lhs.bulls_ == rhs.bulls_) && (lhs.cows_ == rhs.cows_) &&
           (lhs.word_length_ == rhs.word_length_) &&
           (lhs.tmp_storage_.size() == rhs.tmp_storage_.size());
}

bulls_cows &bulls_cows::get_match(std::string_view lhs, std::string_view rhs) {
    if (!correct(lhs, word_length_, tmp_storage_.size()) ||
        !correct(rhs, word_length_, tmp_storage_.size())) {
        std::cout << "Sad" << std::endl;
        // throw
    }
    std::fill(tmp_storage_.begin(), tmp_storage_.end(), 0);
    count_letters(rhs);
    if (std::any_of(tmp_storage_.begin(), tmp_storage_.end(),
                    [](size_t amount) { return amount > 1; })) {
        std::cout << "Sad" << std::endl;
        // throw
    }
    std::fill(tmp_storage_.begin(), tmp_storage_.end(), 0);
    count_letters(lhs);
    if (std::any_of(tmp_storage_.begin(), tmp_storage_.end(),
                    [](size_t amount) { return amount > 1; })) {
        std::cout << "Sad" << std::endl;
        // throw
    }
    count_letters(rhs);

    cows_ = std::count(tmp_storage_.begin(), tmp_storage_.end(), 2) -
            (bulls_ = count_if(lhs.begin(), lhs.end(), rhs.begin(),
                               [](char c1, char c2) { return c1 == c2; }));
    return *this;
}

bulls_cows &bulls_cows::set_match(size_t bulls, size_t cows) {
    bulls_ = bulls;
    cows_ = cows;
    return *this;
}

void bulls_cows::count_letters(std::string_view str) {
    std::for_each(str.begin(), str.end(), [this](char c) {
        ++tmp_storage_[(std::isdigit(c) ? static_cast<size_t>(c - '0')
                                        : static_cast<size_t>(std::tolower(c) - 'a'))];
    });
}
