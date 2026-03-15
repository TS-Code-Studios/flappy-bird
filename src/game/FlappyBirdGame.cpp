#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow& window) : window(window) {
	boost = 550.0f;
	startBoost = 800.0f;
	gravity = -900.0f;
	pipeSpawnRateChance = 202; //je höher, desto seltener
	pipeSpawnRateMin = 5.0f; //zeitgestuert???? wass wenn level schneller?

	birdVel = glm::vec2(0.0f, startBoost);
	gameVel = glm::vec2(-100.0f, 0.0f);
	

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



	// for (CaffeineMeshDrawable*& pipe : pipes) {
	// 	pipe = ResourceManager::createGameObject<CaffeineMeshDrawable>(
	// 		0, ResourceManager::getMesh("quad"),
	// 		Material{
	// 			&ResourceManager::getShader("default"),
	// 			&ResourceManager::getTexture("pipe")
	// 	});
	// }

	bird->setLocation(birdSpawn);
	bird->setSize(glm::vec2(80.0f));

	for(int i = 0; i < sizeof(pipePairs) / sizeof(pipePairs[0]); i++) {
		pipePairs[i] = new PipePair();
	}
}

void FlappyBirdGame::update(const float deltaTime) {
	processInput();

	if (!gamePaused) {
		spawnPipe();
		despawnPipe();
		for (PipePair*& pipePair : pipePairs) {
			pipePair->move(deltaTime, gameVel.x);
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
	//else oder nicht else if
}

void FlappyBirdGame::spawnPipe() {
	// std::cout << static_cast<float>(glfwGetTime()) << "    ";
	// std::cout << lastPipeSpawnTime << "    ";
	// for (PipePair*& pipePair : pipePairs) {
	// 			std::cout << pipePair->used << "    ";
	// }
	std::cout << std::endl;
	if (static_cast<float>(glfwGetTime()) - lastPipeSpawnTime > pipeSpawnRateMin) {
		if (rand() % pipeSpawnRateChance == 0) {
			int freePipeIndex;
			for (PipePair*& pipePair : pipePairs) {
				if (!pipePair->used) {
					pipePair->spawn(virtualWidth, virtualHeight);
					lastPipeSpawnTime = static_cast<float>(glfwGetTime());
					break;
				}
			}
		}
	}
	// float pipeHeight = static_cast<float>(rand() % (static_cast<int>(virtualHeight / 2.0f)-100));
}

void FlappyBirdGame::despawnPipe() {
	for (PipePair*& pipePair : pipePairs) {
		if (pipePair->used && pipePair->topPipe->transform.position.x < 100.0f) {
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