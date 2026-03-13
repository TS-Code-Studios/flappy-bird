#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow& window) : window(window) {
	boost = 550.0f;
	gravity = -800.0f;

	birdVel = glm::vec2(0.0f, boost);
	gameVel = glm::vec2(-10.0f, 0.0f);
	

	birdSpawn = glm::vec2(virtualWidth / 4.0f, virtualHeight / 2.0f);
}

void FlappyBirdGame::init() {
	ResourceManager::setResourceRoot(ResourceManager::getExecutablePath() / "resources");

	ResourceManager::createDefaultMeshes();

	ResourceManager::loadShader("shaders/default.vert", "shaders/default.frag", nullptr, "default");
	
	ResourceManager::loadTexture("textures/missing_texture.png", "placeholder");
	ResourceManager::loadTexture("textures/bird.jpeg", "bird");
	ResourceManager::loadTexture("textures/pipe.png", "pipe");

	bird = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		0, ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("bird")
	});

	pipe0 = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		0, ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("pipe")
	});

	pipe1 = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		0, ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("pipe")
	});

	pipe2 = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		0, ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("pipe")
	});

	pipe3 = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		0, ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("pipe")
	});

	pipe4 = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		0, ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("pipe")
	});

	bird->setLocation(birdSpawn);
	bird->setSize(glm::vec2(100.0f));
}

void FlappyBirdGame::update(const float deltaTime) {
	processInput();
	spawnPipe();
	birdVel.y += gravity * deltaTime;
	bird->translate(birdVel * deltaTime);
	checkGameOver();
}

void FlappyBirdGame::processInput() {
	if (window.keys[GLFW_KEY_UP] && !window.processedKeys[GLFW_KEY_UP]) {
		birdVel.y = boost;	
		window.processedKeys[GLFW_KEY_UP] = true;
	}
	if (!window.keys[GLFW_KEY_SPACE] && window.processedKeys[GLFW_KEY_SPACE]) {
		if (gameOver) {
			resetGame();
		}
	}
	if (window.keys[GLFW_KEY_V] && !window.processedKeys[GLFW_KEY_V]) {
		window.toggleFullscreen();
		window.processedKeys[GLFW_KEY_V] = true;
	}
	//else oder nicht else if
}

void FlappyBirdGame::spawnPipe() {
	if (rand() % 200 == 0) {
		float pipeHeight = static_cast<float>(rand() % static_cast<int>((virtualHeight / 2) + 0.05f * (virtualHeight)));//damit in 66% des bildschirms
		float gapSize = static_cast<float>(rand() % 300 + 600); //abhängig von größe relevant??
		
		// if (!pipeUsed[0]) {
			// 	pipeUsed[0] = true;
			// } 
		pipe0->setSize(glm::vec2(300.0f));
		pipe1->setSize(glm::vec2(300.0f));
		pipe1->setRotation(180.0f);
		pipe0->setLocation(glm::vec2(virtualWidth, pipeHeight));		//ändern
		pipe1->setLocation(glm::vec2(virtualWidth, ((pipeHeight + gapSize >= virtualHeight) ? virtualHeight : pipeHeight + gapSize)));	//ändern
	}
	// float pipeHeight = static_cast<float>(rand() % (static_cast<int>(virtualHeight / 2.0f)-100));
}

void FlappyBirdGame::resetGame() {
	score = 0;
	gameOver = false;
	bird->setLocation(birdSpawn);
	birdVel = glm::vec2(0.0f, boost);
	//gameActive = true;
}

void FlappyBirdGame::checkGameOver() {
	if (bird->transform.position.y < 0.0f || bird->transform.position.y > virtualHeight) {
		gameOver = true;
		std::cout << "Game Over! Final Score: " << score << std::endl;
	}
}

void FlappyBirdGame::render() {
	ResourceManager::renderAllDrawables();
}