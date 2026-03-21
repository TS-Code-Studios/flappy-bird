#ifndef FLAPPYBIRDGAME_HPP
#define FLAPPYBIRDGAME_HPP

#include <caffeine-gl/base.hpp>
//den rest an schnellerres siel anpassen, pipe und springen
//pipes abnhägnig von geschwindigkeit mehr spawnen
//wie soll spiel schneller werden? eine variable die die pipemovement beschleunigt und bird beschleuningt und so
//hintergrund aufteilen

struct PipePair {
	CaffeineMeshDrawable* bottomPipe;
	CaffeineMeshDrawable* topPipe;
	bool used;
	bool scored;

	float gapSize;
	float yOffset;

	PipePair() {
		used = false;
		scored = false;
		bottomPipe = ResourceManager::createGameObject<CaffeineMeshDrawable>(
			0, ResourceManager::getMesh("quad"),
			Material{
				&ResourceManager::getShader("default"),
				&ResourceManager::getTexture("pipe")
		});
		topPipe = ResourceManager::createGameObject<CaffeineMeshDrawable>(
			0, ResourceManager::getMesh("quad"),
			Material{
				&ResourceManager::getShader("default"),
				&ResourceManager::getTexture("pipe")
		});
		bottomPipe->setSize(glm::vec2(-140.0f, 800.0f));
		topPipe->setSize(glm::vec2(140.0f, 800.0f));

		bottomPipe->collider = new Collider(bottomPipe, ColliderType::STATIC, ColliderShape::QUAD, 
			glm::vec2(0.0f), glm::vec2(std::abs(bottomPipe->transform.size.x), bottomPipe->transform.size.y));
		bottomPipe->collider->enable();
		topPipe->collider = new Collider(topPipe, ColliderType::STATIC, ColliderShape::QUAD, 
			glm::vec2(0.0f), glm::vec2(std::abs(topPipe->transform.size.x), topPipe->transform.size.y));
		topPipe->collider->enable();

		topPipe->setRotation(180.0f);

		bottomPipe->visible = false;
		topPipe->visible = false;
	}

	void spawn() {
		used = true;
		bottomPipe->visible = true;
		topPipe->visible = true;
		
		gapSize = static_cast<float>(rand() % 400 + 1000);
		yOffset = static_cast<float>(rand() % static_cast<int>(1680 - gapSize))-300;
		
		bottomPipe->setLocation(glm::vec2(2000, yOffset));
		topPipe->setLocation(glm::vec2(2000, yOffset + gapSize));
	}

	void despawn() {
		used = false;
		scored = false;
		bottomPipe->visible = false;
		topPipe->visible = false;
		bottomPipe->setLocation(glm::vec2(2000, -200.0f));
		topPipe->setLocation(glm::vec2(2000, -200.0f));
	}

	void move(float deltaTime, float gameVel) {
		if (used) {
			bottomPipe->translate(glm::vec2(gameVel * deltaTime, 0.0f));
			topPipe->translate(glm::vec2(gameVel * deltaTime, 0.0f));
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
