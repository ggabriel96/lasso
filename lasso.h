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
        high_resolution_duration time_simulation{0};
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
            /**
             * @TODO add all these time variables to loop status
             */
            auto time_prev = high_resolution_clock::now();
            auto time_total = high_resolution_duration{0};
            auto time_total_sim = high_resolution_duration{0};

            while (!game_logic.is_done()) {
                auto const time_cur = high_resolution_clock::now();
                auto const time_frame = time_cur - time_prev;

                status.time_simulation += time_frame;

                while (MainLoop::delta <= status.time_simulation) {
                    game_logic.simulate(status, MainLoop::delta);
                    status.time_simulation -= MainLoop::delta;
                    time_total_sim += MainLoop::delta;
                }

                status.fps = long(status.fps * this->fps_smoothing)
                             + long((1s / time_frame) * (1.0 - this->fps_smoothing));

                game_logic.render(status, MainLoop::delta);

                time_total += time_frame;
                time_prev = time_cur;
            }
        }

    private:
        // allow some form of adjustment of fps smoothing!?
        double fps_smoothing = 0.85;
    };

}

#endif //LASSO_H
