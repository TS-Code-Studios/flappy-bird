#ifndef FLAPPYBIRDGAME_HPP
#define FLAPPYBIRDGAME_HPP

#include <caffeine-gl/base.hpp>

class FlappyBirdGame : public CaffeineGame {
public:
	SpriteRenderer* renderer;

	explicit FlappyBirdGame(CaffeineWindow& window);

	void Init() override;
	void Update(float deltaTime) override;
	void Render() override;
};

#endif //FLAPPYBIRDGAME_HPP
