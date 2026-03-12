#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow& window) : window(window) {
	gameVel = glm::vec2(-10.0f, 0.0f);
	birdVel = glm::vec2(0.0f);
	boost = 550.0f;
	gravity = -800.0f;

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

	pipe5 = ResourceManager::createGameObject<CaffeineMeshDrawable>(
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

	//else oder nicht else if
}

void FlappyBirdGame::resetGame() {
	score = 0;
	gameOver = false;
	bird->setLocation(birdSpawn);
	birdVel = glm::vec2(0.0f, 0.0f);
}

void FlappyBirdGame::checkGameOver() {
	// if (bird->transform.position.y < 0.0f || bird->transform.position.y > virtualHeight) {
	// 	gameOver = true;
	// }
}

void FlappyBirdGame::render() {
	ResourceManager::renderAllDrawables();
}