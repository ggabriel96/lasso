#include <math.h>
#include <chrono>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/lasso.h"

constexpr auto nl = '\n';

struct Scene {
    bool done{false};
    float circle_speed{50.0f};
    sf::Font font;
    sf::CircleShape circle;
    sf::RenderWindow window;

    explicit Scene(sf::RenderWindow const &window) {
        this->window.create(window.getSystemHandle());
    }

    void init() {
        this->circle.setRadius(50.0f);
        this->circle.setPointCount(256);
        this->circle.setPosition(50.0f, 50.0f);
        this->circle.setFillColor(sf::Color::Yellow);
        this->font.loadFromFile("data/fonts/Roboto-Regular.ttf");
    }

    void simulate(lasso::LoopStatus const &status,
                  lasso::high_resolution_duration const &time_delta) {
//        for (int i = 0; i < 10'000; ++i) {
//            double x = std::sin(i);
//        }

        float const ro = this->circle.getRadius();

        if ((this->circle_speed > 0.0f && ro >= 100.0f)
            || (this->circle_speed < 0.0f && ro <= 50.0f)) {
            this->circle_speed = -this->circle_speed;
        }

        /**
         * @TODO delta_sec is calculated every frame unnecessarily...
         */
        float const delta_sec = float(time_delta.count()) /
                                lasso::high_resolution_duration::period::den;
        this->circle.setRadius(ro + this->circle_speed * delta_sec);

        float const r = this->circle.getRadius();
        this->circle.setPosition(100.0f - r, 100.0f - r);

        this->query_done();
    }

    void render(lasso::LoopStatus const &status,
                lasso::high_resolution_duration const &time_delta) {

        // just to pass time
//        for (int i = 0; i < 700'000; ++i) {
//            double x = std::sin(i);
//        }

        sf::Text fps;
        fps.setFont(this->font);
        fps.setFillColor(sf::Color::Cyan);
        fps.setString("FPS: " + std::to_string(status.fps));
        fps.setPosition(300, 180);

        this->window.clear();
        this->window.draw(this->circle);
        this->window.draw(fps);
        this->window.display();
    }

    [[nodiscard]] bool is_done() const noexcept {
        return this->done;
    }

    void terminate() {
        /**
         * clear event queue
         */
        sf::Event e;
        while (this->window.pollEvent(e));
    }

    void query_done() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            this->done = true;
        }
    }
};

struct Game {
    bool done = false;
    sf::Font font;
    sf::RenderWindow window;

    /**
     * @TODO what should go in the constructor and what in init()?
     */
    Game() {
        this->window.create(sf::VideoMode(480, 240), "Lasso test");
    }

    void init() {
        this->font.loadFromFile("data/fonts/Roboto-Regular.ttf");
    }

    void simulate(lasso::LoopStatus const &status,
                  lasso::high_resolution_duration const &time_delta) {
        sf::Event event;
        /**
         * using sf::Keyboard::isKeyPressed() is a real-time
         * keyboard check, which is not needed here,
         * and it does not interfere with event polling
         * (i.e. keyboard events will still be there!)
         */
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Enter) {
                auto scene = Scene(this->window);
                lasso::MainLoop().run(scene);
            } else if (event.type == sf::Event::Closed
                       || (event.type == sf::Event::KeyPressed &&
                           event.key.code == sf::Keyboard::Escape)) {
                this->window.close();
                this->done = true;
            }
        }
    }

    void render(lasso::LoopStatus const &status,
                lasso::high_resolution_duration const &time_delta) {
        auto const[wx, wy] = this->window.getSize();

        sf::Text fps;
        fps.setFont(this->font);
        fps.setFillColor(sf::Color::Cyan);
        fps.setString(std::to_string(status.fps));
        fps.setPosition(
                wx - fps.getGlobalBounds().width - fps.getCharacterSize(), 0);

        sf::Text description;
        description.setFont(this->font);
        description.setFillColor(sf::Color::White);
        description.setString("Press ENTER to run 'game'");
        description.setPosition(
                wx / 2 - description.getGlobalBounds().width / 2,
                wy / 2 - description.getGlobalBounds().height / 2);

        this->window.clear();
        this->window.draw(fps);
        this->window.draw(description);
        this->window.display();
    }

    [[nodiscard]] bool is_done() const noexcept {
        return this->done;
    }

    void terminate() {}
};

int main() {
    lasso::MainLoop{}.run(Game{});
}