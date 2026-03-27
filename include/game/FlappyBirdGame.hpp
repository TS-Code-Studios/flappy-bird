#ifndef FLAPPYBIRDGAME_HPP
#define FLAPPYBIRDGAME_HPP

#include <caffeine-gl/base.hpp>
//den rest an schnellerres siel anpassen, pipe und springen
//pipes abnhägnig von geschwindigkeit mehr spawnen
//wie soll spiel schneller werden? eine variable die die pipemovement beschleunigt und bird beschleuningt und so
//hintergrund aufteilen

struct PipePair {
	CaffeineWorld& world;
	CaffeineEntity bottomPipe = world.createEntity();
	CaffeineEntity topPipe = world.createEntity();
	bool used;
	bool scored;

	float gapSize;
	float yOffset;
	glm::vec2 &gameVel;

	PipePair(CaffeineWorld& world, glm::vec2 &gameVel) : world(world), gameVel(gameVel) {
		used = false;
		scored = false;
		
		world.addComponent<CaffeineTransformComponent>(bottomPipe, {glm::vec2(2000, -200.0f),  0.0f, glm::vec2(-140.0f, 800.0f)});
		world.addComponent<CaffeineRenderComponent>(bottomPipe, {0, false});
		world.addComponent<CaffeineMeshComponent>(bottomPipe, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));
		world.addComponent<CaffeineMaterialComponent>(bottomPipe, {&CaffeineResourceManager::getShader("default"),
			&CaffeineResourceManager::getTexture("pipe")});
		world.addComponent<CaffeineColliderComponent>(bottomPipe, {ColliderType::STATIC, ColliderShape::QUAD, true,
			glm::vec2(0.0f), glm::vec2(std::abs(world.getComponent<CaffeineTransformComponent>(bottomPipe).size.x), world.getComponent<CaffeineTransformComponent>(bottomPipe).size.y), 
			nullptr});
		world.addComponent<CaffeineVelocityComponent>(bottomPipe, CaffeineVelocityComponent(&gameVel));


		world.addComponent<CaffeineTransformComponent>(topPipe, {glm::vec2(2000, -200.0f),  180.0f, glm::vec2(140.0f, 800.0f)});
		world.addComponent<CaffeineRenderComponent>(topPipe, {false, 0});
		world.addComponent<CaffeineMeshComponent>(topPipe, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));
		world.addComponent<CaffeineMaterialComponent>(topPipe, {&CaffeineResourceManager::getShader("default"),
			&CaffeineResourceManager::getTexture("pipe")});
		world.addComponent<CaffeineColliderComponent>(topPipe, {ColliderType::STATIC, ColliderShape::QUAD, true,
			glm::vec2(0.0f), glm::vec2(std::abs(world.getComponent<CaffeineTransformComponent>(topPipe).size.x), world.getComponent<CaffeineTransformComponent>(topPipe).size.y), 
			nullptr});
		world.addComponent<CaffeineVelocityComponent>(topPipe, CaffeineVelocityComponent(&gameVel));
	}

	void spawn() {
		used = true;
		world.getComponent<CaffeineRenderComponent>(bottomPipe).visible = true;
		world.getComponent<CaffeineRenderComponent>(topPipe).visible = true;
		
		
		gapSize = static_cast<float>(rand() % 400 + 1000);
		yOffset = static_cast<float>(rand() % static_cast<int>(1680 - gapSize))-300;
		
		world.getComponent<CaffeineTransformComponent>(bottomPipe).position = glm::vec2(2000, yOffset);
		world.getComponent<CaffeineTransformComponent>(topPipe).position = glm::vec2(2000, yOffset + gapSize);
	}

	void despawn() {
		used = false;
		scored = false;
		world.getComponent<CaffeineRenderComponent>(bottomPipe).visible = false;
		world.getComponent<CaffeineRenderComponent>(topPipe).visible = false;
		world.getComponent<CaffeineTransformComponent>(bottomPipe).position = glm::vec2(2000, -200.0f);
		world.getComponent<CaffeineTransformComponent>(topPipe).position = glm::vec2(2000, -200.0f);
	}

	void move(float deltaTime, float gameVel) {
		// if (used) {
		// 	bottomPipe->translate(glm::vec2(gameVel * deltaTime, 0.0f));
		// 	topPipe->translate(glm::vec2(gameVel * deltaTime, 0.0f));
		// } //nicht nützlich
	}
};

class FlappyBirdGame {
public:
	CaffeineEntity bird;
	PipePair *pipePairs[10];
	CaffeineEntity background;
	CaffeineEntity background2;
	CaffeineWindow& window;
	CaffeineWorld& world;

	FlappyBirdGame(CaffeineWindow& window, CaffeineWorld& world);

	~FlappyBirdGame();

	void init();
	void update(float deltaTime);
	void render();
	void processInput();

	void resetGame();
	void checkGameOver();
	void birdDying(float deltaTime);
	void birdCollisionCallback(CaffeineEntity thisEntity, CaffeineEntity otherEntity);

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

	const float virtualWidth = 1920.0f;
	const float virtualHeight = 1080.0f;

	int score;
	bool gamePaused;
	bool birdIsDying;
};

#endif //FLAPPYBIRDGAME_HPP
