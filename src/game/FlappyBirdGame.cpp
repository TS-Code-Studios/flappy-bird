#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow& window) : window(window) {
	gameVelValue = -100.0f;
	boost = 550.0f;
	startBoost = 800.0f;
	gravity = -900.0f;
	pipeSpawnRateChance = 202; //je höher, desto seltener
	pipeSpawnRateMin = 6.0f; //zeitgestuert???? wass wenn level schneller?

	birdVel = glm::vec2(0.0f, startBoost);
	gameVel = glm::vec2(gameVelValue, 0.0f);

	birdSpawn = glm::vec2(250.0f, 540.0f);

	lastPipeSpawnTime = 0.0f;
	gamePaused = true;
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
	ResourceManager::loadTexture("textures/pipe.jpeg", "pipe");

	bird = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		0, ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("bird")
	});

	bird->collider = new Collider(bird, ColliderType::QUAD, glm::vec2(0.0f), glm::vec2(80.0f));
	bird->collider->enable();

	bird->setLocation(birdSpawn);
	bird->setSize(glm::vec2(80.0f));

	for(int i = 0; i < sizeof(pipePairs) / sizeof(pipePairs[0]); i++) {
		pipePairs[i] = new PipePair();
	}
}

void FlappyBirdGame::update(const float deltaTime) {
	processInput();
	gameVel.x = gameVelValue - (score * 100.0f);
	if (!gamePaused) {
		spawnPipe();
		despawnPipe();
		for (PipePair*& pipePair : pipePairs) {
			pipePair->move(deltaTime, gameVel.x);
			if (bird->transform.position.x > pipePair->topPipe->transform.position.x) {
				if (pipePair->used && !pipePair->scored) {
					score++;
					pipePair->scored = true;
				}
			}
		}
		birdVel.y += gravity * deltaTime;
		bird->translate(birdVel * deltaTime);
	}
	checkGameOver();
}

void FlappyBirdGame::processInput() {
	if (window.keys[GLFW_KEY_UP] && !window.processedKeys[GLFW_KEY_UP]) {
		window.processedKeys[GLFW_KEY_UP] = true;
		birdVel.y = boost;	
		if (gamePaused) {
			resetGame();
		}
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

void FlappyBirdGame::resetGame() {
	score = 0;
	gamePaused = false;
	bird->setLocation(birdSpawn);
	birdVel = glm::vec2(0.0f, boost);
}

void FlappyBirdGame::checkGameOver() {
	if (bird->transform.position.y < 0.0f || bird->transform.position.y > virtualHeight) {
		gamePaused = true;
	}
	//check collision with pipes
}

void FlappyBirdGame::render() {
	ResourceManager::renderAllDrawables();
}