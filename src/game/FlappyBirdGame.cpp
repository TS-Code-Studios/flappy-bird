#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow& window) : window(window) {
	gameVelValue = -100.0f;
	boost = 550.0f;
	startBoost = 100.0f;
	gravity = -900.0f;
	acceleration = 10.0f;
	pipeSpawnRateMin = 6.0f; //zeitgestuert???? wass wenn level schneller?

	birdVel = glm::vec2(0.0f, startBoost);
	gameVel = glm::vec2(gameVelValue, 0.0f);

	birdSpawn = glm::vec2(250.0f, 540.0f);

	lastPipeSpawnTime = 0.0f;
	gamePaused = true;
	birdIsDying = false;
}	


FlappyBirdGame::~FlappyBirdGame() {
	for(int i = 0; i < sizeof(pipePairs) / sizeof(pipePairs[0]); i++) {
		delete pipePairs[i];
	}
}

void FlappyBirdGame::init() {
	ResourceManager::setResourceRoot(ResourceManager::getExecutablePath() / "resources");

	ResourceManager::createDefaultMeshes();

	ResourceManager::loadShader("shaders/default.vert", "shaders/default.frag", nullptr, "default");
	
	ResourceManager::loadTexture("textures/missing_texture.png", "placeholder");
	ResourceManager::loadTexture("textures/bird.png", "bird");
	ResourceManager::loadTexture("textures/pipe.png", "pipe");
	ResourceManager::loadTexture("textures/background.png", "background");

	bird = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		1, ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("bird")
	});

	background = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		-1000, ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("background")
	});

	background2 = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		-1000, ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("background")
	});

	bird->setLocation(birdSpawn);
	bird->setSize(glm::vec2(100.0f, 70.0f));

	bird->collider = new Collider(bird, ColliderType::DYNAMIC, ColliderShape::QUAD, 
		glm::vec2(0.0f), glm::vec2(bird->transform.size.x, bird->transform.size.y));
	bird->collider->enable();
	bird->collider->collisionCallback = [this](CaffeineGameObject& otherGameObject) {
		gamePaused = true;
	};

	background->setLocation(glm::vec2(virtualWidth / 2, virtualHeight / 2));
	background->setSize(glm::vec2(virtualWidth, virtualHeight));
	background2->setLocation(glm::vec2(virtualWidth / 2 + virtualWidth - 10.0f, virtualHeight / 2));	
	background2->setSize(glm::vec2(virtualWidth, virtualHeight));

	for(int i = 0; i < sizeof(pipePairs) / sizeof(pipePairs[0]); i++) {
		pipePairs[i] = new PipePair();
	}
}

void FlappyBirdGame::update(const float deltaTime) {
	processInput();
	if (!gamePaused) {
		gameVel.x = gameVelValue - (score * acceleration);
		moveBackground(deltaTime, gameVel.x);
		
		movePipes(deltaTime);
		spawnPipe();
		despawnPipe();
		
		rotateBird();
		birdVel.y += gravity * deltaTime;
		bird->translate(birdVel * deltaTime);
	}
	if (gamePaused && birdIsDying) {birdDying(deltaTime);}
	checkGameOver();
}



void FlappyBirdGame::processInput() {
	if (window.keys[GLFW_KEY_UP] && !window.processedKeys[GLFW_KEY_UP]) {
		window.processedKeys[GLFW_KEY_UP] = true;
		birdVel.y = boost;
	}
	if (window.keys[GLFW_KEY_SPACE] && !window.processedKeys[GLFW_KEY_SPACE]) {
		window.processedKeys[GLFW_KEY_SPACE] = true;
		if (gamePaused) {
			resetGame();
		}
	}
	if (window.keys[GLFW_KEY_RIGHT] && !window.processedKeys[GLFW_KEY_RIGHT]) {
		window.processedKeys[GLFW_KEY_RIGHT] = true;
		gameVelValue += -40.0f;
	}
	if (window.keys[GLFW_KEY_LEFT] && !window.processedKeys[GLFW_KEY_LEFT]) {
		window.processedKeys[GLFW_KEY_LEFT] = true;
		gameVelValue += 40.0f;
	}
	if (window.keys[GLFW_KEY_V] && !window.processedKeys[GLFW_KEY_V]) {
		window.processedKeys[GLFW_KEY_V] = true;
		window.toggleFullscreen();
	}
}

void FlappyBirdGame::spawnPipe() {
	if (static_cast<float>(glfwGetTime()) - lastPipeSpawnTime > pipeSpawnRateMin) {
		for (PipePair*& pipePair : pipePairs) {
			if (!pipePair->used) {
				pipePair->spawn();
				lastPipeSpawnTime = static_cast<float>(glfwGetTime());
				break;
			}
		}
	}
}

void FlappyBirdGame::despawnPipe() {
	for (PipePair*& pipePair : pipePairs) {
		if (pipePair->used && pipePair->topPipe->transform.position.x < -100.0f) {
			pipePair->despawn();
		}
	}
}

void FlappyBirdGame::movePipes(float deltaTime) {
	for (PipePair*& pipePair : pipePairs) {
		pipePair->move(deltaTime, gameVel.x);
		if (bird->transform.position.x > pipePair->topPipe->transform.position.x) {
			if (pipePair->used && !pipePair->scored) {
				score ++;
				pipePair->scored = true;
			}
		}
	}
}

void FlappyBirdGame::moveBackground(float deltaTime, float gameVel) {
	background->translate(glm::vec2(gameVel * deltaTime * 0.3, 0.0f));
	background2->translate(glm::vec2(gameVel * deltaTime * 0.3, 0.0f));

	if (background->transform.position.x < -virtualWidth / 2) {
		background->setLocation(glm::vec2(virtualWidth / 2 + virtualWidth -10.0f, virtualHeight / 2));
	}
	if (background2->transform.position.x < -virtualWidth / 2) {
		background2->setLocation(glm::vec2(virtualWidth / 2 + virtualWidth - 10.0f, virtualHeight / 2));
	}
}

void FlappyBirdGame::resetGame() {
	score = 0;
	gamePaused = false;
	bird->setLocation(birdSpawn);
	birdVel = glm::vec2(0.0f, startBoost);
	for (PipePair*& pipePair : pipePairs) {
		pipePair->despawn();
	}
	gameVel = glm::vec2(gameVelValue, 0.0f);
	std::cout << "Game reset!" << std::endl;
}

void FlappyBirdGame::checkGameOver() {
	if (bird->transform.position.y < 0.0f || bird->transform.position.y > virtualHeight) {
		gamePaused = true;
		birdIsDying = true;
	}
}

void FlappyBirdGame::birdDying(float deltaTime) {
	glm::vec2 deathVel = glm::vec2(0.0f, boost);
	birdVel.y += gravity * deltaTime;
}

void FlappyBirdGame::rotateBird() {
	float rotation = pow(std::abs(birdVel.y) / boost, 1.0f / 1.0f) * 10.0f;
	(birdVel.y >= 0) ? bird->setRotation(rotation) : bird->setRotation(-rotation);
}

void FlappyBirdGame::render() {
	ResourceManager::renderAllDrawables();
}