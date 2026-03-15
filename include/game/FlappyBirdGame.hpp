#ifndef FLAPPYBIRDGAME_HPP
#define FLAPPYBIRDGAME_HPP

#include <caffeine-gl/base.hpp>

struct PipePair {
	CaffeineMeshDrawable* topPipe;
	CaffeineMeshDrawable* bottomPipe;
	bool used;

	float gapSize;
	float yOffset;

	PipePair() {
		used = false;
		topPipe = ResourceManager::createGameObject<CaffeineMeshDrawable>(
			0, ResourceManager::getMesh("quad"),
			Material{
				&ResourceManager::getShader("default"),
				&ResourceManager::getTexture("pipe")
		});
		bottomPipe = ResourceManager::createGameObject<CaffeineMeshDrawable>(
			0, ResourceManager::getMesh("quad"),
			Material{
				&ResourceManager::getShader("default"),
				&ResourceManager::getTexture("pipe")
		});
		topPipe->setSize(glm::vec2(300.0f));
		bottomPipe->setSize(glm::vec2(300.0f));
		bottomPipe->setRotation(180.0f);
		topPipe->visible = false;
		bottomPipe->visible = false;
	}

	void spawn(float virtualWidth, float virtualHeight) {
		used = true;
		topPipe->visible = true;
		bottomPipe->visible = true;
		
		yOffset = static_cast<float>(rand() % static_cast<int>((virtualHeight / 2.7) + 0.04f * (virtualHeight)));//damit in 66% des bildschirms
		gapSize = static_cast<float>(rand() % 300 + 600); //abhängig von größe relevant??
		
		topPipe->setLocation(glm::vec2(virtualWidth, yOffset));		//ändern
		bottomPipe->setLocation(glm::vec2(virtualWidth, ((yOffset + gapSize >= virtualHeight) ? virtualHeight : yOffset + gapSize)));
	}

	void despawn() {
		used = false;
		topPipe->visible = false;
		bottomPipe->visible = false;
	}

	void move(float deltaTime, float gameVel) {
		if (used) {
			topPipe->translate(glm::vec2(gameVel * deltaTime, 0.0f));
			bottomPipe->translate(glm::vec2(gameVel * deltaTime, 0.0f));
		}
	}
};

class FlappyBirdGame final : public CaffeineGame {
public:
	CaffeineMeshDrawable *bird;
	PipePair *pipePairs[10];
	CaffeineWindow& window;

	FlappyBirdGame(CaffeineWindow& window);

	~FlappyBirdGame();

	void init() override;
	void update(float deltaTime) override;
	void render() override;
	void processInput();

	void resetGame();
	void checkGameOver();
	void spawnPipe();
	void despawnPipe();

	glm::vec2 birdSpawn;

	glm::vec2 gameVel;
	glm::vec2 birdVel;

	float boost;
	float startBoost;
	float gravity;
	
	float pipeSpawnRateMin;
	int pipeSpawnRateChance;
	float lastPipeSpawnTime;


	int score;
	bool gamePaused;

	
};

#endif //FLAPPYBIRDGAME_HPP
