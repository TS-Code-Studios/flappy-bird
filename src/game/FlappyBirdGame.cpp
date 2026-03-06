#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow &window) {
	WIDTH = window.WIDTH;
	HEIGHT = window.HEIGHT;

	renderer = new Renderer();
}
CaffeineMeshDrawable* test;
void FlappyBirdGame::init() {
	ResourceManager::setResourceRoot(ResourceManager::getExecutablePath() / "resources");

	ResourceManager::createDefaultMeshes();
	ResourceManager::loadShader("shaders/default.vert", "shaders/default.frag", nullptr, "default");
	ResourceManager::loadTexture("textures/missing_texture.png", true, "placeholder");



	test = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("placeholder")
		}
	);

	const glm::vec2 centerOfScreen(WIDTH / 2.0f, HEIGHT / 2.0f);
	test->move(centerOfScreen);
	test->scale(glm::vec2(100.0f));
}

void FlappyBirdGame::update(float deltaTime) {
}

void FlappyBirdGame::render() {
	test->submitToRenderer(*renderer);
	renderer->renderAll();
}



FlappyBirdGame::~FlappyBirdGame() {
	ResourceManager::clear();
	delete renderer;
}
