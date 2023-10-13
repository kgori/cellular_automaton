#include "grid.h"
#include "sfml_renderer.h"
#include <random>

const int H = 256;
const int W = 384;

int main() {

    std::random_device rd;
    auto rng = std::mt19937(rd());

    auto g = Grid(H, W);
    for (int i = 0; i < 6*H*W/10; ++i) {
        int x = std::uniform_int_distribution(0, H-1)(rng);
        int y = std::uniform_int_distribution(0, W-1)(rng);
        g.set(x, y);
    }

//    auto n_updated = 10000;
//    auto max_iter = 100;
//    while (n_updated > 0 && max_iter-- > 0) {
//        n_updated = g.update();
//        //g.print_values();
//    }

    auto renderer = SFMLRenderer(1600, 1200, W, H);
    while (renderer.isWindowOpen()) {
        renderer.update(g);
        g.update();
    }



    return 0;
}
