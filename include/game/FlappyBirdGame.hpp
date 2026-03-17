#ifndef FLAPPYBIRDGAME_HPP
#define FLAPPYBIRDGAME_HPP

#include <caffeine-gl/base.hpp>
//gap generation
//den rest an schnellerres siel anpassen, pipe und spirngen
//death animation

struct PipePair {
	CaffeineMeshDrawable* topPipe;
	CaffeineMeshDrawable* bottomPipe;
	bool used;
	bool scored;

	float gapSize;
	float yOffset;

	PipePair() {
		used = false;
		scored = false;
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
		topPipe->setSize(glm::vec2(-140.0f, 800.0f));
		bottomPipe->setSize(glm::vec2(140.0f, 800.0f));

		topPipe->collider = new Collider(topPipe, ColliderType::STATIC, ColliderShape::QUAD, 
			glm::vec2(0.0f), glm::vec2(std::abs(topPipe->transform.size.x), topPipe->transform.size.y));
		topPipe->collider->enable();
		bottomPipe->collider = new Collider(bottomPipe, ColliderType::STATIC, ColliderShape::QUAD, 
			glm::vec2(0.0f), glm::vec2(std::abs(bottomPipe->transform.size.x), bottomPipe->transform.size.y));
		bottomPipe->collider->enable();

		bottomPipe->setRotation(180.0f);

		topPipe->visible = false;
		bottomPipe->visible = false;
	}

	void spawn() {
		used = true;
		topPipe->visible = true;
		bottomPipe->visible = true;
		
		gapSize = static_cast<float>(/*rand() % 400*/ + 1000);
		yOffset = 0;
		// yOffset = static_cast<float>(rand() % 500 - 300);
		
		topPipe->setLocation(glm::vec2(2000, yOffset));
		bottomPipe->setLocation(glm::vec2(2000, ((yOffset + gapSize >= 1300) ? 1300 : yOffset + gapSize)));
	}

	void despawn() {
		used = false;
		scored = false;
		topPipe->visible = false;
		bottomPipe->visible = false;
		topPipe->setLocation(glm::vec2(2000, -200.0f));
		bottomPipe->setLocation(glm::vec2(2000, -200.0f));
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
	CaffeineMeshDrawable *background;
	CaffeineMeshDrawable *background2;
	CaffeineWindow& window;

	FlappyBirdGame(CaffeineWindow& window);

	~FlappyBirdGame();

	void init() override;
	void update(float deltaTime) override;
	void render() override;
	void processInput();

	void resetGame();
	void checkGameOver();
	void birdDying(float deltaTime);

	void spawnPipe();
	void despawnPipe();
	void movePipes(float deltaTime);
	void moveBackground(float deltaTime, float gameVel);

	void rotateBird();

	glm::vec2 birdSpawn;

	glm::vec2 gameVel;
	glm::vec2 birdVel;

	float gameVelValue;

	float boost;
	float startBoost;
	float gravity;
	float acceleration;
	
	float pipeSpawnRateMin;
	float lastPipeSpawnTime;


	int score;
	bool gamePaused;
	bool birdIsDying;
};

#endif //FLAPPYBIRDGAME_HPP
