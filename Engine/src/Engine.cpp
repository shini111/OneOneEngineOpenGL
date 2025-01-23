#include "Engine.h"
#include "Objects.h"

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

enum CharEnum {
	SPACEBAR = 0,
	EXCLAMATION = 1,
	DOUBLE_QUOTE = 2,
	HASH = 3,
	DOLLAR = 4,
	PERCENT = 5,
	AMPERSAND = 6,
	SINGLE_QUOTE = 7,
	LEFT_PAREN = 8,
	RIGHT_PAREN = 9,
	ASTERISK = 10,
	PLUS = 11,
	COMMA = 12,
	MINUS = 13,
	DOT = 14,
	SLASH = 15,
	ZERO = 16,
	ONE = 17,
	TWO = 18,
	THREE = 19,
	FOUR = 20,
	FIVE = 21,
	SIX = 22,
	SEVEN = 23,
	EIGHT = 24,
	NINE = 25,
	COLON = 26,
	SEMICOLON = 27,
	LESS_THAN = 28,
	EQUALS = 29,
	GREATER_THAN = 30,
	QUESTION = 31,
	AT = 32,
	A = 33,
	B = 34,
	C = 35,
	D = 36,
	E = 37,
	F = 38,
	G = 39,
	H = 40,
	I = 41,
	J = 42,
	K = 43,
	L = 44,
	M = 45,
	N = 46,
	O = 47,
	P = 48,
	Q = 49,
	R = 50,
	S = 51,
	T = 52,
	U = 53,
	V = 54,
	W = 55,
	X = 56,
	Y = 57,
	Z = 58,
	LEFT_BRACKET = 59,
	BACKSLASH = 60,
	RIGHT_BRACKET = 61,
	CARET = 62,
	UNDERSCORE = 63,
	BACKTICK = 64,
	a = 65,
	b = 66,
	c = 67,
	d = 68,
	e = 69,
	f = 70,
	g = 71,
	h = 72,
	i = 73,
	j = 74,
	k = 75,
	l = 76,
	m = 77,
	n = 78,
	o = 79,
	p = 80,
	q = 81,
	r = 82,
	s = 83,
	t = 84,
	u = 85,
	v = 86,
	w = 87,
	x = 88,
	y = 89,
	z = 90,
	LEFT_CURLY = 91,
	PIPE = 92,
	RIGHT_CURLY = 93,
	UNKNOWN = 94
};



std::vector<char> isolateChars(const std::string& str) {
	std::vector<char> chars;
	for (char c : str) {
		chars.push_back(c);
	}
	return chars;
}

int returnCharEnum(char letter) {
	switch (letter) {
	case ' ': return SPACEBAR; break;
	case '!': return EXCLAMATION; break;
	case '"': return DOUBLE_QUOTE; break;
	case '#': return HASH; break;
	case '$': return DOLLAR; break;
	case '%': return PERCENT; break;
	case '&': return AMPERSAND; break;
	case '\'': return SINGLE_QUOTE; break;
	case '(': return LEFT_PAREN; break;
	case ')': return RIGHT_PAREN; break;
	case '*': return ASTERISK; break;
	case '+': return PLUS; break;
	case ',': return COMMA; break;
	case '-': return MINUS; break;
	case '.': return DOT; break;
	case '/': return SLASH; break;
	case '0': return ZERO; break;
	case '1': return ONE; break;
	case '2': return TWO; break;
	case '3': return THREE; break;
	case '4': return FOUR; break;
	case '5': return FIVE; break;
	case '6': return SIX; break;
	case '7': return SEVEN; break;
	case '8': return EIGHT; break;
	case '9': return NINE; break;
	case ':': return COLON; break;
	case ';': return SEMICOLON; break;
	case '<': return LESS_THAN; break;
	case '=': return EQUALS; break;
	case '>': return GREATER_THAN; break;
	case '?': return QUESTION; break;
	case '@': return AT; break;
	case 'A': return A; break;
	case 'B': return B; break;
	case 'C': return C; break;
	case 'D': return D; break;
	case 'E': return E; break;
	case 'F': return F; break;
	case 'G': return G; break;
	case 'H': return H; break;
	case 'I': return I; break;
	case 'J': return J; break;
	case 'K': return K; break;
	case 'L': return L; break;
	case 'M': return M; break;
	case 'N': return N; break;
	case 'O': return O; break;
	case 'P': return P; break;
	case 'Q': return Q; break;
	case 'R': return R; break;
	case 'S': return S; break;
	case 'T': return T; break;
	case 'U': return U; break;
	case 'V': return V; break;
	case 'W': return W; break;
	case 'X': return X; break;
	case 'Y': return Y; break;
	case 'Z': return Z; break;
	case '[': return LEFT_BRACKET; break;
	case '\\': return BACKSLASH; break;
	case ']': return RIGHT_BRACKET; break;
	case '^': return CARET; break;
	case '_': return UNDERSCORE; break;
	case '`': return BACKTICK; break;
	case 'a': return a; break;
	case 'b': return b; break;
	case 'c': return c; break;
	case 'd': return d; break;
	case 'e': return e; break;
	case 'f': return f; break;
	case 'g': return g; break;
	case 'h': return h; break;
	case 'i': return i; break;
	case 'j': return j; break;
	case 'k': return k; break;
	case 'l': return l; break;
	case 'm': return m; break;
	case 'n': return n; break;
	case 'o': return o; break;
	case 'p': return p; break;
	case 'q': return q; break;
	case 'r': return r; break;
	case 's': return s; break;
	case 't': return t; break;
	case 'u': return u; break;
	case 'v': return v; break;
	case 'w': return w; break;
	case 'x': return x; break;
	case 'y': return y; break;
	case 'z': return z; break;
	case '{': return LEFT_CURLY; break;
	case '|': return PIPE; break;
	case '}': return RIGHT_CURLY; break;
	default: return UNKNOWN; break;
	}
}

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
		0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f, 1.f,             // top right
		0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f , 0.0f,           // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,           // bottom left
	   -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.f             // top left
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



			for (int i = 0; i < getLevel()->listOfLayers.size(); i++)
			{
				getLevel()->listOfLayers[i].clear();
			}

			//Sort/Update LevelBackground
			for (int i = 0; i < getLevel()->levelbackgrounds.size(); ++i)
			{
				getLevel()->levelbackgrounds[i]->OnUpdate();
				getLevel()->listOfLayers[getLevel()->levelbackgrounds[i]->GetSortingLayer()].push_back(getLevel()->levelbackgrounds[i]);
			}

			// Delete GameObjects
			for (int i = getLevel()->levelObjects.size() - 1; i >= 0; --i) {
				if (getLevel()->levelObjects[i]->toBeDeleted == true) {
					getLevel()->levelObjects[i]->OnDestroyed();
					if (getLevel()->levelObjects[i]->animation->tilemapPath != "")
					{
						glUseProgram(getLevel()->levelObjects[i]->m_ShaderProgram);
						glBindVertexArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
						glActiveTexture(GL_TEXTURE0);
						glDeleteProgram(getLevel()->levelObjects[i]->m_ShaderProgram);
					}

					if (getLevel()->levelObjects[i]->bodyId != nullptr)
					{
						b2DestroyBody(*getLevel()->levelObjects[i]->bodyId);
					}
					else
					{
						std::cout << "Object with no body" << i << std::endl;
					}
					delete getLevel()->levelObjects[i];
					getLevel()->levelObjects.erase(getLevel()->levelObjects.begin() + i);
				}
			}

			for (int i = getLevel()->levelObjects.size() - 1; i >= 0; --i)
			{
				auto obj = getLevel()->levelObjects[i];
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

			//Sort/Update Level Objects
			for (int i = 0; i < getLevel()->levelObjects.size(); ++i)
			{
				getLevel()->levelObjects[i]->OnUpdate();
				getLevel()->listOfLayers[getLevel()->levelObjects[i]->GetSortingLayer()].push_back(getLevel()->levelObjects[i]);
			}

			//Sort/Update UI Text
			for (int i = 0; i < getLevel()->uiTexts.size(); ++i)
			{
				getLevel()->uiTexts[i]->OnUpdate();
				getLevel()->listOfLayers[getLevel()->uiTexts[i]->GetSortingLayer()].push_back(getLevel()->uiTexts[i]);
			}

			//Manage Created Objects
			for (int i = 0; i < getLevel()->levelObjects.size(); ++i) {
				GameObject* obj = getLevel()->levelObjects[i];

				Animation* spriteAnimation = obj->animation;

				//THIS IS TO IGNORE SPAWNERS. THE FIRST TWO OBJECTS IN THE LEVEL OBJECTS VECTOR ARE SPAWNERS
					//This is a just a workaround for now. I will implement a better way to handle this later, because i need to create
					//a bool variable for objects for the user to want or not a box2d body but right now i dont have time for that.

				if (getLevel()->levelObjects[i]->hasBox2d)
				{
					float bodyWidth;// = getLevel()->levelObjects[i]->collisionBoxSize.w;
					float bodyHeight;// = getLevel()->levelObjects[i]->collisionBoxSize.h;
					bodyWidth = getLevel()->levelObjects[i]->collisionBoxSize.w / 2.0f;
					bodyHeight = getLevel()->levelObjects[i]->collisionBoxSize.h / 2.0f;


					b2BodyDef* bodyDef = new b2BodyDef;
					*bodyDef = b2DefaultBodyDef();
					bodyDef->type = b2_dynamicBody;
					bodyDef->position = { getLevel()->levelObjects[i]->position.x, getLevel()->levelObjects[i]->position.y };
					//bodyDef-> = getLevel()->levelObjects[i]->isBullet;
					bodyDef->userData = getLevel()->levelObjects[i];


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

					//shapeDef->enableSensorEvents = getLevel()->levelObjects[i]->hasSense;

					//shapeDef->enableSensorEvents = true;
					//shapeDef->isSensor = getLevel()->levelObjects[i]->hasSense;

					//shapeDef->enableContactEvents = true;

					shapeDef->userData = getLevel()->levelObjects[i];

					shapeDef->enableContactEvents = true;

					b2ShapeId* shapeId = new b2ShapeId;
					*shapeId = b2CreatePolygonShape(*bodyId, shapeDef, dynamicBox);

					getLevel()->levelObjects[i]->bodyId = bodyId;
					getLevel()->levelObjects[i]->bodyDef = bodyDef;
					getLevel()->levelObjects[i]->shapeId = shapeId;
					getLevel()->levelObjects[i]->shapeDef = shapeDef;
					getLevel()->levelObjects[i]->boxCollision = dynamicBox;
				}

				b2World_Step(worldId, timeStep, subStepCount);
				contactListener();

				while (SDL_PollEvent(&event) != 0) {
					if (event.type == SDL_QUIT) {
						isRunning = false;
					}
				}
			}

			glClearColor(0.0f, 1.0f, 1.0f, 1.0f); // Cyan Blue

			glClear(GL_COLOR_BUFFER_BIT);

			for (int i = 0; i < getLevel()->listOfLayers.size(); i++)
			{
				
				for (Object* obj: getLevel()->listOfLayers[i])
				{
					switch (obj->getType())
					{
					case Object::Type::LevelBackground:

						if (LevelBackground* it = dynamic_cast<LevelBackground*>(obj))
						{
							//Create Background/Background Asset
							if (!(it)->isTiled)
							{
								if (!(it)->isInit)
								{
									std::cout << "shader program is null\n" << std::endl;

									glGenBuffers(1, &(it)->m_vbo); // Generate 1 buffer

									glGenBuffers(1, &(it)->m_ebo);

									glGenVertexArrays(1, &(it)->m_vao);

									// 1. bind Vertex Array Object
									glBindVertexArray((it)->m_vao);

									// 2. copy our vertices array in a buffer for OpenGL to use
									glBindBuffer(GL_ARRAY_BUFFER, (it)->m_vbo);
									glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);

									glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (it)->m_ebo);
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

									if (!success)
									{
										GLchar* infoLog = new GLchar();
										glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
										std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
									}

									// Fragment Shader

									const char* fragmentShaderSource = R"glsl(
											#version 330 core
											in vec3 Color;
											in vec2 TexCoord;
											
											out vec4 outColor;

											uniform sampler2D ourTexture;
											uniform vec4 ColorChange;

											void main()
											{
												vec4 colTex1 = texture(ourTexture, TexCoord);
												if(colTex1 == vec4(1, 0, 1, 1)) discard;

												outColor = colTex1 + ColorChange;
											})glsl";

									GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
									glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
									glCompileShader(fragmentShader);

									glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

									if (!success)
									{
										GLchar* infoLog = new GLchar();
										glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
										std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
									}

									(it)->m_ShaderProgram = glCreateProgram();

									glAttachShader((it)->m_ShaderProgram, vertexShader);
									glAttachShader((it)->m_ShaderProgram, fragmentShader);
									glLinkProgram((it)->m_ShaderProgram);

									glDeleteShader(vertexShader);
									glDeleteShader(fragmentShader);

									glGetProgramiv((it)->m_ShaderProgram, GL_LINK_STATUS, &success);
									if (!success) 
									{
										GLchar* infoLog = new GLchar();
										glGetProgramInfoLog((it)->m_ShaderProgram, 512, NULL, infoLog);
										std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
									}

									// 3. then set our vertex attributes pointers
									GLint posAttrib = glGetAttribLocation((it)->m_ShaderProgram, "position");
									glEnableVertexAttribArray(posAttrib);
									glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

									GLint colorAttrib = glGetAttribLocation((it)->m_ShaderProgram, "color");
									glEnableVertexAttribArray(colorAttrib);
									glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

									GLint texCoordAttrib = glGetAttribLocation((it)->m_ShaderProgram, "texCoord");
									glEnableVertexAttribArray(texCoordAttrib);
									glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


									glGenTextures(1, &(it)->m_Texture);
									glBindTexture(GL_TEXTURE_2D, (it)->m_Texture);


									// set the texture wrapping/filtering options (on the currently bound texture object)
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

									stbi_set_flip_vertically_on_load(true);

									int width, height, nrChannels;
									unsigned char* data = stbi_load((it)->background_path.c_str(), &width, &height, &nrChannels, 0);
									if (data)
									{
										glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
										glGenerateMipmap(GL_TEXTURE_2D);
									}
									else
									{
										std::cout << "Failed to load texture" << (it)->background_path << std::endl;
									}
									stbi_image_free(data);

									glUseProgram((it)->m_ShaderProgram);

									GLuint textureLocation;

									textureLocation = glGetUniformLocation((it)->m_ShaderProgram, "ourTexture");

									glUniform1i(textureLocation, 0);

									(it)->isInit = true;

								}

								if ((it)->isInit)
								{
									glUseProgram((it)->m_ShaderProgram);

									glm::vec4 color = glm::vec4((it)->colorChange.r, (it)->colorChange.g, (it)->colorChange.b, (it)->colorChange.a);

									GLuint colorChangeLocation;
									colorChangeLocation = glGetUniformLocation((it)->m_ShaderProgram, "ColorChange");
									glUniform4fv(colorChangeLocation, 1, glm::value_ptr(color));

									glm::mat4 model = glm::mat4(1.0f); // Identity matrix
									model = glm::translate(model, glm::vec3((it)->scrollRect.w, (it)->scrollRect.h, 1.0f)); // Apply translation
									model = glm::scale(model, glm::vec3((it)->size.x, (it)->size.y, 1.0f)); // Apply scaling
									
									// Pass the model matrix to the shader
									GLuint modelLoc = glGetUniformLocation((it)->m_ShaderProgram, "model");
									glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


									glBindVertexArray((it)->m_vao);

									glActiveTexture(GL_TEXTURE0);
									glBindTexture(GL_TEXTURE_2D, (it)->m_Texture);

									glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

								}
							}
							else
							{
								if (!(it)->isInit)
								{

									float tempVertices[] = {
										// positions         // colors           // texture coords                                                                        
										0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f / ((float)(it)->tileMapSize.columns),  1.f,	                                      // top right
										0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f / ((float)(it)->tileMapSize.columns),  1.f - (1.f / ((float)(it)->tileMapSize.rows)),// bottom right
									   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,										1.f - (1.f / ((float)(it)->tileMapSize.rows)),// bottom left
									   -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,										1.f     							          // top left
									};

									std::copy(std::begin(tempVertices), std::end(tempVertices), std::begin((it)->tiledVertices));

									// Initialize tiled background
									glGenBuffers(1, &(it)->m_vbo); // Generate 1 buffer
									glGenBuffers(1, &(it)->m_ebo);
									glGenVertexArrays(1, &(it)->m_vao);

									// 1. bind Vertex Array Object
									glBindVertexArray((it)->m_vao);

									// 2. copy our vertices array in a buffer for OpenGL to use
									glBindBuffer(GL_ARRAY_BUFFER, (it)->m_vbo);
									glBufferData(GL_ARRAY_BUFFER, sizeof((it)->tiledVertices), (it)->tiledVertices, GL_STATIC_DRAW);

									glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (it)->m_ebo);
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
											uniform vec4 ColorChange;

											out vec4 outColor;

											uniform sampler2D ourTexture;

											void main()
											{
												vec4 colTex1 = texture(ourTexture, TexCoord);
												if(colTex1 == vec4(1, 0, 1, 1)) discard;

												outColor = colTex1 + ColorChange;
											})glsl";

									GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
									glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
									glCompileShader(fragmentShader);

									glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

									(it)->m_ShaderProgram = glCreateProgram();
									glAttachShader((it)->m_ShaderProgram, vertexShader);
									glAttachShader((it)->m_ShaderProgram, fragmentShader);
									glLinkProgram((it)->m_ShaderProgram);

									glDeleteShader(vertexShader);
									glDeleteShader(fragmentShader);

									glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);

									// 3. then set our vertex attributes pointers
									GLint posAttrib = glGetAttribLocation((it)->m_ShaderProgram, "position");
									glEnableVertexAttribArray(posAttrib);
									glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

									GLint colorAttrib = glGetAttribLocation((it)->m_ShaderProgram, "color");
									glEnableVertexAttribArray(colorAttrib);
									glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

									GLint texCoordAttrib = glGetAttribLocation((it)->m_ShaderProgram, "texCoord");
									glEnableVertexAttribArray(texCoordAttrib);
									glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

									glGenTextures(1, &(it)->m_Texture);
									glBindTexture(GL_TEXTURE_2D, (it)->m_Texture);

									// set the texture wrapping/filtering options (on the currently bound texture object)
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

									stbi_set_flip_vertically_on_load(true);

									int width, height, nrChannels;
									unsigned char* data = stbi_load((it)->background_path.c_str(), &width, &height, &nrChannels, 0);
									if (data)
									{
										glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
										glGenerateMipmap(GL_TEXTURE_2D);
									}
									else
									{
										std::cout << "Failed to load texture" << (it)->background_path << std::endl;
									}
									stbi_image_free(data);

									glUseProgram((it)->m_ShaderProgram);

									GLuint textureLocation;
									textureLocation = glGetUniformLocation((it)->m_ShaderProgram, "ourTexture");
									glUniform1i(textureLocation, 0);

									(it)->isInit = true;
								}

								if ((it)->isInit)
								{
									glUseProgram((it)->m_ShaderProgram);

									for (int y = 0; y < (it)->numTiles.y; ++y)
									{

										for (int x = 0; x < (it)->numTiles.x; ++x)
										{
											int tileIndex = y * (it)->numTiles.x + x;

											if (tileIndex >= (it)->tileIDs.size())
												continue;

											int tileID = (it)->tileIDs[tileIndex];

											int column = tileID % (it)->tileMapSize.columns;
											int row = tileID / (it)->tileMapSize.columns;

											float texWidth = 1.0f / (it)->tileMapSize.columns;
											float texHeight = 1.0f / (it)->tileMapSize.rows;

											float xTexCoord = column * texWidth;
											float yTexCoord = 1.0f - ((row + 1) * texHeight);

											// Update texture coordinates
											(it)->tiledVertices[6] = xTexCoord + texWidth;
											(it)->tiledVertices[7] = yTexCoord + texHeight; // Top right
											(it)->tiledVertices[14] = xTexCoord + texWidth;
											(it)->tiledVertices[15] = yTexCoord; // Bottom right
											(it)->tiledVertices[22] = xTexCoord;
											(it)->tiledVertices[23] = yTexCoord; // Bottom left
											(it)->tiledVertices[30] = xTexCoord;
											(it)->tiledVertices[31] = yTexCoord + texHeight; // Top left

											// Update VBO with new texture coordinates
											glBindBuffer(GL_ARRAY_BUFFER, (it)->m_vbo);
											glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof((it)->tiledVertices), (it)->tiledVertices);

											glm::vec4 color = glm::vec4((it)->colorChange.r, (it)->colorChange.g, (it)->colorChange.b, (it)->colorChange.a);

											GLuint colorChangeLocation;
											colorChangeLocation = glGetUniformLocation((it)->m_ShaderProgram, "ColorChange");
											glUniform4fv(colorChangeLocation, 1, glm::value_ptr(color));

											glm::mat4 model = glm::mat4(1.0f); // Identity matrix
											model = glm::translate(model, glm::vec3(((it)->scrollRect.w / 320.f) + x * (it)->size.x, ((it)->scrollRect.h / 240.f) - y * (it)->size.y, 1.0f)); // Apply translation
											model = glm::scale(model, glm::vec3((it)->size.x, (it)->size.y, 1.0f)); // Apply scaling

											// Pass the model matrix to the shader
											GLuint modelLoc = glGetUniformLocation((it)->m_ShaderProgram, "model");
											glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

											glBindVertexArray((it)->m_vao);
											glActiveTexture(GL_TEXTURE0);
											glBindTexture(GL_TEXTURE_2D, (it)->m_Texture);
											glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
										}
									}
								}
							}

						}
						
						break;
					case Object::Type::GameObject:

						if (GameObject* it = dynamic_cast<GameObject*>(obj))
						{
							//Create Objects
								if ((it)->animation != nullptr)
								{
									if ((it)->animation->tilemapPath != "")
									{
										//Initialize Object
										if (!(it)->isInit)
										{
											float tempVertices[] = {
												// positions         // colors           // texture coords
												0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f / ((float)(it)->animation->tilemapSize.w),  1.f,   // top right
												0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f / ((float)(it)->animation->tilemapSize.w),  1.f - (1.f / ((float)(it)->animation->tilemapSize.h)),   // bottom right
											   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,											1.f - (1.f / ((float)(it)->animation->tilemapSize.h)),   // bottom left
											   -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,											1.f    // top left
											};

											std::copy(std::begin(tempVertices), std::end(tempVertices), std::begin((it)->m_Vertices));

											glGenBuffers(1, &(it)->m_vbo); // Generate 1 buffer

											glGenBuffers(1, &(it)->m_ebo);

											glGenVertexArrays(1, &(it)->m_vao);

											// 1. bind Vertex Array Object
											glBindVertexArray((it)->m_vao);

											// 2. copy our vertices array in a buffer for OpenGL to use
											glBindBuffer(GL_ARRAY_BUFFER, (it)->m_vbo);
											glBufferData(GL_ARRAY_BUFFER, sizeof((it)->m_Vertices), (it)->m_Vertices, GL_STATIC_DRAW);

											glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (it)->m_ebo);
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
											uniform vec4 ColorChange;

											out vec4 outColor;

											uniform sampler2D ourTexture;

											void main()
											{
												vec4 colTex1 = texture(ourTexture, TexCoord);
												if(colTex1 == vec4(1, 0, 1, 1)) discard;

												outColor = colTex1 + ColorChange;
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

											(it)->m_ShaderProgram = glCreateProgram();

											glAttachShader((it)->m_ShaderProgram, vertexShader);
											glAttachShader((it)->m_ShaderProgram, fragmentShader);
											glLinkProgram((it)->m_ShaderProgram);

											glDeleteShader(vertexShader);
											glDeleteShader(fragmentShader);

											glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
											if (!success) {
												//glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
												//std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
											}

											// 3. then set our vertex attributes pointers
											GLint posAttrib = glGetAttribLocation((it)->m_ShaderProgram, "position");
											glEnableVertexAttribArray(posAttrib);
											glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

											GLint colorAttrib = glGetAttribLocation((it)->m_ShaderProgram, "color");
											glEnableVertexAttribArray(colorAttrib);
											glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

											GLint texCoordAttrib = glGetAttribLocation((it)->m_ShaderProgram, "texCoord");
											glEnableVertexAttribArray(texCoordAttrib);
											glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

											glGenTextures(1, &(it)->m_Texture);
											glBindTexture(GL_TEXTURE_2D, (it)->m_Texture);


											// set the texture wrapping/filtering options (on the currently bound texture object)
											glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
											glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
											glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
											glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

											stbi_set_flip_vertically_on_load(true);

											int width, height, nrChannels;
											unsigned char* data = stbi_load((it)->animation->tilemapPath.c_str(), &width, &height, &nrChannels, 0);
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

											glUseProgram((it)->m_ShaderProgram);

											GLuint textureLocation;

											textureLocation = glGetUniformLocation((it)->m_ShaderProgram, "ourTexture");

											glUniform1i(textureLocation, 0);

											(it)->isInit = true;

										}

										//Update Object
										if ((it)->isInit)
										{
											Animation* spriteAnimation = (it)->animation;
											glUseProgram((it)->m_ShaderProgram);
											if (spriteAnimation->tilemapPath != "") {

												if (spriteAnimation->manual.empty() == true)
												{
													// Increment elapsed time
													(it)->elapsedTime += deltaTime;

													// Check if enough time has passed to advance to the next frame
													if ((it)->elapsedTime >= spriteAnimation->frameDuration) {
														// Subtract frameTime to preserve leftover time
														(it)->elapsedTime -= spriteAnimation->frameDuration;

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
														(it)->m_Vertices[6] = x + texWidth; (it)->m_Vertices[7] = y + texHeight; // Top right
														(it)->m_Vertices[14] = x + texWidth; (it)->m_Vertices[15] = y;           // Bottom right
														(it)->m_Vertices[22] = x;            (it)->m_Vertices[23] = y;           // Bottom left
														(it)->m_Vertices[30] = x;            (it)->m_Vertices[31] = y + texHeight; // Top left

														if (spriteAnimation->currentFrame == spriteAnimation->tilemapSize.h - 1)
														{
															(it)->OnAnimationFinish();
														}

														// Update VBO with new texture coordinates
														glBindBuffer(GL_ARRAY_BUFFER, (it)->m_vbo);
														glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 32, (it)->m_Vertices);
													}
												}
												if (spriteAnimation->manual.empty() == false)
												{
													// Increment elapsed time
													(it)->elapsedTime += deltaTime;

													// Check if enough time has passed to advance to the next frame
													if ((it)->elapsedTime >= spriteAnimation->frameDuration) {
														// Subtract frameTime to preserve leftover time
														(it)->elapsedTime -= spriteAnimation->frameDuration;

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
															(it)->OnAnimationFinish();
														}
														// Calculate texture coordinates for the current frame
														int column = spriteAnimation->currentFrame % spriteAnimation->tilemapSize.w;
														int row = spriteAnimation->currentFrame / spriteAnimation->tilemapSize.w;

														float texWidth = 1.0f / spriteAnimation->tilemapSize.w;
														float texHeight = 1.0f / spriteAnimation->tilemapSize.h;

														float x = column * texWidth;
														float y = 1.0f - ((row + 1) * texHeight);

														// Update texture coordinates
														(it)->m_Vertices[6] = x + texWidth; (it)->m_Vertices[7] = y + texHeight; // Top right
														(it)->m_Vertices[14] = x + texWidth; (it)->m_Vertices[15] = y;           // Bottom right
														(it)->m_Vertices[22] = x;            (it)->m_Vertices[23] = y;           // Bottom left
														(it)->m_Vertices[30] = x;            (it)->m_Vertices[31] = y + texHeight; // Top left

														// Update VBO with new texture coordinates
														glBindBuffer(GL_ARRAY_BUFFER, (it)->m_vbo);
														glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 32, (it)->m_Vertices);
													}
												}

											}

											glm::vec4 color = glm::vec4((it)->colorChange.r, (it)->colorChange.g, (it)->colorChange.b, (it)->colorChange.a);

											GLuint colorChangeLocation;
											colorChangeLocation = glGetUniformLocation((it)->m_ShaderProgram, "ColorChange");
											glUniform4fv(colorChangeLocation, 1, glm::value_ptr(color));

											glm::mat4 model = glm::mat4(1.0f); // Identity matrix
											model = glm::translate(model, glm::vec3((it)->position.x / 320.f, (it)->position.y / 240.f, 1.0f)); // Apply translation
											model = glm::scale(model, glm::vec3((it)->collisionBoxSize.w / 250.f, (it)->collisionBoxSize.h / 250.f, 1.0f)); // Apply scaling

											// Pass the model matrix to the shader
											GLuint modelLoc = glGetUniformLocation((it)->m_ShaderProgram, "model");
											//GLint projectionLoc = glGetUniformLocation((it)->m_ShaderProgram, "projection");
											glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


											glBindVertexArray((it)->m_vao);

											glActiveTexture(GL_TEXTURE0);
											glBindTexture(GL_TEXTURE_2D, (it)->m_Texture);

											glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

											glUseProgram(0);
										}
									}
								}
							
						}

						break;
					case Object::Type::UIText:

						//Create UI text elements
						if (UIText* targetUI = dynamic_cast<UIText*>(obj))
						{
							if (!targetUI->isInit)
							{

								float tempVertices[] = {
									// positions         // colors           // texture coords
									0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f / ((float)targetUI->myFont->bitMapSize.columns),  1.f,   // top right
									0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.f / ((float)targetUI->myFont->bitMapSize.columns),  1.f - (1.f / ((float)targetUI->myFont->bitMapSize.rows)),   // bottom right
								   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,											1.f - (1.f / ((float)targetUI->myFont->bitMapSize.rows)),   // bottom left
								   -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f,											1.f    // top left
								};

								std::copy(std::begin(tempVertices), std::end(tempVertices), std::begin(targetUI->myVertices));

								// Initialize tiled background
								glGenBuffers(1, &targetUI->m_vbo); // Generate 1 buffer
								glGenBuffers(1, &targetUI->m_ebo);
								glGenVertexArrays(1, &targetUI->m_vao);

								// 1. bind Vertex Array Object
								glBindVertexArray(targetUI->m_vao);

								// 2. copy our vertices array in a buffer for OpenGL to use
								glBindBuffer(GL_ARRAY_BUFFER, targetUI->m_vbo);
								glBufferData(GL_ARRAY_BUFFER, sizeof(targetUI->myVertices), targetUI->myVertices, GL_STATIC_DRAW);

								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, targetUI->m_ebo);
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

								targetUI->m_ShaderProgram = glCreateProgram();
								glAttachShader(targetUI->m_ShaderProgram, vertexShader);
								glAttachShader(targetUI->m_ShaderProgram, fragmentShader);
								glLinkProgram(targetUI->m_ShaderProgram);

								glDeleteShader(vertexShader);
								glDeleteShader(fragmentShader);

								glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);

								// 3. then set our vertex attributes pointers
								GLint posAttrib = glGetAttribLocation(targetUI->m_ShaderProgram, "position");
								glEnableVertexAttribArray(posAttrib);
								glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

								GLint colorAttrib = glGetAttribLocation(targetUI->m_ShaderProgram, "color");
								glEnableVertexAttribArray(colorAttrib);
								glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

								GLint texCoordAttrib = glGetAttribLocation(targetUI->m_ShaderProgram, "texCoord");
								glEnableVertexAttribArray(texCoordAttrib);
								glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

								glGenTextures(1, &targetUI->m_Texture);
								glBindTexture(GL_TEXTURE_2D, targetUI->m_Texture);

								// set the texture wrapping/filtering options (on the currently bound texture object)
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

								stbi_set_flip_vertically_on_load(true);

								int width, height, nrChannels;
								unsigned char* data = stbi_load(targetUI->myFont->bitMapPath.c_str(), &width, &height, &nrChannels, 0);
								if (data)
								{
									glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
									glGenerateMipmap(GL_TEXTURE_2D);
								}
								else
								{
									std::cout << "Failed to load texture" << targetUI->myFont->bitMapPath << std::endl;
								}
								stbi_image_free(data);

								glUseProgram(targetUI->m_ShaderProgram);

								GLuint textureLocation;
								textureLocation = glGetUniformLocation(targetUI->m_ShaderProgram, "ourTexture");
								glUniform1i(textureLocation, 0);

								targetUI->isInit = true;
							}
							if (targetUI->isInit)
							{
								glUseProgram(targetUI->m_ShaderProgram);

								targetUI->charText = isolateChars(targetUI->myText);

								int nLetters = 0;

								std::vector<int> letterIDs;
								std::vector<int> nLetterInPar;

								for (int i = 0; i < targetUI->charText.size(); i++) {
									if (targetUI->charText[i] == '\n' || returnCharEnum(targetUI->charText[i]) == UNKNOWN) {
										nLetterInPar.push_back(nLetters);
										nLetters = 0;
									}
									else {
										nLetters++;
										letterIDs.push_back(returnCharEnum(targetUI->charText[i]));
									}
								}
								nLetterInPar.push_back(nLetters);

								int letterwritten = 0;
								for (int y = 0; y < nLetterInPar.size(); ++y) {
									for (int x = 0; x < nLetterInPar[y]; ++x) {
										int tileIndex = letterwritten;
										if (tileIndex >= letterIDs.size())
											continue;
										letterwritten++;
										int tileID = letterIDs[tileIndex];

										int column = tileID % targetUI->myFont->bitMapSize.columns;
										int row = tileID / targetUI->myFont->bitMapSize.columns;

										float texWidth = 1.0f / targetUI->myFont->bitMapSize.columns;
										float texHeight = 1.0f / targetUI->myFont->bitMapSize.rows;

										float xTexCoord = column * texWidth;
										float yTexCoord = 1.0f - ((row + 1) * texHeight);

										// Update texture coordinates
										targetUI->myVertices[6] = xTexCoord + texWidth;
										targetUI->myVertices[7] = yTexCoord + texHeight; // Top right
										targetUI->myVertices[14] = xTexCoord + texWidth;
										targetUI->myVertices[15] = yTexCoord; // Bottom right
										targetUI->myVertices[22] = xTexCoord;
										targetUI->myVertices[23] = yTexCoord; // Bottom left
										targetUI->myVertices[30] = xTexCoord;
										targetUI->myVertices[31] = yTexCoord + texHeight; // Top left

										// Update VBO with new texture coordinates
										glBindBuffer(GL_ARRAY_BUFFER, targetUI->m_vbo);
										glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(targetUI->myVertices), targetUI->myVertices);

										glm::mat4 model = glm::mat4(1.0f); // Identity matrix
										model = glm::translate(model, glm::vec3(targetUI->position.x + x * targetUI->size.x, targetUI->position.y - y * targetUI->size.y, 1.0f)); // Apply translation
										model = glm::scale(model, glm::vec3(targetUI->size.x, targetUI->size.y, 1.0f)); // Apply scaling

										// Pass the model matrix to the shader
										GLuint modelLoc = glGetUniformLocation(targetUI->m_ShaderProgram, "model");
										glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

										glBindVertexArray(targetUI->m_vao);
										glActiveTexture(GL_TEXTURE0);
										glBindTexture(GL_TEXTURE_2D, targetUI->m_Texture);
										glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
									}

								}
							}
						}
						break;
					default:
						std::cout << "No Object type to render found!" << std::endl;
						break;
					}
				}
			}

			SDL_GL_SwapWindow(window);
		}
			SDL_DestroyWindow(window);

			window = nullptr;

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

		SDL_GL_MakeCurrent(window, m_Context);

		b2World_EnableContinuous(worldId, true);


		Update();
	}

	void Engine::setLevel(GameLevel* level)
	{
		mainLevel = level;
	}

	void Engine::print(std::string printText)
	{
		std::cout << printText << std::endl;
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

				void* myUserData2 = b2Shape_GetUserData(beginTouch->shapeIdB);

				
				if (myUserData2)
				{
					GameObject* m2 = static_cast<GameObject*>(myUserData2);
					m->OnCollideEnter(*m2);

				}
			}
		}
	}


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

void GameLevel::addUIText(UIText* uiText)
{
	uiTexts.push_back(uiText);
}

void GameLevel::addBackground(LevelBackground* background)
{
	levelbackgrounds.push_back(background);
	background->OnStart();
}


void GameLevel::SetSortingLayerSize(int i)
{
	listOfLayers.resize(i);
}


int Animation::GetSpriteWidth()
{
	int ret = animationRect.w / tilemapSize.w;
	return ret;
}


