#ifndef FLAPPYBIRDGAME_HPP
#define FLAPPYBIRDGAME_HPP

#include <caffeine-gl/base.hpp>

class FlappyBirdGame final : public CaffeineGame {
public:
	CaffeineMeshDrawable *bird;
	CaffeineMeshDrawable *pipe0, *pipe1, *pipe2, *pipe3, *pipe4;
	CaffeineWindow& window;

	bool pipeUsed[5]; 

	FlappyBirdGame(CaffeineWindow& window);

	void init() override;
	void update(float deltaTime) override;
	void render() override;
	void processInput();

	void resetGame();
	void checkGameOver();
	void spawnPipe(); //void??????????????

	glm::vec2 birdSpawn;

	glm::vec2 gameVel;
	glm::vec2 birdVel;

	float boost;
	float gravity;

	int score;
	bool gameOver;
	bool gameActive;
};

#endif //FLAPPYBIRDGAME_HPP
