#include "Engine.h"
#include "Shader.h"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <SDL.h>
#include <box2d/box2d.h>
#include "SDL_gamecontroller.h"
#include "glad/glad.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Input input;

static GLuint LoadTexture(const std::string& filePath) {
	SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
	if (!surface) {
		std::cerr << "Error loading image: " << filePath << std::endl;
		return 0;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	return texture;
}

SDL_Window* window = nullptr;
SDL_GLContext glContext = nullptr;
Shader* shader = nullptr;

//box2d setup
b2Vec2 gravity = { 0.0f, 0.0f };
b2WorldDef worldDef = b2DefaultWorldDef();
b2WorldId worldId = b2CreateWorld(&worldDef);

float timeStep = 1.0f / 60.0f;
int subStepCount = 2;


InputEnum Input::mapSDLKeyToInputEnum(SDL_Keycode key) {
	switch (key) {
	case SDLK_w: return InputEnum::North;
	case SDLK_s: return InputEnum::South;
	case SDLK_a: return InputEnum::West;
	case SDLK_d: return InputEnum::East;
	case SDLK_UP: return InputEnum::DNorth;
	case SDLK_DOWN: return InputEnum::DSouth;
	case SDLK_LEFT: return InputEnum::DWest;
	case SDLK_RIGHT: return InputEnum::DEast;
		// Add more mappings as needed
	default: return InputEnum::LeftThumbstick; // Default case
	}
}

InputEnum Input::mapSDLButtonToInputEnum(Uint8 button) {
	switch (button) {
	case SDL_CONTROLLER_BUTTON_A: return InputEnum::ButtonA;
	case SDL_CONTROLLER_BUTTON_B: return InputEnum::ButtonB;
	case SDL_CONTROLLER_BUTTON_X: return InputEnum::ButtonX;
	case SDL_CONTROLLER_BUTTON_Y: return InputEnum::ButtonY;
		// Add more mappings as needed
	default: return InputEnum::LeftThumbstick; // Default case
	}
}

InputEnum Input::getKeyPressed() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			return mapSDLKeyToInputEnum(event.key.keysym.sym);
		}
		if (event.type == SDL_CONTROLLERBUTTONDOWN) {
			return mapSDLButtonToInputEnum(event.cbutton.button);
		}
	}
	return inputEnum; // Return the current inputEnum if no key is pressed
}

void Input::setGameController(SDL_GameController* controller) {
	gameController = controller;
}

bool Input::IsGamepadButtonPressed(GamepadButton button, bool singleClick) {
	if (!gameController) return false;

	SDL_GameControllerButton sdlButton;
	switch (button) {
	case GamepadButton::A: sdlButton = SDL_CONTROLLER_BUTTON_A; break;
	case GamepadButton::B: sdlButton = SDL_CONTROLLER_BUTTON_B; break;
	case GamepadButton::X: sdlButton = SDL_CONTROLLER_BUTTON_X; break;
	case GamepadButton::Y: sdlButton = SDL_CONTROLLER_BUTTON_Y; break;
	case GamepadButton::DPadLeft: sdlButton = SDL_CONTROLLER_BUTTON_DPAD_LEFT; break;
	case GamepadButton::DPadRight: sdlButton = SDL_CONTROLLER_BUTTON_DPAD_RIGHT; break;
	case GamepadButton::DPadUp: sdlButton = SDL_CONTROLLER_BUTTON_DPAD_UP; break;
	case GamepadButton::DPadDown: sdlButton = SDL_CONTROLLER_BUTTON_DPAD_DOWN; break;
	default: return false;
	}

	static std::map<GamepadButton, bool> buttonState;
	bool isPressed = SDL_GameControllerGetButton(gameController, sdlButton) != 0;

	if (singleClick) {
		if (isPressed && !buttonState[button]) {
			buttonState[button] = true;
			return true;
		}
		if (!isPressed) {
			buttonState[button] = false;
		}
		return false;
	}
	return isPressed;
}

namespace GameEngine {

	void Engine::Initialize(GameWindow windowSettings) {
		// Set Gravity
		worldDef.gravity = gravity;

		windowDisplay = windowSettings;
		SDL_GameController* controller;
		int i;

		// Initialize OpenGL
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


		SDL_Init(SDL_INIT_VIDEO);
		SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

		for (i = 0; i < SDL_NumJoysticks(); ++i) {
			if (SDL_IsGameController(i)) {
				char* mapping;
				std::cout << "Index '" << i << "' is a compatible controller, named '" << SDL_GameControllerNameForIndex(i) << "'" << std::endl;
				controller = SDL_GameControllerOpen(i);
				input.setGameController(controller);
				mapping = SDL_GameControllerMapping(controller);
				std::cout << "Controller " << i << " is mapped as \"" << mapping << std::endl;
				SDL_free(mapping);
			}
			else {
				std::cout << "Index '" << i << "' is not a compatible controller." << std::endl;
			}
		}

		window = SDL_CreateWindow(windowSettings.windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSettings.windowWidth, windowSettings.windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (!window) {
			std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
			return;
		}

		glContext = SDL_GL_CreateContext(window);
		if (!glContext) {
			std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
			return;
		}

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			std::cerr << "Failed to initialize GLAD" << std::endl;
			return;
		}

		SDL_GL_SetSwapInterval(1); // Enable vsync

		glViewport(0, 0, windowSettings.windowWidth, windowSettings.windowHeight);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Create shader here
		shader = new Shader("texture.shader");

		b2World_EnableContinuous(worldId, true);

		Update();
	}

	void Engine::Update() {
		int prevTime = 0;
		int currentTime = 0;
		bool isRunning = true;
		SDL_Event event;

		shader->Bind();

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowDisplay.windowWidth), static_cast<float>(windowDisplay.windowHeight), 0.0f, -1.0f, 1.0f);
		shader->SetUniformMat4f("projection", projection);

		while (isRunning) {
			prevTime = currentTime;
			currentTime = SDL_GetTicks();
			deltaTime = (currentTime - prevTime) / 1000.0f;

			glClear(GL_COLOR_BUFFER_BIT);

			for (int i = 0; i < getLevel().background.size(); ++i) {
				if (getLevel().background[i].scrollingDirection == getLevel().background[i].vertical) {
					getLevel().background[i].scrollRect.h += getLevel().background[i].scrollingSpeed * deltaTime;
					if (getLevel().background[i].scrollingSpeed > 0)
						getLevel().background[i].scrollRect.h2 = getLevel().background[i].scrollRect.h - windowDisplay.windowHeight;
					else if (getLevel().background[i].scrollingSpeed < 0)
						getLevel().background[i].scrollRect.h2 = getLevel().background[i].scrollRect.h + windowDisplay.windowHeight;
				}
				else {
					getLevel().background[i].scrollRect.w += getLevel().background[i].scrollingSpeed * deltaTime;
					if (getLevel().background[i].scrollingSpeed > 0)
						getLevel().background[i].scrollRect.w2 = getLevel().background[i].scrollRect.w - windowDisplay.windowWidth;
					else if (getLevel().background[i].scrollingSpeed < 0)
						getLevel().background[i].scrollRect.w2 = getLevel().background[i].scrollRect.w + windowDisplay.windowWidth;
				}
			}

			// Render background layers
			for (int i = 0; i < getLevel().background.size(); ++i) {
				GLuint backgroundTexture = LoadTexture(getLevel().background[i].background_path);

				glBindTexture(GL_TEXTURE_2D, backgroundTexture);

				// Set up vertex data (and buffer(s)) and configure vertex attributes
				float vertices[] = {
					// positions          // texture coords
					1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
					1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
					-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom left
					-1.0f,  1.0f, 0.0f,  0.0f, 1.0f  // top left 
				};
				unsigned int indices[] = {
					0, 1, 3, // first triangle
					1, 2, 3  // second triangle
				};

				unsigned int VBO, VAO, EBO;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);

				glBindVertexArray(VAO);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

				// position attribute
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				// texture coord attribute
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				// Render the texture
				glBindTexture(GL_TEXTURE_2D, backgroundTexture);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				glDeleteVertexArrays(1, &VAO);
				glDeleteBuffers(1, &VBO);
				glDeleteBuffers(1, &EBO);
				glDeleteTextures(1, &backgroundTexture);
			}

			// Delete GameObjects
			for (int i = getLevel().levelObjects.size() - 1; i >= 0; --i) {
				if (getLevel().levelObjects[i]->toBeDeleted == true) {
					getLevel().levelObjects[i]->OnDestroyed();

					if (i > 1) {
						b2DestroyBody(*getLevel().levelObjects[i]->bodyId);
						delete getLevel().levelObjects[i]->bodyDef;
						delete getLevel().levelObjects[i]->bodyId;
					}
					else {
						std::cout << "Spawner delete" << i << std::endl;
					}
					delete getLevel().levelObjects[i];
					getLevel().levelObjects.erase(getLevel().levelObjects.begin() + i);
				}
			}

			for (int i = getLevel().levelObjects.size() - 1; i >= 0; --i) {
				auto obj = getLevel().levelObjects[i];
				if (obj->bodyId != nullptr) {
					b2DestroyBody(*obj->bodyId);
				}
			}

			// Manage Created Objects
			for (int i = 0; i < getLevel().levelObjects.size(); ++i) {
				GameObject* obj = getLevel().levelObjects[i];

				obj->OnUpdate();

				Animation* spriteAnimation = &obj->animation;
				if (i > 1) {
					float bodyWidth = getLevel().levelObjects[i]->collisionBoxSize.w;
					float bodyHeight = getLevel().levelObjects[i]->collisionBoxSize.h;
					bodyWidth = bodyWidth / 2.0f;
					bodyHeight = bodyHeight / 2.0f;

					b2BodyDef* bodyDef = new b2BodyDef;
					*bodyDef = b2DefaultBodyDef();
					bodyDef->type = b2_dynamicBody;
					bodyDef->position = { getLevel().levelObjects[i]->position.x, getLevel().levelObjects[i]->position.y };
					bodyDef->userData = getLevel().levelObjects[i];

					b2BodyId* bodyId = new b2BodyId;
					*bodyId = b2CreateBody(worldId, bodyDef);

					b2Vec2 bodyCenter{ bodyWidth, bodyHeight };
					float angle = 4.0f;

					b2Polygon* dynamicBox = new b2Polygon;
					*dynamicBox = b2MakeOffsetBox(bodyWidth, bodyHeight, bodyCenter, b2MakeRot(angle * b2_pi));

					b2ShapeDef* shapeDef = new b2ShapeDef;
					*shapeDef = b2DefaultShapeDef();
					shapeDef->density = 1.0f;
					shapeDef->friction = 0.3f;
					shapeDef->userData = getLevel().levelObjects[i];
					shapeDef->enableContactEvents = true;

					b2ShapeId* shapeId = new b2ShapeId;
					*shapeId = b2CreatePolygonShape(*bodyId, shapeDef, dynamicBox);

					getLevel().levelObjects[i]->bodyId = bodyId;
					getLevel().levelObjects[i]->bodyDef = bodyDef;
					getLevel().levelObjects[i]->shapeId = shapeId;
					getLevel().levelObjects[i]->shapeDef = shapeDef;
					getLevel().levelObjects[i]->boxCollision = dynamicBox;
				}

				if (spriteAnimation->tilemapPath != "") {
					GLuint spriteTexture = LoadTexture(spriteAnimation->tilemapPath);

					glBindTexture(GL_TEXTURE_2D, spriteTexture);

					// Set up vertex data (and buffer(s)) and configure vertex attributes
					float vertices[] = {
						// positions          // texture coords
						1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
						1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
						-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom left
						-1.0f,  1.0f, 0.0f,  0.0f, 1.0f  // top left 
					};
					unsigned int indices[] = {
						0, 1, 3, // first triangle
						1, 2, 3  // second triangle
					};

					unsigned int VBO, VAO, EBO;
					glGenVertexArrays(1, &VAO);
					glGenBuffers(1, &VBO);
					glGenBuffers(1, &EBO);

					glBindVertexArray(VAO);

					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

					// position attribute
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);
					// texture coord attribute
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
					glEnableVertexAttribArray(1);

					// Render the texture
					glBindTexture(GL_TEXTURE_2D, spriteTexture);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

					glDeleteVertexArrays(1, &VAO);
					glDeleteBuffers(1, &VBO);
					glDeleteBuffers(1, &EBO);
					glDeleteTextures(1, &spriteTexture);
				}
			}

			b2World_Step(worldId, timeStep, subStepCount);
			contactListener();

			SDL_GL_SwapWindow(window);

			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) {
					isRunning = false;
				}
			}
		}

		SDL_DestroyWindow(window);
		SDL_GL_DeleteContext(glContext);

		window = nullptr;

		b2DestroyWorld(worldId);
		worldId = b2_nullWorldId;

		SDL_Quit();
	}

	void Engine::setLevel(GameLevel level)
	{
		mainLevel = level;
	}

	void Engine::print(std::string printText)
	{
		std::cout << printText << std::endl;
	}

	GameLevel& Engine::getLevel()
	{
		return mainLevel;
	}

	void Engine::sensorListener()
	{
		b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);

		for (int i = 0; i < sensorEvents.beginCount; ++i)
		{
			b2SensorBeginTouchEvent* beginTouch = sensorEvents.beginEvents + i;
			void* myUserData = b2Shape_GetUserData(beginTouch->visitorShapeId);
			if (myUserData)
			{
				GameObject* m = static_cast<GameObject*>(myUserData);

				void* myUserData2 = b2Shape_GetUserData(beginTouch->sensorShapeId);
				std::cout << "Sensor detected collision with object group: " << m->objectGroup << std::endl;

				if (myUserData2)
				{
					GameObject* m2 = static_cast<GameObject*>(myUserData2);
					m->OnCollideEnter(*m2);
					if (m2->objectGroup == "player")
					{
						std::cout << "Sensor detected collision with object group: " << m2->objectGroup << std::endl;
					}
				}
			}
		}
	}

	void Engine::contactListener() {
		b2ContactEvents contactEvents = b2World_GetContactEvents(worldId);

		if (contactEvents.beginCount > 0) {
			//std::cout << "Contact Events Begin Count: " << contactEvents.beginCount << std::endl;
		}

		for (int i = 0; i < contactEvents.beginCount; ++i)
		{
			b2ContactBeginTouchEvent* beginTouch = contactEvents.beginEvents + i;
			void* myUserData = b2Shape_GetUserData(beginTouch->shapeIdA);
			if (myUserData)
			{
				GameObject* m = static_cast<GameObject*>(myUserData);
				//std::cout << m->objectGroup << std::endl;
				void* myUserData2 = b2Shape_GetUserData(beginTouch->shapeIdB);
				//std::cout << "Collision A: " << m->objectGroup << " " << m->collisionBoxSize.w << " " << m->collisionBoxSize.h;
				
				if (myUserData2)
				{
					GameObject* m2 = static_cast<GameObject*>(myUserData2);
					m->OnCollideEnter(*m2);
					//std::cout << " Collision B: " << m2->objectGroup << " " << m2->collisionBoxSize.w << " " << m2->collisionBoxSize.h << std::endl;
				}
			}
		}
	}
}

void GameLevel::setLayerSize(int layerSize)
{
	background.resize(layerSize);
}

void GameObject::Destroy()
{
	toBeDeleted = true;
}

void GameLevel::addObject(GameObject* obj)
{
	levelObjects.push_back(obj);
	obj->OnStart();
}

int Animation::GetSpriteWidth()
{
	int ret = animationRect.w / tilemapSize.w;
	return ret;
}

