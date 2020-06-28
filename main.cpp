#include <iostream>
#include <memory>

#include "AI_hard_player.hpp"

int main() {
    std::unique_ptr<player> ai = std::make_unique<AI_hard_player>(10, 5);
    bulls_cows last_answer{10, 5};
    last_answer.set_match(0, 0);
    size_t b, c;
    while (!last_answer.is_correct()) {
        if (std::string variant = ai->get_variant(); variant == "error") {
            std::cout << "error" << std::endl;
            return 0;
        } else {
            std::cout << variant << std::endl;
        }
        std::cout << "Bulls: " << std::flush;
        std::cin >> b;
        std::cout << "Cows: " << std::flush;
        std::cin >> c;
        last_answer.set_match(b, c);
        ai->answer(last_answer);
    }
    std::cout << "AI win" << std::endl;
    return 0;
}
