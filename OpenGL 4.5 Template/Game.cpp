#include "stdafx.h"
#include "Game.h"


void Game::handleInput()
{

}

void Game::update()
{
	// update all objects by iterating over the list - polymorphism FTW.
	for (int y = 0; y < GRIDHEIGHT; y++)
	{
		for (int x = 0; x < GRIDWIDTH; x++)
		{
			gameGrid[x][y]->update();
		}
	}
}

void Game::draw(HWND hWnd)
{
	// clear screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update view and projection matrices inside shader (in case they have changed)
	int matrixLocation = glGetUniformLocation(shaderProgram, "worldToView");
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, worldToView);
	matrixLocation = glGetUniformLocation(shaderProgram, "viewToProjection");
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, viewToProjection);
	
	// draw all objects by iterating over the list - polymorphism FTW.
	for (int y = 0; y < GRIDHEIGHT; y++)
	{
		for (int x = 0; x < GRIDWIDTH; x++)
		{
			gameGrid[x][y]->draw();
		}
	}



	// flip
	SwapBuffers(GetDC(hWnd));
}


Game::Game(HGLRC& renderContext)
{

	// generate one vertex buffer object for each mesh type
	glGenBuffers(2, VBO);

	// generate one element buffer object for each mesh type
	glGenBuffers(2, EBO);

	// generate a vertex array object for each mesh type to allow us to recall the various VBO commands all in one go
	glGenVertexArrays(2, VAO);


	// begin list of VBO commands for the rectangle, and then the triangle...

		glBindVertexArray(VAO[0]);	// 0 = rect

		// specify our vertex array for the VBO to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_rect), vertices_rect, GL_STATIC_DRAW); // STATIC_DRAW because the mesh data doesn't change, i.e. isn't animated.

		// specify our indices array for the VBO to use 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_rect), indices_rect, GL_STATIC_DRAW);

		// this bit is almost the same for both shapes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		// vertex position layout location is listed as 0 in the vert shader
		// 3 floats
		// not normalised
		// tightly packed
		// start position in array of vertex data (0) 

		glEnableVertexAttribArray(0); // enable attribute in layout location 0 (vertex position in our case)


		// now do the triangle...

		glBindVertexArray(VAO[1]);	// 1 = tri

		// specify our vertex array for the VBO to use (same for both shapes)
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_tri), vertices_tri, GL_STATIC_DRAW); // STATIC_DRAW because the mesh data doesn't change, i.e. isn't animated.

			// this bit is almost the same for both shapes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		// specify our indices array for the VBO to use (same for both shapes)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_tri), indices_tri, GL_STATIC_DRAW);

		// vertex position layout location is listed as 0 in the vert shader
		// 3 floats
		// not normalised
		// tightly packed
		// start position in array of vertex data (0)

		glEnableVertexAttribArray(0); // enable attribute in layout location 0 (vertex position in our case)
	


	// https://learnopengl.com/Getting-started/Shaders
	// create and compile shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// we will try to compile them as strings
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open("VertexShader.vert");
		fShaderFile.open("FragmentShader.frag");
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	// convert strings to "C-style" strings
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	// compile vert shader
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];

	// check if failed
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		// leave a breakpoint here and you can see if the compilation failed
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;		
	}

	// compile frag shader
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	
	// check if failed
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		// leave a breakpoint here and you can see if the compilation failed
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// combine and link shaders into finished shader program

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check if failed
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
	// free up the original resources
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// background colour of openGL world
	glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
	
	// since our camera and projection aren't going to change during the game (this might not be the case for you) we can just compute them once at the beginning.
	computeView(worldToView);
	computeProjection(45.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 10.0f, viewToProjection);

	// add some shapes to the scene
	for (int y = 0; y < GRIDHEIGHT; y++)
	{
		for (int x = 0; x < GRIDWIDTH; x++)
		{
			gameGrid[x][y] = new Shape_Rect(-0.2f + (x * 0.03f), -0.2f + (y * 0.03), VAO[0], shaderProgram, 1.0f, 1.0f, 1.0f);
		}
	}
}

// once game exits, clean up memory
Game::~Game()
{
	for (int y = 0; y < GRIDHEIGHT; y++)
	{
		for (int x = 0; x < GRIDWIDTH; x++)
		{
			delete(gameGrid[x][y]);
			gameGrid[x][y] = nullptr;
		}
	}
}

