#include "Engine.h"

#include <cstdint>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <SDL.h>
#include <box2d/box2d.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


#include "SDL_gamecontroller.h"
#include "stb_image.h"


//SDL_Renderer* SDL_CreateRenderer(SDL_Window* window, int index, Uint32 flags);
//SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);

Input input;

static SDL_Surface* OptimizedSurface(std::string filePath, SDL_Surface* windowSurface) {
	SDL_Surface* optimizedSurface = nullptr;
	SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());

	if (surface == nullptr) {
		std::cout << "Error loading image: " << filePath << std::endl;
	}
	else {
		optimizedSurface = SDL_ConvertSurface(surface, windowSurface->format, 0);
		if (optimizedSurface == nullptr) {
			std::cout << "Error optimizing surface: " << filePath << std::endl;
		}
		SDL_FreeSurface(surface);
		return optimizedSurface;
	}

}

SDL_Texture* windowSurface = nullptr;
unsigned char* background = nullptr;
SDL_Renderer* renderTarget = nullptr;
SDL_Window* window = nullptr;

//box2d setup
b2Vec2 gravity = { 0.0f, 0.0f };
b2WorldDef worldDef = b2DefaultWorldDef();
b2WorldId worldId = b2CreateWorld(&worldDef);

float timeStep = 1.0f / 60.0f;
int subStepCount = 2;
// int32 velocityIterations = 8;
// int32 positionIterations = 3;

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

	GLuint m_Texture;
	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_ebo;
	GLuint m_ShaderProgram;
	glm::vec2 m_Scale2D = glm::vec2(1.f, 1.f);
	glm::vec3 m_Position2D = glm::vec3(0.0f, 0.0f, 1.f);

	unsigned int m_Indices[] = {  // note that we start from 0!
					0, 1, 3,   // first triangle
					1, 2, 3    // second triangle
	};

	float m_Vertices[] = {
		// positions         // colors           // texture coords
		0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f, 1.f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f , 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.f    // top left
	};


	void Engine::Update()
	{
		int prevTime = 0;
		int currentTime = 0;
		bool isRunning = true;
		SDL_Event event;
		bool swap = false;

		while (isRunning) {
			prevTime = currentTime;
			currentTime = SDL_GetTicks();
			deltaTime = (currentTime - prevTime) / 1000.0f;

			for (int i = 0; i < getLevel().background.size(); ++i)
			{
				getLevel().background[i]->OnUpdate();

			}


			glClearColor(0.0f, 1.0f, 1.0f, 1.0f); // Cyan Blue

			glClear(GL_COLOR_BUFFER_BIT);

			//Multiple background layers
			for (auto i = getLevel().background.begin(); i != getLevel().background.end(); ++i)
			{
				if (!(*i)->isTiled)
				{
					if (!(*i)->isInit)
					{
						std::cout << "shader program is null\n" << std::endl;

						glGenBuffers(1, &(*i)->m_vbo); // Generate 1 buffer

						glGenBuffers(1, &(*i)->m_ebo);

						glGenVertexArrays(1, &(*i)->m_vao);

						// 1. bind Vertex Array Object
						glBindVertexArray((*i)->m_vao);

						// 2. copy our vertices array in a buffer for OpenGL to use
						glBindBuffer(GL_ARRAY_BUFFER, (*i)->m_vbo);
						glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*i)->m_ebo);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices), m_Indices, GL_STATIC_DRAW);

						// Vertex Shader

						const char* vertexShaderSource = R"glsl(
				#version 330 core

				in vec3 position;
				in vec3 color;
				in vec2 texCoord;

				out vec3 Color;
				out vec2 TexCoord;

				uniform mat4 model;

				void main()
				{
					Color = color;
					TexCoord = texCoord;
					gl_Position = model * vec4(position, 1.0);
				}
			)glsl";

						GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
						glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
						glCompileShader(vertexShader);

						GLint  success;
						//char infoLog[512];
						glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

						// Fragment Shader

						const char* fragmentShaderSource = R"glsl(
				#version 330 core
				in vec3 Color;
				in vec2 TexCoord;

				out vec4 outColor;

				uniform sampler2D ourTexture;

				void main()
				{
					vec4 colTex1 = texture(ourTexture, TexCoord);
					if(colTex1 == vec4(1, 0, 1, 1)) discard;

					outColor = colTex1;
				})glsl";

						GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
						glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
						glCompileShader(fragmentShader);

						glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

						if (!success)
						{
							//glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
							//std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
						}

						(*i)->m_ShaderProgram = glCreateProgram();

						glAttachShader((*i)->m_ShaderProgram, vertexShader);
						glAttachShader((*i)->m_ShaderProgram, fragmentShader);
						glLinkProgram((*i)->m_ShaderProgram);

						glDeleteShader(vertexShader);
						glDeleteShader(fragmentShader);

						glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
						if (!success) {
							//glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
							//std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
						}

						// 3. then set our vertex attributes pointers
						GLint posAttrib = glGetAttribLocation((*i)->m_ShaderProgram, "position");
						glEnableVertexAttribArray(posAttrib);
						glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

						GLint colorAttrib = glGetAttribLocation((*i)->m_ShaderProgram, "color");
						glEnableVertexAttribArray(colorAttrib);
						glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

						GLint texCoordAttrib = glGetAttribLocation((*i)->m_ShaderProgram, "texCoord");
						glEnableVertexAttribArray(texCoordAttrib);
						glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

						glGenTextures(1, &(*i)->m_Texture);
						glBindTexture(GL_TEXTURE_2D, (*i)->m_Texture);


						// set the texture wrapping/filtering options (on the currently bound texture object)
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

						stbi_set_flip_vertically_on_load(true);

						int width, height, nrChannels;
						unsigned char* data = stbi_load((*i)->background_path.c_str(), &width, &height, &nrChannels, 0);
						if (data)
						{
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
							glGenerateMipmap(GL_TEXTURE_2D);
						}
						else
						{
							std::cout << "Failed to load texture" << (*i)->background_path << std::endl;
						}
						stbi_image_free(data);

						glUseProgram((*i)->m_ShaderProgram);

						GLuint textureLocation;

						textureLocation = glGetUniformLocation((*i)->m_ShaderProgram, "ourTexture");

						glUniform1i(textureLocation, 0);

						(*i)->isInit = true;

					}

					if ((*i)->isInit)
					{
						glUseProgram((*i)->m_ShaderProgram);

						glm::mat4 model = glm::mat4(1.0f); // Identity matrix
						model = glm::translate(model, glm::vec3((*i)->scrollRect.w, (*i)->scrollRect.h, 1.0f)); // Apply translation
						model = glm::scale(model, glm::vec3((*i)->size.x, (*i)->size.y, 1.0f)); // Apply scaling

						// Pass the model matrix to the shader
						GLuint modelLoc = glGetUniformLocation((*i)->m_ShaderProgram, "model");
						glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


						glBindVertexArray((*i)->m_vao);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, (*i)->m_Texture);

						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

					}
				}
				else
				{
					if (!(*i)->isInit)
					{
						std::cout << "Initialize tiled background" << std::endl;

						float tempVertices[] = {
							// positions         // colors           // texture coords
							0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f / ((float)(*i)->tileMapSize.columns),  1.f,   // top right
							0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f / ((float)(*i)->tileMapSize.columns),  1.f - (1.f / ((float)(*i)->tileMapSize.rows)),   // bottom right
						   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,											1.f - (1.f / ((float)(*i)->tileMapSize.rows)),   // bottom left
						   -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,											1.f    // top left
						};

						std::copy(std::begin(tempVertices), std::end(tempVertices), std::begin((*i)->tiledVertices));

						// Initialize tiled background
						glGenBuffers(1, &(*i)->m_vbo); // Generate 1 buffer
						glGenBuffers(1, &(*i)->m_ebo);
						glGenVertexArrays(1, &(*i)->m_vao);

						// 1. bind Vertex Array Object
						glBindVertexArray((*i)->m_vao);

						// 2. copy our vertices array in a buffer for OpenGL to use
						glBindBuffer(GL_ARRAY_BUFFER, (*i)->m_vbo);
						glBufferData(GL_ARRAY_BUFFER, sizeof((*i)->tiledVertices), (*i)->tiledVertices, GL_STATIC_DRAW);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*i)->m_ebo);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices), m_Indices, GL_STATIC_DRAW);

						// Vertex Shader
						const char* vertexShaderSource = R"glsl(
                    #version 330 core
                    in vec3 position;
                    in vec3 color;
                    in vec2 texCoord;
                    out vec3 Color;
                    out vec2 TexCoord;
                    uniform mat4 model;
                    void main()
                    {
                        Color = color;
                        TexCoord = texCoord;
                        gl_Position = model * vec4(position, 1.0);
                    }
                )glsl";

						GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
						glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
						glCompileShader(vertexShader);

						GLint success;
						glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

						// Fragment Shader
						const char* fragmentShaderSource = R"glsl(
                    #version 330 core
                    in vec3 Color;
                    in vec2 TexCoord;
                    out vec4 outColor;
                    uniform sampler2D ourTexture;
                    void main()
                    {
                        vec4 colTex1 = texture(ourTexture, TexCoord);
                        if(colTex1 == vec4(1, 0, 1, 1)) discard;
                        outColor = colTex1;
                    }
                )glsl";

						GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
						glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
						glCompileShader(fragmentShader);

						glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

						(*i)->m_ShaderProgram = glCreateProgram();
						glAttachShader((*i)->m_ShaderProgram, vertexShader);
						glAttachShader((*i)->m_ShaderProgram, fragmentShader);
						glLinkProgram((*i)->m_ShaderProgram);

						glDeleteShader(vertexShader);
						glDeleteShader(fragmentShader);

						glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);

						// 3. then set our vertex attributes pointers
						GLint posAttrib = glGetAttribLocation((*i)->m_ShaderProgram, "position");
						glEnableVertexAttribArray(posAttrib);
						glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

						GLint colorAttrib = glGetAttribLocation((*i)->m_ShaderProgram, "color");
						glEnableVertexAttribArray(colorAttrib);
						glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

						GLint texCoordAttrib = glGetAttribLocation((*i)->m_ShaderProgram, "texCoord");
						glEnableVertexAttribArray(texCoordAttrib);
						glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

						glGenTextures(1, &(*i)->m_Texture);
						glBindTexture(GL_TEXTURE_2D, (*i)->m_Texture);

						// set the texture wrapping/filtering options (on the currently bound texture object)
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

						stbi_set_flip_vertically_on_load(true);

						int width, height, nrChannels;
						unsigned char* data = stbi_load((*i)->background_path.c_str(), &width, &height, &nrChannels, 0);
						if (data)
						{
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
							glGenerateMipmap(GL_TEXTURE_2D);
						}
						else
						{
							std::cout << "Failed to load texture" << (*i)->background_path << std::endl;
						}
						stbi_image_free(data);

						glUseProgram((*i)->m_ShaderProgram);

						GLuint textureLocation;
						textureLocation = glGetUniformLocation((*i)->m_ShaderProgram, "ourTexture");
						glUniform1i(textureLocation, 0);

						(*i)->isInit = true;
					}

					if ((*i)->isInit)
					{
						glUseProgram((*i)->m_ShaderProgram);

						for (int y = 0; y < (*i)->numTiles.y; ++y)
						{
							std::cout << "-----------------" << std::endl;

							for (int x = 0; x < (*i)->numTiles.x; ++x)
							{
								int tileIndex = y * (*i)->numTiles.x + x;
								//std::cout << "Tile Index: " << tileIndex << std::endl;
								if (tileIndex >= (*i)->tileIDs.size())
									continue;

								int tileID = (*i)->tileIDs[tileIndex];
								
								int column = tileID % (*i)->tileMapSize.columns;
								int row = tileID / (*i)->tileMapSize.columns;

								std::cout << "Tile ID: " << tileID << std::endl;
								std::cout << "Columns: " << column << std::endl;
								std::cout << "Rows: " << row << std::endl;

								float texWidth = 1.0f / (*i)->tileMapSize.columns;
								float texHeight = 1.0f / (*i)->tileMapSize.rows;

								float xTexCoord = column * texWidth;
								float yTexCoord = 1.0f - ((row + 1) * texHeight);

								// Update texture coordinates
								(*i)->tiledVertices[6] = xTexCoord + texWidth;
								(*i)->tiledVertices[7] = yTexCoord + texHeight; // Top right
								(*i)->tiledVertices[14] = xTexCoord + texWidth;
								(*i)->tiledVertices[15] = yTexCoord; // Bottom right
								(*i)->tiledVertices[22] = xTexCoord;
								(*i)->tiledVertices[23] = yTexCoord; // Bottom left
								(*i)->tiledVertices[30] = xTexCoord;
								(*i)->tiledVertices[31] = yTexCoord + texHeight; // Top left

								// Update VBO with new texture coordinates
								glBindBuffer(GL_ARRAY_BUFFER, (*i)->m_vbo);
								glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof((*i)->tiledVertices), (*i)->tiledVertices);

								glm::mat4 model = glm::mat4(1.0f); // Identity matrix
								model = glm::translate(model, glm::vec3((*i)->scrollRect.w + x * (*i)->size.x, (*i)->scrollRect.h - y * (*i)->size.y, 1.0f)); // Apply translation
								model = glm::scale(model, glm::vec3((*i)->size.x, (*i)->size.y, 1.0f)); // Apply scaling

								// Pass the model matrix to the shader
								GLuint modelLoc = glGetUniformLocation((*i)->m_ShaderProgram, "model");
								glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

								glBindVertexArray((*i)->m_vao);
								glActiveTexture(GL_TEXTURE0);
								glBindTexture(GL_TEXTURE_2D, (*i)->m_Texture);
								glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
							}
						}
					}
				}
				
			}

			// Delete GameObjects

			for (int i = getLevel().levelObjects.size() - 1; i >= 0; --i) {
				if (getLevel().levelObjects[i]->toBeDeleted == true) {
					getLevel().levelObjects[i]->OnDestroyed();
					if (getLevel().levelObjects[i]->animation->tilemapPath != "")
					{
						glUseProgram(getLevel().levelObjects[i]->m_ShaderProgram);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glActiveTexture(GL_TEXTURE0);
						glDeleteProgram(getLevel().levelObjects[i]->m_ShaderProgram);
					}

					if (getLevel().levelObjects[i]->bodyId != nullptr)
					{
						b2DestroyBody(*getLevel().levelObjects[i]->bodyId);
					}
					else
					{
						std::cout << "Object with no body" << i << std::endl;
					}
					delete getLevel().levelObjects[i];
					getLevel().levelObjects.erase(getLevel().levelObjects.begin() + i);
				}
			}

			for (int i = getLevel().levelObjects.size() - 1; i >= 0; --i)
			{
				auto obj = getLevel().levelObjects[i];
				if (obj->bodyId != nullptr)
				{
					b2DestroyBody(*obj->bodyId);
					delete obj->bodyDef;
					delete obj->bodyId;
					delete obj->boxCollision;
					delete obj->shapeId;
					delete obj->shapeDef;
				}
			}

			//Create Objects
			for (auto i = getLevel().levelObjects.begin(); i != getLevel().levelObjects.end(); ++i)
			{
				if ((*i)->animation != nullptr)
				{
					if ((*i)->animation->tilemapPath != "")
					{
						//Initialize Object
						if (!(*i)->isInit)
						{
							float tempVertices[] = {
								// positions         // colors           // texture coords
								0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f / ((float)(*i)->animation->tilemapSize.w),  1.f,   // top right
								0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f / ((float)(*i)->animation->tilemapSize.w),  1.f - (1.f / ((float)(*i)->animation->tilemapSize.h)),   // bottom right
							   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,											1.f - (1.f / ((float)(*i)->animation->tilemapSize.h)),   // bottom left
							   -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,											1.f    // top left
							};

							std::copy(std::begin(tempVertices), std::end(tempVertices), std::begin((*i)->m_Vertices));

							glGenBuffers(1, &(*i)->m_vbo); // Generate 1 buffer

							glGenBuffers(1, &(*i)->m_ebo);

							glGenVertexArrays(1, &(*i)->m_vao);

							// 1. bind Vertex Array Object
							glBindVertexArray((*i)->m_vao);

							// 2. copy our vertices array in a buffer for OpenGL to use
							glBindBuffer(GL_ARRAY_BUFFER, (*i)->m_vbo);
							glBufferData(GL_ARRAY_BUFFER, sizeof((*i)->m_Vertices), (*i)->m_Vertices, GL_STATIC_DRAW);

							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*i)->m_ebo);
							glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices), m_Indices, GL_STATIC_DRAW);



							// Vertex Shader

							const char* vertexShaderSource = R"glsl(
				#version 330 core

				in vec3 position;
				in vec3 color;
				in vec2 texCoord;

				out vec3 Color;
				out vec2 TexCoord;

				uniform mat4 model;

				void main()
				{
					Color = color;
					TexCoord = texCoord;
					gl_Position = model * vec4(position, 1.0);
				}
			)glsl";

							GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
							glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
							glCompileShader(vertexShader);

							GLint  success;
							//char infoLog[512];
							glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

							// Fragment Shader

							const char* fragmentShaderSource = R"glsl(
				#version 330 core
				in vec3 Color;
				in vec2 TexCoord;

				out vec4 outColor;

				uniform sampler2D ourTexture;

				void main()
				{
					vec4 colTex1 = texture(ourTexture, TexCoord);
					if(colTex1 == vec4(1, 0, 1, 1)) discard;

					outColor = colTex1;
				})glsl";

							GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
							glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
							glCompileShader(fragmentShader);

							glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

							if (!success)
							{
								//glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
								//std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
							}

							(*i)->m_ShaderProgram = glCreateProgram();

							glAttachShader((*i)->m_ShaderProgram, vertexShader);
							glAttachShader((*i)->m_ShaderProgram, fragmentShader);
							glLinkProgram((*i)->m_ShaderProgram);

							glDeleteShader(vertexShader);
							glDeleteShader(fragmentShader);

							glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
							if (!success) {
								//glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
								//std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
							}

							// 3. then set our vertex attributes pointers
							GLint posAttrib = glGetAttribLocation((*i)->m_ShaderProgram, "position");
							glEnableVertexAttribArray(posAttrib);
							glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

							GLint colorAttrib = glGetAttribLocation((*i)->m_ShaderProgram, "color");
							glEnableVertexAttribArray(colorAttrib);
							glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

							GLint texCoordAttrib = glGetAttribLocation((*i)->m_ShaderProgram, "texCoord");
							glEnableVertexAttribArray(texCoordAttrib);
							glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

							glGenTextures(1, &(*i)->m_Texture);
							glBindTexture(GL_TEXTURE_2D, (*i)->m_Texture);


							// set the texture wrapping/filtering options (on the currently bound texture object)
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

							stbi_set_flip_vertically_on_load(true);

							int width, height, nrChannels;
							unsigned char* data = stbi_load((*i)->animation->tilemapPath.c_str(), &width, &height, &nrChannels, 0);
							if (data)
							{
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
								glGenerateMipmap(GL_TEXTURE_2D);
							}
							else
							{
								std::cout << "Failed to load object texture" << std::endl;
							}
							stbi_image_free(data);

							glUseProgram((*i)->m_ShaderProgram);

							GLuint textureLocation;

							textureLocation = glGetUniformLocation((*i)->m_ShaderProgram, "ourTexture");

							glUniform1i(textureLocation, 0);

							(*i)->isInit = true;

						}

						//Update Object
						if ((*i)->isInit)
						{
							Animation* spriteAnimation = (*i)->animation;
							glUseProgram((*i)->m_ShaderProgram);
							if (spriteAnimation->tilemapPath != "") {

								if (spriteAnimation->manual.empty() == true)
								{
									// Increment elapsed time
									(*i)->elapsedTime += deltaTime;

									// Check if enough time has passed to advance to the next frame
									if ((*i)->elapsedTime >= spriteAnimation->frameDuration) {
										// Subtract frameTime to preserve leftover time
										(*i)->elapsedTime -= spriteAnimation->frameDuration;

										// Advance to the next frame in the animation
										int frameCount = (spriteAnimation->tilemapSize.w * spriteAnimation->tilemapSize.h - 1) + 1;
										spriteAnimation->currentFrame =
											((spriteAnimation->currentFrame + 1) % frameCount);

										// Calculate texture coordinates for the current frame
										int column = spriteAnimation->currentFrame % spriteAnimation->tilemapSize.w;
										int row = spriteAnimation->currentFrame / spriteAnimation->tilemapSize.w;

										float texWidth = 1.0f / spriteAnimation->tilemapSize.w;
										float texHeight = 1.0f / spriteAnimation->tilemapSize.h;

										float x = column * texWidth;
										float y = 1.0f - ((row + 1) * texHeight);

										// Update texture coordinates
										(*i)->m_Vertices[6] = x + texWidth; (*i)->m_Vertices[7] = y + texHeight; // Top right
										(*i)->m_Vertices[14] = x + texWidth; (*i)->m_Vertices[15] = y;           // Bottom right
										(*i)->m_Vertices[22] = x;            (*i)->m_Vertices[23] = y;           // Bottom left
										(*i)->m_Vertices[30] = x;            (*i)->m_Vertices[31] = y + texHeight; // Top left

										if (spriteAnimation->currentFrame == spriteAnimation->tilemapSize.h - 1)
										{
											(*i)->OnAnimationFinish();
										}

										// Update VBO with new texture coordinates
										glBindBuffer(GL_ARRAY_BUFFER, (*i)->m_vbo);
										glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 32, (*i)->m_Vertices);
									}
								}
								if (spriteAnimation->manual.empty() == false)
								{
									// Increment elapsed time
									(*i)->elapsedTime += deltaTime;

									// Check if enough time has passed to advance to the next frame
									if ((*i)->elapsedTime >= spriteAnimation->frameDuration) {
										// Subtract frameTime to preserve leftover time
										(*i)->elapsedTime -= spriteAnimation->frameDuration;

										// Advance to the next frame in the animation
										spriteAnimation->currentFrame = spriteAnimation->manual[spriteAnimation->targetFrame];
										if (spriteAnimation->targetFrame < (spriteAnimation->manual.size() - 1))
										{
											spriteAnimation->targetFrame++;
										}
										else
										{
											if (spriteAnimation->loop)
											{
												spriteAnimation->targetFrame = 0;
											}
											(*i)->OnAnimationFinish();
										}
										// Calculate texture coordinates for the current frame
										int column = spriteAnimation->currentFrame % spriteAnimation->tilemapSize.w;
										int row = spriteAnimation->currentFrame / spriteAnimation->tilemapSize.w;

										float texWidth = 1.0f / spriteAnimation->tilemapSize.w;
										float texHeight = 1.0f / spriteAnimation->tilemapSize.h;

										float x = column * texWidth;
										float y = 1.0f - ((row + 1) * texHeight);

										// Update texture coordinates
										(*i)->m_Vertices[6] = x + texWidth; (*i)->m_Vertices[7] = y + texHeight; // Top right
										(*i)->m_Vertices[14] = x + texWidth; (*i)->m_Vertices[15] = y;           // Bottom right
										(*i)->m_Vertices[22] = x;            (*i)->m_Vertices[23] = y;           // Bottom left
										(*i)->m_Vertices[30] = x;            (*i)->m_Vertices[31] = y + texHeight; // Top left

										// Update VBO with new texture coordinates
										glBindBuffer(GL_ARRAY_BUFFER, (*i)->m_vbo);
										glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 32, (*i)->m_Vertices);
									}
								}

							}



							glm::mat4 model = glm::mat4(1.0f); // Identity matrix
							model = glm::translate(model, glm::vec3((*i)->position.x / 320.f, (*i)->position.y / 240.f, 1.0f)); // Apply translation
							model = glm::scale(model, glm::vec3((*i)->collisionBoxSize.w / 250.f, (*i)->collisionBoxSize.h / 250.f, 1.0f)); // Apply scaling

							// Pass the model matrix to the shader
							GLuint modelLoc = glGetUniformLocation((*i)->m_ShaderProgram, "model");
							//GLint projectionLoc = glGetUniformLocation((*i)->m_ShaderProgram, "projection");
							glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


							glBindVertexArray((*i)->m_vao);

							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, (*i)->m_Texture);

							glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

							glUseProgram(0);
						}
					}
				}
				
			}

			//Manage Created Objects
			for (int i = 0; i < getLevel().levelObjects.size(); ++i) {
				GameObject* obj = getLevel().levelObjects[i];

				obj->OnUpdate();

				Animation* spriteAnimation = obj->animation;


				//THIS IS TO IGNORE SPAWNERS. THE FIRST TWO OBJECTS IN THE LEVEL OBJECTS VECTOR ARE SPAWNERS
					//This is a just a workaround for now. I will implement a better way to handle this later, because i need to create
					//a bool variable for objects for the user to want or not a box2d body but right now i dont have time for that.

				if (getLevel().levelObjects[i]->hasBox2d)
				{
					float bodyWidth;// = getLevel().levelObjects[i]->collisionBoxSize.w;
					float bodyHeight;// = getLevel().levelObjects[i]->collisionBoxSize.h;
					bodyWidth = getLevel().levelObjects[i]->collisionBoxSize.w / 2.0f;
					bodyHeight = getLevel().levelObjects[i]->collisionBoxSize.h / 2.0f;


					b2BodyDef* bodyDef = new b2BodyDef;
					*bodyDef = b2DefaultBodyDef();
					bodyDef->type = b2_dynamicBody;
					bodyDef->position = { getLevel().levelObjects[i]->position.x, getLevel().levelObjects[i]->position.y };
					//bodyDef-> = getLevel().levelObjects[i]->isBullet;
					bodyDef->userData = getLevel().levelObjects[i];


					b2BodyId* bodyId = new b2BodyId;
					*bodyId = b2CreateBody(worldId, bodyDef);

					b2Vec2 bodyCenter{ bodyWidth, bodyHeight };
					float angle = 4.0f;

					b2Polygon* dynamicBox = new b2Polygon;
					//*dynamicBox = b2MakeBox(bodyWidth, bodyHeight);
					*dynamicBox = b2MakeOffsetBox(bodyWidth, bodyHeight, bodyCenter, b2MakeRot(angle * b2_pi));

					b2ShapeDef* shapeDef = new b2ShapeDef;
					*shapeDef = b2DefaultShapeDef();
					shapeDef->density = 1.0f;
					shapeDef->friction = 0.3f;

					//shapeDef->enableSensorEvents = getLevel().levelObjects[i]->hasSense;

					//shapeDef->enableSensorEvents = true;
					//shapeDef->isSensor = getLevel().levelObjects[i]->hasSense;

					//shapeDef->enableContactEvents = true;

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


				b2World_Step(worldId, timeStep, subStepCount);
				contactListener();

				while (SDL_PollEvent(&event) != 0) {
					if (event.type == SDL_QUIT) {
						isRunning = false;
					}
				}
			}

			SDL_GL_SwapWindow(window);
		}
			SDL_DestroyWindow(window);
			//SDL_DestroyRenderer(renderTarget);

			window = nullptr;
			windowSurface = nullptr;
			background = nullptr;
			renderTarget = nullptr;

			b2DestroyWorld(worldId);
			worldId = b2_nullWorldId;

			SDL_Quit();
		
	}

	void Engine::Initialize(GameWindow windowSettings)
	{
		//Set Gravity
		worldDef.gravity = gravity;


		windowDisplay = windowSettings;
		SDL_GameController* controller;
		int i;

		SDL_Init(SDL_INIT_VIDEO );

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
		window = SDL_CreateWindow(windowSettings.windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSettings.windowWidth, windowSettings.windowHeight, SDL_WINDOW_OPENGL);
		//renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		// Create an OpenGL context
		SDL_GLContext m_Context = SDL_GL_CreateContext(window);
		if (!m_Context) {
			std::cout << "Failed to create OpenGL context" << std::endl;
			SDL_DestroyWindow(window);
			SDL_Quit();
			return;
		}

		// Load OpenGL functions with GLAD
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			// Print GLAD initialization log
			int error;
			glGetIntegerv(GL_MAJOR_VERSION, &error);
			std::cout << "Failed to initialize GLAD: " << error << std::endl;
			SDL_Quit();
		}
		//glEnable(GL_DEPTH_TEST);

		SDL_GL_MakeCurrent(window, m_Context);

		b2World_EnableContinuous(worldId, true);

		//Init("resources/graphics/galaxy2.bmp");
		//updateActor();

		Update();
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


// 	bool Engine::b2OverlapResultFcn(b2ShapeId id) {
// 		
// 		GameObject* obj = static_cast<GameObject*>(b2Shape_GetUserData(id));
// 		if (obj != nullptr && obj->objectGroup)
// 		{
// 			return true;
// 		}
// 		// continue the query
// 		return true;
// 	}
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

