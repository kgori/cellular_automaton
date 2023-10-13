#include "grid.h"
#include "sfml_renderer.h"
#include <random>
#include <SFML/System/Clock.hpp>
const int H = 96;
const int W = 144;

int main() {

    std::random_device rd;
    auto rng = std::mt19937(rd());

    auto g = Grid(H, W);
    for (int i = 0; i < 6*H*W/240; ++i) {
        int x = std::uniform_int_distribution(0, H-1)(rng);
        int y = std::uniform_int_distribution(0, W-1)(rng);
        g.set(x, y);
    }

    auto renderer = SFMLRenderer(1440, 960, W, H);

    sf::Clock clock;
    const float fixedTimeStep = 0.1f; // Desired time step in seconds
    float accumulatedTime = 0.0f;

    while (renderer.isWindowOpen()) {
        float deltaTime = clock.restart().asSeconds();
        accumulatedTime += deltaTime;

        auto inputs = renderer.collectUserInputs(g);
        auto [cell_width, cell_height] = renderer.cellSize();
        g.handleInputs(inputs, cell_width, cell_height);
        while (accumulatedTime >= fixedTimeStep) {
            if (!g.isPaused()) {
                g.update();
            }
            accumulatedTime -= fixedTimeStep;
        }
        renderer.draw(g);
    }

    return 0;
}
