//
// Created by gbrl on 20/01/19.
//

#ifndef LASSO_H
#define LASSO_H

#include <chrono>

namespace lasso {
    using high_resolution_clock = std::chrono::high_resolution_clock;
    using hi_res_rep = std::chrono::high_resolution_clock::rep;
    using hi_res_period = std::chrono::high_resolution_clock::period;
    using high_resolution_duration = std::chrono::duration<hi_res_rep, hi_res_period>;

    struct LoopStatus {
        long fps{0};
        high_resolution_duration time_frame{0};
        high_resolution_duration time_simulation{0};
        high_resolution_duration time_total_elapsed{0};
        high_resolution_duration time_total_simulated{0};
        high_resolution_clock::time_point time_curr_frame = high_resolution_clock::now();
        high_resolution_clock::time_point time_prev_frame = high_resolution_clock::now();
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
        // allow some form of adjustment of delta!?
        high_resolution_duration const delta =
                high_resolution_duration{std::chrono::seconds{1}} / 60
                + std::chrono::nanoseconds{1};

        MainLoop() = default;

        void run(GameLogic &game_logic) {
            using namespace std::chrono_literals;

            LoopStatus status;
            game_logic.init();
            while (!game_logic.is_done()) {
                status.time_curr_frame = high_resolution_clock::now();
                status.time_frame = status.time_curr_frame
                                    - status.time_prev_frame;

                status.time_simulation += status.time_frame;

                while (status.time_simulation >= MainLoop::delta) {
                    game_logic.simulate(status, MainLoop::delta);
                    status.time_simulation -= MainLoop::delta;
                    status.time_total_simulated += MainLoop::delta;
                }

                status.fps = long(status.fps * this->fps_smoothing)
                             + long((1s / status.time_frame)
                             * (1.0 - this->fps_smoothing));

                game_logic.render(status, MainLoop::delta);

                status.time_total_elapsed += status.time_frame;
                status.time_prev_frame = status.time_curr_frame;
            }
        }

    private:
        // allow some form of adjustment of fps smoothing!?
        double fps_smoothing = 0.85;
    };

}

#endif //LASSO_H
