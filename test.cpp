#include <math.h>
#include <chrono>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "lasso.h"

constexpr auto nl = '\n';

struct GameLogic {
    bool done = false;
    bool circle_should_grow = true;
    sf::Font font;
    sf::CircleShape circle;
    sf::RenderWindow window;

    GameLogic() {
        this->window.create(sf::VideoMode(480, 240), "Lasso test");
        this->circle.setRadius(50.0f);
        this->circle.setPointCount(256);
        this->circle.setFillColor(sf::Color::Yellow);
        this->font.loadFromFile("res/font/Roboto-Regular.ttf");
    }

    void init() {
    }

    void simulate(lasso::LoopStatus const &status,
                  lasso::high_resolution_duration const &time_delta) {
//        for (int i = 0; i < 10'000; ++i) {
//            double x = std::sin(i);
//        }

        float const radius = this->circle.getRadius();
        if (this->circle_should_grow) {
            if (radius < 100.0f) {
                this->circle.setRadius(radius + 1.0f);
            } else {
                this->circle_should_grow = !this->circle_should_grow;
                this->circle.setRadius(radius - 1.0f);
            }
        } else {
            if (radius > 50.0f) {
                this->circle.setRadius(radius - 1.0f);
            } else {
                this->circle_should_grow = !this->circle_should_grow;
                this->circle.setRadius(radius + 1.0f);
            }
        }

        this->query_done();
    }

    void render(lasso::LoopStatus const &status,
                lasso::high_resolution_duration const &time_delta) {

        // just to pass time
//        for (int i = 0; i < 700'000; ++i) {
//            double x = std::sin(i);
//        }

        sf::Text text;
        text.setFont(this->font);
        text.setFillColor(sf::Color::Cyan);
        text.setString("FPS: " + std::to_string(status.fps));
        text.setPosition(300, 180);

        this->window.clear();
        this->window.draw(this->circle);
        this->window.draw(text);
        this->window.display();
    }

    [[nodiscard]] bool is_done() const noexcept {
        return this->done;
    }

    void query_done() {
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
    GameLogic gm;
    lasso::MainLoop().run(gm);
}