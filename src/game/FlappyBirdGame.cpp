#include <game/FlappyBirdGame.hpp>

CaffeineMeshDrawable* test;
CaffeineMeshDrawable* bird;
glm::vec2 velocity(100.0f, 0.0f);

void FlappyBirdGame::init() {
	ResourceManager::setResourceRoot(ResourceManager::getExecutablePath() / "resources");

	ResourceManager::createDefaultMeshes();
	ResourceManager::loadShader("shaders/default.vert", "shaders/default.frag", nullptr, "default");
	ResourceManager::loadTexture("textures/missing_texture.png", "placeholder");
	ResourceManager::loadTexture("textures/bird.jpeg", "bird");
	

	bird = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		0,
		ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("bird")
		}
	);

	test = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		10,
		ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("placeholder")
		}
	);

	const glm::vec2 bottom_left(virtualWidth / 4.0f, virtualHeight / 4.0f);
	bird->setLocation(bottom_left);
	bird->setSize(glm::vec2(100.0f));

	const glm::vec2 centerOfScreen(virtualWidth / 2.0f, virtualHeight / 2.0f);
	test->setLocation(centerOfScreen);
	test->setSize(glm::vec2(100.0f));

}

void FlappyBirdGame::update(const float deltaTime) {
	test->translate(velocity * deltaTime);
}

void FlappyBirdGame::render() {
	ResourceManager::renderAllDrawables();
}