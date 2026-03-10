#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow &window) {
	WIDTH = static_cast<float>(window.WIDTH);
	HEIGHT = static_cast<float>(window.HEIGHT);

	renderer = new Renderer(WIDTH, HEIGHT);
}

CaffeineMeshDrawable* test;
CaffeineMeshDrawable* test2;
glm::vec2 velocity(0.1f, 0.0f);
void FlappyBirdGame::init() {
	ResourceManager::setResourceRoot(ResourceManager::getExecutablePath() / "resources");

	ResourceManager::createDefaultMeshes();
	ResourceManager::loadShader("shaders/default.vert", "shaders/default.frag", nullptr, "default");
	ResourceManager::loadTexture("textures/missing_texture.png", "placeholder");
	ResourceManager::loadTexture("textures/bird.jpeg", "bird");
	

	test2 = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("bird")
		}
	);

	test = ResourceManager::createGameObject<CaffeineMeshDrawable>(
		ResourceManager::getMesh("quad"),
		Material{
			&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("placeholder")
		}
	);

	const glm::vec2 bottom_left(WIDTH / 4.0f, HEIGHT / 4.0f);
	test2->move(bottom_left);
	test2->scale(glm::vec2(100.0f));

	const glm::vec2 centerOfScreen(WIDTH / 2.0f, HEIGHT / 2.0f);
	test->move(centerOfScreen);
	test->scale(glm::vec2(100.0f));

}

void FlappyBirdGame::update(float deltaTime) {
	static float elapsedTime = 0.0f;
	elapsedTime += deltaTime;
	if (elapsedTime < 5.0f) {
		return;
	}
	elapsedTime = 0.0f;
	test->move(velocity * deltaTime);
}

void FlappyBirdGame::render() {
	ResourceManager::renderAllDrawables(*renderer);
}



FlappyBirdGame::~FlappyBirdGame() {
	ResourceManager::clear();
	delete renderer;
}