#include <lasso/lasso.h>

namespace Blueprint {
    class Game {
    public:
        void init();
        void simulate(lasso::LoopStatus const &loopStatus,
                    lasso::high_resolution_duration const &delta);
        void render(lasso::LoopStatus const &loopStatus,
                    lasso::high_resolution_duration const &delta);
        bool is_done() noexcept;
        void terminate();
    private:
    };
}