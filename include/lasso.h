//
// Created by gbrl on 20/01/19.
//

#ifndef LASSO_LIBRARY_H
#define LASSO_LIBRARY_H

#include <algorithm>
#include <chrono>
#include <thread>
#include <type_traits>

namespace lasso {
using namespace std::chrono_literals;
using clock = std::chrono::steady_clock;
using duration = std::chrono::duration<clock::rep, clock::period>;
static_assert(lasso::clock::period::den > std::milli::den, "'lasso::clock' (aka 'std::chrono::steady_clock') resolution"
                                                           " is not higher than std::milli (milliseconds)");

constexpr static float default_smoothing = 0.0;
constexpr static unsigned int default_simulations = 30;

struct Options {
    unsigned short max_fps = 0;
    unsigned short simulations_per_second = default_simulations;
    float fps_smoothing = default_smoothing;
};

struct LoopStatus {
    int fps{0};
    duration time_last_iteration{0};
    duration time_last_render{0};
    duration time_last_simulation{0};
    duration time_total_elapsed{0};
    duration time_total_simulating{0};
    duration time_total_rendering{0};
    duration time_simulation_available{0};
    clock::time_point iteration_start_prev{clock::now()};
    clock::time_point iteration_start{clock::now()};
};

template <typename T>
concept GameLogic = requires(T logic, LoopStatus const &status, duration const &delta) {
    { logic.init() } -> std::same_as<void>;
    { logic.simulate(status, delta) } -> std::same_as<void>;
    { logic.render(status, delta) } -> std::same_as<void>;
    { logic.is_done() } -> std::same_as<bool>;
    { logic.terminate() } -> std::same_as<void>;
};

class MainLoop {
public:
    explicit MainLoop(Options options = {})
        : options(options), delta{duration{1s} / options.simulations_per_second}, max_simulation_incr{2 * delta} {}

    template <GameLogic GL>
    void run(GL &&game_logic) {
        game_logic.init();
        while (!game_logic.is_done()) {
            status.iteration_start_prev = status.iteration_start;
            status.iteration_start = clock::now();
            status.time_last_iteration = status.iteration_start - status.iteration_start_prev;
            status.time_total_elapsed += status.time_last_iteration;

            compute_fps();

            status.time_simulation_available +=
                std::clamp(status.time_last_iteration, std::chrono::nanoseconds::zero(), max_simulation_incr);
            while (status.time_simulation_available >= delta) {
                auto const simulation_start = clock::now();
                game_logic.simulate(status, delta);
                status.time_last_simulation = clock::now() - simulation_start;
                status.time_total_simulating += status.time_last_simulation;
                status.time_simulation_available -= delta;
            }

            auto const render_start = clock::now();
            game_logic.render(status, delta);
            status.time_last_render = clock::now() - render_start;
            status.time_total_rendering += status.time_last_render;

            if (options.max_fps > 0) {
                // floor it to allow some headroom
                thread_local static auto const sleep_amount =
                    std::chrono::floor<std::chrono::milliseconds>(duration{1s} / options.max_fps);
                std::this_thread::sleep_until(status.iteration_start + sleep_amount);
            }
        }
        game_logic.terminate();
    }

private:
    LoopStatus status;
    Options options;
    duration const delta;
    duration const max_simulation_incr;

    inline void compute_fps() {
        status.fps = int(status.fps * options.fps_smoothing) + int((1s / status.time_last_iteration) * (1.0 - options.fps_smoothing));
    }
};

} // namespace lasso

#endif // LASSO_LIBRARY_H
