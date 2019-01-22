#include <math.h>
#include <chrono>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "lasso.h"

constexpr auto nl = '\n';

struct LM : lasso::LoopManager<LM> {
    bool done = false;
    sf::RenderWindow window;

    [[nodiscard]] bool is_done() const noexcept {
        return this->done;
    }

    void render(lasso::LoopStatus const &status,
                lasso::high_resolution_duration const &time_delta) {

        double x = 0.0;
        for (int i = 0; i < 700'000; ++i) {
            x = std::sin(i);
        }
        std::cout << x << nl;

        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Yellow);

        sf::Font font;
        font.loadFromFile("res/font/Roboto-Regular.ttf");

        sf::Text text;
        text.setString("FPS: " + std::to_string(status.fps));
        text.setFont(font);
        text.setFillColor(sf::Color::Cyan);
        text.setOutlineColor(sf::Color::Black);
        text.setPosition(300, 180);

        this->window.clear();
        this->window.draw(shape);
        this->window.draw(text);
        this->window.display();
    }

    void simulate(lasso::LoopStatus const &status,
                  lasso::high_resolution_duration const &time_delta) {
        sf::Event event;
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->window.close();
                this->done = true;
            }
        }
    }
};

int main() {
    LM lm;
    lm.window.create(sf::VideoMode(480, 240), "SFML");
    lasso::MainLoop{}.run(lm);
}