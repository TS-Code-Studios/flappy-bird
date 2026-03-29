#include <game/FlappyBirdGame.hpp>
#include <game/FlappyBirdGame.hpp>

FlappyBirdGame::FlappyBirdGame(CaffeineWindow& window, CaffeineWorld& world) : window(window), world(world) {
	gameVelValue = -300.0f;
	boost = 550.0f;
	startBoost = 100.0f;
	gravity = -900.0f;
	acceleration = 20.0f;
	pipeSpawnRate = 2.0f;

	birdVel = glm::vec2(0.0f);
	gameVel = glm::vec2(0.0f);
	gameVelClouds = glm::vec2(0.0f);
	gameVelBuildings = glm::vec2(0.0f);
	gameVelBushes = glm::vec2(0.0f);


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
	CaffeineResourceManager::setResourceRoot(CaffeineResourceManager::getExecutablePath() / "resources");

	CaffeineResourceManager::createDefaultMeshes();

	CaffeineResourceManager::loadShader("shaders/default.vert", "shaders/default.frag", nullptr, "default");
	CaffeineResourceManager::loadShader("shaders/default_text.vert", "shaders/default_text.frag", nullptr, "default_text");
	
	CaffeineResourceManager::loadTexture("textures/missing_texture.png", "placeholder");
	CaffeineResourceManager::loadTexture("textures/bird.png", "bird");
	CaffeineResourceManager::loadTexture("textures/pipe.png", "pipe");
	CaffeineResourceManager::loadTexture("textures/color.png", "backgroundColor");
	CaffeineResourceManager::loadTexture("textures/clouds.png", "backgroundClouds");
	CaffeineResourceManager::loadTexture("textures/buildings.png", "backgroundBuildings");
	CaffeineResourceManager::loadTexture("textures/bushes.png", "backgroundBushes");
	CaffeineResourceManager::loadFont("fonts/test.ttf", "arial");

	bird = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(bird, {birdSpawn,  0.0f, glm::vec2(100.0f, 70.0f)}); //ort, rotation, scale
	world.addComponent<CaffeineRenderComponent>(bird, {1000, true});//visible, layer
	world.addComponent<CaffeineMeshComponent>(bird, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));//mesh
	world.addComponent<CaffeineMaterialComponent>(bird, {&CaffeineResourceManager::getShader("default"),
			&CaffeineResourceManager::getTexture("bird")});//shader, texture
	world.addComponent<CaffeineColliderComponent>(bird, {ColliderType::DYNAMIC, ColliderShape::QUAD, true,
		glm::vec2(0.0f), glm::vec2(world.getComponent<CaffeineTransformComponent>(bird).size.x, world.getComponent<CaffeineTransformComponent>(bird).size.y), 
		[this] (CaffeineEntity thisEntity, CaffeineEntity otherEntity) {birdCollisionCallback(thisEntity, otherEntity);}});//collidertype, colidershape, enabled, offset, size, callback
	world.addComponent<CaffeineVelocityComponent>(bird, CaffeineVelocityComponent(&birdVel));//velocitywert

	backgroundColor = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(backgroundColor, {glm::vec2(virtualWidth / 2, virtualHeight / 2),  0.0f, glm::vec2(virtualWidth, virtualHeight)}); 
	world.addComponent<CaffeineRenderComponent>(backgroundColor, {-1000, true});
	world.addComponent<CaffeineMeshComponent>(backgroundColor, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));
	world.addComponent<CaffeineMaterialComponent>(backgroundColor, {&CaffeineResourceManager::getShader("default"),
			&CaffeineResourceManager::getTexture("backgroundColor")});

	backgroundClouds = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(backgroundClouds, {glm::vec2(virtualWidth / 2, virtualHeight / 2),  0.0f, glm::vec2(virtualWidth, virtualHeight)});
	world.addComponent<CaffeineRenderComponent>(backgroundClouds, {-999, true});
	world.addComponent<CaffeineMeshComponent>(backgroundClouds, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));
	world.addComponent<CaffeineMaterialComponent>(backgroundClouds, {&CaffeineResourceManager::getShader("default"),
			&CaffeineResourceManager::getTexture("backgroundClouds")});
	world.addComponent<CaffeineVelocityComponent>(backgroundClouds, CaffeineVelocityComponent(&gameVelClouds));
	backgroundEntities.push_back(backgroundClouds);

	backgroundClouds2 = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(backgroundClouds2, {glm::vec2(virtualWidth / 2 + virtualWidth - 10.0f, virtualHeight / 2),  0.0f, glm::vec2(virtualWidth, virtualHeight)});
	world.addComponent<CaffeineRenderComponent>(backgroundClouds2, {-999, true});
	world.addComponent<CaffeineMeshComponent>(backgroundClouds2, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));
	world.addComponent<CaffeineMaterialComponent>(backgroundClouds2, {&CaffeineResourceManager::getShader("default"),
			&CaffeineResourceManager::getTexture("backgroundClouds")});
	world.addComponent<CaffeineVelocityComponent>(backgroundClouds2, CaffeineVelocityComponent(&gameVelClouds));
	backgroundEntities.push_back(backgroundClouds2);

	backgroundBuildings = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(backgroundBuildings, {glm::vec2(virtualWidth / 2, virtualHeight / 2),  0.0f, glm::vec2(virtualWidth, virtualHeight)}); 
	world.addComponent<CaffeineRenderComponent>(backgroundBuildings, {-998, true});
	world.addComponent<CaffeineMeshComponent>(backgroundBuildings, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));
	world.addComponent<CaffeineMaterialComponent>(backgroundBuildings, {&CaffeineResourceManager::getShader("default"),
			&CaffeineResourceManager::getTexture("backgroundBuildings")});
	world.addComponent<CaffeineVelocityComponent>(backgroundBuildings, CaffeineVelocityComponent(&gameVelBuildings));
	backgroundEntities.push_back(backgroundBuildings);

	backgroundBuildings2 = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(backgroundBuildings2, {glm::vec2(virtualWidth / 2 + virtualWidth - 10.0f, virtualHeight / 2),  0.0f, glm::vec2(virtualWidth, virtualHeight)});
	world.addComponent<CaffeineRenderComponent>(backgroundBuildings2, {-998, true});
	world.addComponent<CaffeineMeshComponent>(backgroundBuildings2, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));
	world.addComponent<CaffeineMaterialComponent>(backgroundBuildings2, {&CaffeineResourceManager::getShader("default"),
			&CaffeineResourceManager::getTexture("backgroundBuildings")});
	world.addComponent<CaffeineVelocityComponent>(backgroundBuildings2, CaffeineVelocityComponent(&gameVelBuildings));
	backgroundEntities.push_back(backgroundBuildings2);

	backgroundBushes = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(backgroundBushes, {glm::vec2(virtualWidth / 2, virtualHeight / 2),  0.0f, glm::vec2(virtualWidth, virtualHeight)});
	world.addComponent<CaffeineRenderComponent>(backgroundBushes, {-997, true});
	world.addComponent<CaffeineMeshComponent>(backgroundBushes, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));
	world.addComponent<CaffeineMaterialComponent>(backgroundBushes, {&CaffeineResourceManager::getShader("default"),
			&CaffeineResourceManager::getTexture("backgroundBushes")});
	world.addComponent<CaffeineVelocityComponent>(backgroundBushes, CaffeineVelocityComponent(&gameVelBushes));
	backgroundEntities.push_back(backgroundBushes);

	backgroundBushes2 = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(backgroundBushes2, {glm::vec2(virtualWidth / 2 + virtualWidth - 10.0f, virtualHeight / 2),  0.0f, glm::vec2(virtualWidth, virtualHeight)});
	world.addComponent<CaffeineRenderComponent>(backgroundBushes2, {-997, true});
	world.addComponent<CaffeineMeshComponent>(backgroundBushes2, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));
	world.addComponent<CaffeineMaterialComponent>(backgroundBushes2, {&CaffeineResourceManager::getShader("default"),
			&CaffeineResourceManager::getTexture("backgroundBushes")});
	world.addComponent<CaffeineVelocityComponent>(backgroundBushes2, CaffeineVelocityComponent(&gameVelBushes));
	backgroundEntities.push_back(backgroundBushes2);

	scoreText = world.createEntity();
	world.addComponent<CaffeineTransformComponent>(scoreText, {glm::vec2(virtualWidth - 200.0f, virtualHeight - 100.0f),  0.0f, glm::vec2(1.0f)});
	world.addComponent<CaffeineRenderComponent>(scoreText, {1001, true});
	world.addComponent<CaffeineMeshComponent>(scoreText, CaffeineMeshComponent(&CaffeineResourceManager::getMesh("quad")));
	world.addComponent<CaffeineTextComponent>(scoreText, {"score:  0", &CaffeineResourceManager::getFont("arial"), &CaffeineResourceManager::getShader("default_text"), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)});

	for(int i = 0; i < std::size(pipePairs); i++) {
		pipePairs[i] = new PipePair(world, gameVel);
		pipePairs[i]->world.addComponent<CaffeineVelocityComponent>(pipePairs[i]->bottomPipe, CaffeineVelocityComponent(&gameVel));
		pipePairs[i]->world.addComponent<CaffeineVelocityComponent>(pipePairs[i]->topPipe, CaffeineVelocityComponent(&gameVel));
	}
}

void FlappyBirdGame::update(const float  deltaTime) {
	processInput();
	gameVelClouds.x = gameVel.x * 0.5f;
	gameVelBuildings.x = gameVel.x * 0.75f;
	gameVelBushes.x = gameVel.x * 1.0f;
	if (!gamePaused) {
		gameVel.x = gameVelValue - (score * acceleration);
		gameVelFactor = gameVel.x / gameVelValue;
		CaffeineCollisionSystem::update(world);
		
		world.getComponent<CaffeineTextComponent>(scoreText).text = "score:  " + std::to_string(score);
		movePipes(deltaTime);
		spawnPipe();
		despawnPipe();
		moveBackground();
		
		rotateBird();
		birdVel.y += gravity * deltaTime * gameVelFactor;
		//world.getComponent<CaffeineTransformComponent>(bird).position += birdVel * deltaTime;//nicht nützlich
	}
	

	if (gamePaused && birdIsDying) {birdDying(deltaTime);}
	checkGameOver();
	render(deltaTime);
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
	if (static_cast<float>(glfwGetTime()) - lastPipeSpawnTime > pipeSpawnRate / gameVelFactor) {
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

void FlappyBirdGame::moveBackground() {
	for (CaffeineEntity& backgroundEntity : backgroundEntities) {
		if (world.getComponent<CaffeineTransformComponent>(backgroundEntity).position.x < -virtualWidth / 2) {
			world.getComponent<CaffeineTransformComponent>(backgroundEntity).position = glm::vec2(virtualWidth / 2 + virtualWidth - 50.0f, virtualHeight / 2);
		}
	}
}

void FlappyBirdGame::resetGame() {
	score = 0;
	gamePaused = false;
	world.getComponent<CaffeineTransformComponent>(bird).position = birdSpawn;
	birdVel = glm::vec2(0.0f, startBoost);

	world.getComponent<CaffeineTransformComponent>(backgroundColor).position = glm::vec2(virtualWidth / 2, virtualHeight / 2);
	world.getComponent<CaffeineTransformComponent>(backgroundClouds2).position = glm::vec2(virtualWidth / 2 + virtualWidth - 10.0f, virtualHeight / 2);
	world.getComponent<CaffeineTransformComponent>(backgroundClouds).position = glm::vec2(virtualWidth / 2, virtualHeight / 2);
	world.getComponent<CaffeineTransformComponent>(backgroundBuildings2).position = glm::vec2(virtualWidth / 2 + virtualWidth - 10.0f, virtualHeight / 2);
	world.getComponent<CaffeineTransformComponent>(backgroundBuildings).position = glm::vec2(virtualWidth / 2, virtualHeight / 2);
	world.getComponent<CaffeineTransformComponent>(backgroundBushes2).position = glm::vec2(virtualWidth / 2 + virtualWidth - 10.0f, virtualHeight / 2);
	world.getComponent<CaffeineTransformComponent>(backgroundBushes).position = glm::vec2(virtualWidth / 2, virtualHeight / 2);
	
	for (PipePair*& pipePair : pipePairs) {
		pipePair->despawn();
	}
	gameVel = glm::vec2(gameVelValue, 0.0f);
	std::cout << "Game reset!" << std::endl;
}

void FlappyBirdGame::checkGameOver() {
	if (world.getComponent<CaffeineTransformComponent>(bird).position.y < 0.0f || world.getComponent<CaffeineTransformComponent>(bird).position.y > virtualHeight) {
		gameOver();
	}
}

void FlappyBirdGame::birdCollisionCallback(CaffeineEntity thisEntity, CaffeineEntity otherEntity) {
    birdVel.y += boost / 2;
	gameOver();
}

void FlappyBirdGame::gameOver() {
    gamePaused = true;
    birdIsDying = true;
    gameVel = glm::vec2(0.0f);
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

void FlappyBirdGame::rotateBird() {
	float rotation = std::abs(birdVel.y) / boost * 10.0f;
	(birdVel.y >= 0) ? world.getComponent<CaffeineTransformComponent>(bird).rotation = rotation : world.getComponent<CaffeineTransformComponent>(bird).rotation = -rotation;
}

void FlappyBirdGame::render(float deltaTime) {
	CaffeineRenderingSystem::update(world);
	CaffeineTextRenderingSystem::update(world);
	CaffeineVelocitySystem::update(world, deltaTime);
}