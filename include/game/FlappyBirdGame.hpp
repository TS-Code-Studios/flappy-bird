#ifndef FLAPPYBIRDGAME_HPP
#define FLAPPYBIRDGAME_HPP

#include <caffeine-gl/base.hpp>

class FlappyBirdGame final : public CaffeineGame {
public:
	FlappyBirdGame() = default;

	void init() override;
	void update(float deltaTime) override;
	void render() override;
};

#endif //FLAPPYBIRDGAME_HPP
