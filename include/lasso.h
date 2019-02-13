//
// Created by gbrl on 20/01/19.
//

#ifndef LASSO_LIBRARY_H
#define LASSO_LIBRARY_H

#include <chrono>

namespace lasso {
    using namespace std::chrono_literals;
    using high_resolution_clock = std::chrono::high_resolution_clock;
    using hi_res_rep = std::chrono::high_resolution_clock::rep;
    using hi_res_period = std::chrono::high_resolution_clock::period;
    using high_resolution_duration = std::chrono::duration<hi_res_rep, hi_res_period>;

    constexpr unsigned int default_smoothing = 0.75;
    constexpr unsigned int default_simulations = 30;

    struct LoopStatus {
        long fps{0};
        high_resolution_duration time_frame{0};
        high_resolution_duration time_total_elapsed{0};
        high_resolution_duration time_total_simulated{0};
        high_resolution_duration time_simulation_available{0};
        high_resolution_clock::time_point time_curr{
                high_resolution_clock::now()};
        high_resolution_clock::time_point time_prev{
                high_resolution_clock::now()};
    };

    template<typename T> concept bool GameLogic =
    requires (T logic,
              LoopStatus const &status,
              high_resolution_duration const &time_delta) {
        { logic.init() } -> void;
        { logic.simulate(status, time_delta) } -> void;
        { logic.render(status, time_delta) } -> void;
        { logic.is_done() } noexcept -> bool;
    };

    class MainLoop {
    public:
        high_resolution_duration const delta;

        explicit MainLoop(
                unsigned int simulations_per_second = default_simulations,
                float fps_smoothing = default_smoothing)
                : delta{high_resolution_duration{1s} / simulations_per_second},
                  fps_smoothing{fps_smoothing} {}

        void run(GameLogic &&game_logic) {
            game_logic.init();
            while (!game_logic.is_done()) {
                status.time_curr = high_resolution_clock::now();
                status.time_frame = status.time_curr - status.time_prev;

                status.time_simulation_available += status.time_frame;
                while (status.time_simulation_available >= MainLoop::delta) {
                    game_logic.simulate(status, MainLoop::delta);
                    status.time_simulation_available -= MainLoop::delta;
                    status.time_total_simulated += MainLoop::delta;
                }

                compute_fps();

                game_logic.render(status, MainLoop::delta);

                status.time_total_elapsed += status.time_frame;
                status.time_prev = status.time_curr;
            }
        }

    private:
        LoopStatus status;
        float fps_smoothing;

        inline void compute_fps() {
            status.fps = long(status.fps * fps_smoothing) +
                         long((1s / status.time_frame) * (1.0 - fps_smoothing));
        }
    };

}

#endif //LASSO_LIBRARY_H
