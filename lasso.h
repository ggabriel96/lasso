//
// Created by gbrl on 20/01/19.
//

#ifndef LASSO_H
#define LASSO_H

#include <chrono>
#include <type_traits>

namespace lasso {
    using high_resolution_clock = std::chrono::high_resolution_clock;
    using hi_res_rep = std::chrono::high_resolution_clock::rep;
    using hi_res_period = std::chrono::high_resolution_clock::period;
    using high_resolution_duration = std::chrono::duration<hi_res_rep, hi_res_period>;

    struct LoopStatus {
        long fps{0};
        high_resolution_duration time_available{0};
    };

    template<typename Impl>
    class LoopManager {
        using impl = Impl;

        [[nodiscard]] bool is_done() const noexcept {
            return static_cast<Impl *>(this)->done();
        }

        void render(LoopStatus const &status, high_resolution_duration const &time_delta) {
            static_cast<Impl *>(this)->render(status, time_delta);
        }

        void simulate(LoopStatus const &status, high_resolution_duration const &time_delta) {
            static_cast<Impl *>(this)->simulate(status, time_delta);
        }
    };

    class MainLoop {
        double fps_smoothing = 0.85;
        high_resolution_duration const delta =
                high_resolution_duration{std::chrono::seconds{1}} / 60 + std::chrono::nanoseconds{1};

    public:
        MainLoop() = default;

        template<typename Impl>
        void run(Impl &loopManager) {
            static_assert(std::is_base_of_v<LoopManager<Impl>, Impl>);
            using namespace std::chrono_literals;

            LoopStatus status;
            auto time_prev = high_resolution_clock::now();
            auto time_total = high_resolution_duration{0};
            auto time_total_sim = high_resolution_duration{0};

            while (!loopManager.is_done()) {
                auto const time_cur = high_resolution_clock::now();
                auto const time_frame = time_cur - time_prev;

                status.time_available += time_frame;

                while (MainLoop::delta <= status.time_available) {
                    loopManager.simulate(status, MainLoop::delta);
                    status.time_available -= MainLoop::delta;
                    time_total_sim += MainLoop::delta;
                }

                status.fps = long(status.fps * this->fps_smoothing)
                             + long((1s / time_frame) * (1.0 - this->fps_smoothing));

                loopManager.render(status, MainLoop::delta);

                time_total += time_frame;
                time_prev = time_cur;
            }
        }

    private:
    };

}

#endif //LASSO_H
