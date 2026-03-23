#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow& window, CaffeineWorld& world) : window(window), world(world) {
	gameVelValue = -200.0f;
	boost = 550.0f;
	startBoost = 100.0f;
	gravity = -900.0f;
	acceleration = 40.0f;
	pipeSpawnRateMin = 4.0f; //zeitgestuert???? wass wenn level schneller?

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

	bird = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(bird, {birdSpawn,  0.0f, glm::vec2(100.0f, 70.0f)}); //ort, rotation, scale
	world.addComponent<CaffeineRenderComponent>(bird, {true, 1});//visible, layer
	world.addComponent<CaffeineMeshComponent>(bird, {&ResourceManager::getMesh("quad")});//mesh
	world.addComponent<CaffeineMaterialComponent>(bird, {&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("bird")});//shader, texture
	world.addComponent<CaffeineColliderComponent>(bird, {ColliderType::DYNAMIC, ColliderShape::QUAD, true,
		glm::vec2(0.0f), glm::vec2(world.getComponent<CaffeineTransformComponent>(bird).size.x, world.getComponent<CaffeineTransformComponent>(bird).size.y), 
		[this] (CaffeineEntity thisEntity, CaffeineEntity otherEntity) {birdCollisionCallback(thisEntity, otherEntity);}});//collidertype, colidershape, enabled, offset, size, callback
	world.addComponent<CaffeineVelocityComponent>(bird, {birdVel});//velocitywert

	background = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(background, {glm::vec2(virtualWidth / 2, virtualHeight / 2),  0.0f, glm::vec2(virtualWidth, virtualHeight)}); 
	world.addComponent<CaffeineRenderComponent>(background, {true, -1000});
	world.addComponent<CaffeineMeshComponent>(background, {&ResourceManager::getMesh("quad")});
	world.addComponent<CaffeineMaterialComponent>(background, {&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("background")});
	world.addComponent<CaffeineVelocityComponent>(background, {gameVel * 0.3f});

	world.addComponent<CaffeineTransformComponent>(background2, {glm::vec2(virtualWidth / 2 + virtualWidth - 10.0f, virtualHeight / 2),  0.0f, glm::vec2(virtualWidth, virtualHeight)});
	world.addComponent<CaffeineRenderComponent>(background2, {true, -1000});
	world.addComponent<CaffeineMeshComponent>(background2, {&ResourceManager::getMesh("quad")});
	world.addComponent<CaffeineMaterialComponent>(background2, {&ResourceManager::getShader("default"),
			&ResourceManager::getTexture("background")});
	world.addComponent<CaffeineVelocityComponent>(background2, {gameVel * 0.3f});

	for(int i = 0; i < sizeof(pipePairs) / sizeof(pipePairs[0]); i++) {
		pipePairs[i] = new PipePair(world, gameVel);
		pipePairs[i]->world.addComponent<CaffeineVelocityComponent>(pipePairs[i]->bottomPipe, {gameVel * 0.3f});
		pipePairs[i]->world.addComponent<CaffeineVelocityComponent>(pipePairs[i]->topPipe, {gameVel * 0.3f});
	}
}

void FlappyBirdGame::update(const float deltaTime) {
	processInput();
	if (!gamePaused) {
		//gameVel.x = gameVelValue - (score * acceleration);
		CaffeineVelocitySystem::update(world, deltaTime);
		CaffeineCollisionSystem::update(world);

		
		movePipes(deltaTime);
		spawnPipe();
		despawnPipe();
		
		rotateBird();
		birdVel.y += gravity * deltaTime;
		//world.getComponent<CaffeineTransformComponent>(bird).position += birdVel * deltaTime;//nicht nützlich
	}
	if (gamePaused && birdIsDying) {birdDying(deltaTime);}
	checkGameOver();
}



void FlappyBirdGame::processInput() {
	if (window.keys[GLFW_KEY_UP] && !window.processedKeys[GLFW_KEY_UP]) {
		window.processedKeys[GLFW_KEY_UP] = true;
		if (!gamePaused) {
			birdVel.y = boost;
		}
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
		if (pipePair->used && pipePair->world.getComponent<CaffeineTransformComponent>(pipePair->bottomPipe).position.x < -100.0f) {
			pipePair->despawn();
		}
	}
}

void FlappyBirdGame::movePipes(float deltaTime) {
	for (PipePair*& pipePair : pipePairs) {
		pipePair->move(deltaTime, gameVel.x);
		if (world.getComponent<CaffeineTransformComponent>(bird).position.x > pipePair->world.getComponent<CaffeineTransformComponent>(pipePair->bottomPipe).position.x) {
			if (pipePair->used && !pipePair->scored) {
				score ++;
				pipePair->scored = true;
			}
		}
	}
}

void FlappyBirdGame::moveBackground(float deltaTime, float gameVel) {
	if (world.getComponent<CaffeineTransformComponent>(background).position.x < -virtualWidth / 2) {
		world.getComponent<CaffeineTransformComponent>(background).position = glm::vec2(virtualWidth / 2 + virtualWidth -10.0f, virtualHeight / 2);
	}
	if (world.getComponent<CaffeineTransformComponent>(background2).position.x < -virtualWidth / 2) {
		world.getComponent<CaffeineTransformComponent>(background2).position = glm::vec2(virtualWidth / 2 + virtualWidth - 10.0f, virtualHeight / 2);
	}
}

void FlappyBirdGame::resetGame() {
	score = 0;
	gamePaused = false;
	world.getComponent<CaffeineTransformComponent>(bird).position = birdSpawn;
	birdVel = glm::vec2(0.0f, startBoost);
	for (PipePair*& pipePair : pipePairs) {
		pipePair->despawn();
	}
	gameVel = glm::vec2(gameVelValue, 0.0f);
	std::cout << "Game reset!" << std::endl;
}

void FlappyBirdGame::checkGameOver() {
	if (world.getComponent<CaffeineTransformComponent>(bird).position.y < 0.0f || world.getComponent<CaffeineTransformComponent>(bird).position.y > virtualHeight) {
		gamePaused = true;
		birdIsDying = true;
	}
}

void FlappyBirdGame::birdDying(float deltaTime) {
	if (world.getComponent<CaffeineTransformComponent>(bird).rotation < 90.0f) {
		world.getComponent<CaffeineTransformComponent>(bird).rotation += 200.0f * deltaTime;
	}
	else {
		world.getComponent<CaffeineTransformComponent>(bird).rotation += 100.0f * deltaTime;
	}
	if (world.getComponent<CaffeineTransformComponent>(bird).position.y < 0.0f) {
		birdIsDying = false;
	}
	birdVel.y += gravity * deltaTime;
	//bird->translate(birdVel * deltaTime); nicht nützlich
}

void FlappyBirdGame::birdCollisionCallback(CaffeineEntity thisEntity, CaffeineEntity otherEntity) {
	gamePaused = true;
	birdIsDying = true;
}

void FlappyBirdGame::rotateBird() {
	float rotation = std::abs(birdVel.y) / boost * 10.0f;
	(birdVel.y >= 0) ? world.getComponent<CaffeineTransformComponent>(bird).rotation = rotation : world.getComponent<CaffeineTransformComponent>(bird).rotation = -rotation;
}

void FlappyBirdGame::render() {
	CaffeineRenderingSystem::update(world);
}