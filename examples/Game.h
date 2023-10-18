#include <lasso/lasso.h>

namespace Blueprint {
class Game {
public:
    void init();
    void simulate(lasso::LoopStatus const &loopStatus, lasso::duration const &delta);
    void render(lasso::LoopStatus const &loopStatus, lasso::duration const &delta);
    bool is_done();
    void terminate();

private:
};
} // namespace Blueprint