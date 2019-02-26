//
// Created by gbrl on 20/01/19.
//

#ifndef LASSO_LIBRARY_H
#define LASSO_LIBRARY_H

#include <chrono>

namespace lasso {
    using namespace std::chrono_literals;
    using clock = std::chrono::steady_clock;
    using duration = std::chrono::duration<clock::rep, clock::period>;
    static_assert(lasso::clock::period::den > std::milli::den,
                  "'lasso::clock' (aka 'std::chrono::steady_clock') resolution"
                  " is not higher than std::milli (milliseconds)");

    constexpr unsigned int default_smoothing = 0.75;
    constexpr unsigned int default_simulations = 30;

    struct LoopStatus {
        int fps{0};
        duration time_frame{0};
        duration time_total_elapsed{0};
        duration time_total_simulated{0};
        duration time_simulation_available{0};
        clock::time_point time_prev{clock::now()};
        clock::time_point time_curr{clock::now()};
    };

    template<typename T> concept bool GameLogic =
    requires (T logic,
              LoopStatus const &status,
              duration const &delta) {
        { logic.init() } -> void;
        { logic.simulate(status, delta) } -> void;
        { logic.render(status, delta) } -> void;
        { logic.is_done() } noexcept -> bool;
        { logic.terminate() } -> void;
    };

    class MainLoop {
    public:
        duration const delta;

        explicit MainLoop(
                unsigned int simulations_per_second = default_simulations,
                float fps_smoothing = default_smoothing)
                : delta{duration{1s} / simulations_per_second},
                  fps_smoothing{fps_smoothing} {}

        void run(GameLogic &&game_logic) {
            game_logic.init();
            while (!game_logic.is_done()) {
                status.time_curr = clock::now();
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
            game_logic.terminate();
        }

    private:
        LoopStatus status;
        float fps_smoothing;

        inline void compute_fps() {
            status.fps = int(status.fps * fps_smoothing) +
                         int((1s / status.time_frame) * (1.0 - fps_smoothing));
        }
    };

}

#endif //LASSO_LIBRARY_H
