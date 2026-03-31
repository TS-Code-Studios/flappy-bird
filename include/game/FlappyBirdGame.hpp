#ifndef FLAPPYBIRDGAME_HPP
#define FLAPPYBIRDGAME_HPP

#include <caffeine-gl/base.hpp>

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
		world.addComponent<CaffeineRenderComponent>(topPipe, {0, false});
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
		
		
		gapSize = static_cast<float>(rand() % 350 + 1050);
		yOffset = static_cast<float>(rand() % static_cast<int>(1680 - gapSize)) - 300;
		
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
};

class FlappyBirdGame {
public:
	CaffeineEntity bird;
	PipePair *pipePairs[10];
	CaffeineEntity backgroundColor, backgroundClouds, backgroundClouds2, backgroundBuildings, backgroundBuildings2, backgroundBushes, backgroundBushes2;
	CaffeineWindow& window;
	CaffeineWorld& world;
	CaffeineEntity scoreText;
	CaffeineEntity gameOverBackground, gameOverText1, gameOverText2, gameOverText3, gameOverText4;

	FlappyBirdGame(CaffeineWindow& window, CaffeineWorld& world);

	~FlappyBirdGame();

	void init();
	void update(float deltaTime);
	void render(float deltaTime);
	void processInput();

	void resetGame();
	void checkGameOver();
	void birdDying(float deltaTime);
    void birdCollisionCallback(CaffeineEntity thisEntity, CaffeineEntity otherEntity);
    void gameOver();

	void spawnPipe();
	void despawnPipe();
	void updateScore();
	void moveBackground();

	void rotateBird();

	glm::vec2 birdSpawn;

	glm::vec2 gameVel;
	glm::vec2 gameVelClouds;
	glm::vec2 gameVelBuildings;
	glm::vec2 gameVelBushes;
	glm::vec2 birdVel;

	float gameVelValue;

	float boost;
	float startBoost;
	float gravity;
	float acceleration;
	float gameVelFactor;
	
	float pipeSpawnRate;
	float lastPipeSpawnTime;

	const float virtualWidth = 1920.0f;
	const float virtualHeight = 1080.0f;

	int score;
	int highScore;
	bool gamePaused;
	bool birdIsDying;

	std::vector<CaffeineEntity> backgroundEntities;
};

#endif //FLAPPYBIRDGAME_HPP
