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

		topPipe->collider = new Collider(topPipe, ColliderType::QUAD, glm::vec2(0.0f), glm::vec2(100.0f, 600.0f));
		topPipe->collider->enable();
		bottomPipe->collider = new Collider(bottomPipe, ColliderType::QUAD, glm::vec2(0.0f), glm::vec2(100.0f, 600.0f));
		bottomPipe->collider->enable();

		topPipe->setSize(glm::vec2(100.0f, 600.0f));
		bottomPipe->setSize(glm::vec2(100.0f, 600.0f));

		bottomPipe->setRotation(180.0f);

		topPipe->visible = false;
		bottomPipe->visible = false;
	}

	void spawn() {
		used = true;
		topPipe->visible = true;
		bottomPipe->visible = true;
		
		yOffset = static_cast<float>(rand() % 500 - 300); //damit in 66% des bildschirms
		gapSize = static_cast<float>(rand() % 450 + 900); //abhängig von größe relevant??
		
		topPipe->setLocation(glm::vec2(1920, yOffset));		//ändern
		bottomPipe->setLocation(glm::vec2(1920, ((yOffset + gapSize >= 1300) ? 1300 : yOffset + gapSize)));
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
