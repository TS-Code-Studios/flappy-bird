#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow &window) {
	WIDTH = static_cast<float>(window.WIDTH);
	HEIGHT = static_cast<float>(window.HEIGHT);

	renderer = new Renderer(WIDTH, HEIGHT);
}

CaffeineMeshDrawable* test;
void FlappyBirdGame::init() {
	ResourceManager::setResourceRoot(ResourceManager::getExecutablePath() / "resources");

	ResourceManager::createDefaultMeshes();
	ResourceManager::loadShader("shaders/default.vert", "shaders/default.frag", nullptr, "default");
	ResourceManager::loadTexture("textures/missing_texture.png", "placeholder");



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
	ResourceManager::renderAllDrawables(*renderer);
}



FlappyBirdGame::~FlappyBirdGame() {
	ResourceManager::clear();
	delete renderer;
}