#include <math.h>
#include <chrono>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "lasso.h"

constexpr auto nl = '\n';

struct Scene {
    bool done = false;
    bool circle_should_grow = true;
    sf::Font font;
    sf::CircleShape circle;
    sf::RenderWindow window;

    explicit Scene(sf::RenderWindow const &window) {
        this->window.create(window.getSystemHandle());
    }

    ~Scene() {
        /**
         * clear event queue
         */
        sf::Event e;
        while (this->window.pollEvent(e));
    }

    void init() {
        this->circle.setRadius(50.0f);
        this->circle.setPointCount(256);
        this->circle.setFillColor(sf::Color::Yellow);
        this->font.loadFromFile("res/font/Roboto-Regular.ttf");
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
        this->font.loadFromFile("res/font/Roboto-Regular.ttf");
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
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                auto scene = Scene(this->window);
                lasso::MainLoop().run(scene);
            } else if (event.type == sf::Event::Closed
                || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                this->window.close();
                this->done = true;
            }
        }
    }

    void render(lasso::LoopStatus const &status,
                lasso::high_resolution_duration const &time_delta) {
        auto const [wx, wy] = this->window.getSize();

        sf::Text fps;
        fps.setFont(this->font);
        fps.setFillColor(sf::Color::Cyan);
        fps.setString(std::to_string(status.fps));
        fps.setPosition(wx - fps.getGlobalBounds().width - fps.getCharacterSize(), 0);

        sf::Text description;
        description.setFont(this->font);
        description.setFillColor(sf::Color::White);
        description.setString("Press ENTER to run 'game'");
        description.setPosition(wx / 2 - description.getGlobalBounds().width / 2,
                wy / 2 - description.getGlobalBounds().height / 2);

        this->window.clear();
        this->window.draw(fps);
        this->window.draw(description);
        this->window.display();
    }

    [[nodiscard]] bool is_done() const noexcept {
        return this->done;
    }
};

int main() {
    Game game;
    lasso::MainLoop().run(game);
}