#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow &window) {
	WIDTH = window.WIDTH;
	HEIGHT = window.HEIGHT;

	ResourceManager::setResourceRoot(ResourceManager::getExecutablePath() / "resources");
	ResourceManager::loadShader("shaders/default.vert", "shaders/default.frag", nullptr, "default");

	renderer = new SpriteRenderer(ResourceManager::getShader("default"));
}

void FlappyBirdGame::init() {
}

void FlappyBirdGame::update(float deltaTime) {
}

void FlappyBirdGame::render() {
}



FlappyBirdGame::~FlappyBirdGame() {
	ResourceManager::clear();
	delete renderer;
}
