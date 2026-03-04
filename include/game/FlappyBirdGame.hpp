#ifndef FLAPPYBIRDGAME_HPP
#define FLAPPYBIRDGAME_HPP

#include <caffeine-gl/base.hpp>

class FlappyBirdGame : public CaffeineGame {
public:
	int WIDTH, HEIGHT;

	Renderer* renderer;

	explicit FlappyBirdGame(CaffeineWindow& window);
	~FlappyBirdGame() override;

	void init() override;
	void update(float deltaTime) override;
	void render() override;
};

#endif //FLAPPYBIRDGAME_HPP
