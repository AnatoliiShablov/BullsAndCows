#include <algorithm>
#include <bitset>
#include <cctype>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <string>
#include <string_view>

class bulls_cows;

[[nodiscard]] constexpr bool operator==(bulls_cows const&, bulls_cows const&) noexcept;
[[nodiscard]] constexpr bool operator!=(bulls_cows const&, bulls_cows const&) noexcept;

class bulls_cows {
public:
    constexpr bulls_cows(size_t bulls, size_t cows) noexcept : bulls_{bulls}, cows_{cows} {}

    [[nodiscard]] constexpr size_t bulls() const noexcept { return bulls_; }
    [[nodiscard]] constexpr size_t cows() const noexcept { return cows_; }

    [[nodiscard]] constexpr bool is_correct() const noexcept { return bulls_ == 4; }

    [[nodiscard]] constexpr bool is_full() const noexcept { return bulls_ + cows_ == 4; }

    [[nodiscard]] constexpr bool is_empty() const noexcept { return bulls_ + cows_ == 0; }

    friend constexpr bool operator==(bulls_cows const& lhs, bulls_cows const& rhs) noexcept {
        return (lhs.bulls_ == rhs.bulls_) && (lhs.cows_ == rhs.cows_);
    }
    friend constexpr bool operator!=(bulls_cows const& lhs, bulls_cows const& rhs) noexcept {
        return !(lhs == rhs);
    }

private:
    size_t bulls_;
    size_t cows_;
};

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

class player {
public:
    [[nodiscard]] constexpr bulls_cows get_match(std::string_view check) const noexcept {
        return get_match(get_value(), check);
    }

    [[nodiscard]] virtual std::string get_variant() = 0;

    virtual void answer(bulls_cows const&) = 0;

    virtual ~player() = default;

protected:
    static constexpr void count(std::array<size_t, 10>& storage, std::string_view value) noexcept {
        std::for_each(value.begin(), value.end(),
                      [&storage](char c) { ++storage[static_cast<size_t>(c - '0')]; });
    }
    static constexpr bulls_cows get_match(std::string_view lhs, std::string_view rhs) noexcept {
        std::array<size_t, 10> tmp_storage{};
        count(tmp_storage, lhs);
        count(tmp_storage, rhs);

        size_t bulls = count_if(lhs.begin(), lhs.end(), rhs.begin(),
                                [](char c1, char c2) { return c1 == c2; });
        size_t cows = std::count(tmp_storage.begin(), tmp_storage.end(), 2) - bulls;
        return bulls_cows{bulls, cows};
    }

    virtual constexpr std::string_view get_value() const noexcept = 0;
};

class AI_hard_player: public player {
public:
    AI_hard_player() : value_{"0123456789"}, last_asked{} {
        std::shuffle(value_.begin(), value_.end(), std::mt19937());
        value_.erase(4);
        std::bitset<10> used;
        used.reset();
        for (size_t ch1 = 0; ch1 <= 9; ++ch1) {
            used.set(ch1);
            for (size_t ch2 = 0; ch2 <= 9; ++ch2) {
                if (used.test(ch2)) {
                    continue;
                }
                used.set(ch2);
                for (size_t ch3 = 0; ch3 <= 9; ++ch3) {
                    if (used.test(ch3)) {
                        continue;
                    }
                    used.set(ch3);
                    for (size_t ch4 = 0; ch4 <= 9; ++ch4) {
                        if (used.test(ch4)) {
                            continue;
                        }
                        all.emplace_back(std::initializer_list<char>{
                            static_cast<char>(ch1 + '0'), static_cast<char>(ch2 + '0'),
                            static_cast<char>(ch3 + '0'), static_cast<char>(ch4 + '0')});
                    }
                    used.reset(ch3);
                }
                used.reset(ch2);
            }
            used.reset(ch1);
        }
        current_variants = all;
    }

    std::string get_variant() {
        if (all.size() == current_variants.size()) {
            last_asked = "0123";
            return last_asked;
        }

        if (current_variants.size() == 1) {
            last_asked = current_variants.front();
            return last_asked;
        }

        if (current_variants.size() == 0) {
            last_asked = "shit";
            return last_asked;
        }

        std::array<std::array<size_t, 5>, 5> counter{};
        size_t min = current_variants.size() + 1;
        for (std::string const& rhs : all) {
            counter = std::array<std::array<size_t, 5>, 5>{};
            for (std::string const& lhs : current_variants) {
                auto tmp = get_match(lhs, rhs);
                ++counter[tmp.bulls()][tmp.cows()];
            }
            size_t tmp_max = 0;
            for (size_t i = 0; i < 5; ++i) {
                for (size_t j = 0; j < 5; ++j) {
                    if (counter[i][j] > tmp_max) {
                        tmp_max = counter[i][j];
                    }
                }
            }
            if (tmp_max < min) {
                min = tmp_max;
                last_asked = rhs;
            }
        }
        return last_asked;
    }

    void answer(bulls_cows const& answer) {
        std::vector<std::string> new_variants;
        for (std::string const& lhs : current_variants) {
            auto tmp = get_match(lhs, last_asked);
            if (tmp == answer) {
                new_variants.push_back(lhs);
            }
        }
        current_variants = new_variants;
    }

private:
    std::string_view get_value() const noexcept { return value_; }
    std::string value_;
    std::vector<std::string> all;
    std::vector<std::string> current_variants;
    std::string last_asked;
};

int main() {
    std::unique_ptr<player> ai = std::make_unique<AI_hard_player>();
    bulls_cows last_answer{0, 0};
    size_t b, c;
    while (!last_answer.is_correct()) {
        std::cout << ai->get_variant() << std::endl;
        std::cout << "Bulls: " << std::flush;
        std::cin >> b;
        std::cout << "Cows: " << std::flush;
        std::cin >> c;
        last_answer = bulls_cows{b, c};
        ai->answer(last_answer);
    }
    std::cout << "AI win" << std::endl;
    return 0;
}
