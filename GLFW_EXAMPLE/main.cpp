/*
	COMP3320
	Project: Trains
	Module 1
	File name: main.cpp
	Students:
		Jack Ratcliffe 3236537
		Sebastian Brown 3220619
	Description: This is the first module in our attempt to make a low poly sandbox game.
*/
	
//Library for loading textures (Simple OpenGL Image Library)
#include <SOIL.h>

#include <GL/glew.h>  

#include <iostream> //cout
#include <fstream> //fstream
#include <ctime> 

//Include GLFW  
#include <GLFW/glfw3.h>  
      
//Include the standard C++ headers  
#include <stdio.h>  
#include <stdlib.h> 

//Include matrix libraries
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//Include header files
#include "Shader_m.h"
#include "Camera.h"
#include "Model.h"
#include "LevelData.h"
#include "TrackSegmentStraight.h"
#include "Particle.h"

const GLint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//Camera Movement
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

//Default Camera Variables
Camera camera(glm::vec3(0.0f, 20.0f, 90.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;
bool startSequence = false;

//Time starts at 0
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(36.0, 200, 200); // Above water -36.0, 50, -70

//Particles
const int MaxParticles = 100000;
Particle ParticlesContainer[MaxParticles];
int LastUsedParticle = 0;

// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int FindUnusedParticle() {

	for (int i = LastUsedParticle; i<MaxParticles; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i<LastUsedParticle; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void SortParticles() {
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath) {

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return textureID;


}

int main() {


	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "COMP3320 Project", nullptr, nullptr);


	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	if (nullptr == window) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader ourShader("shaders/4.1.lighting_maps.vs", "shaders/4.1.lighting_maps.fs");

	// Load the level data object, it contans all the data for the game's initial state.

	LevelData levelData;

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Camera fly in sequence
	if (startSequence)
	{
		camera.startFlyIn();
	}


	// Create and compile our GLSL program from the shaders
	Shader programID("Particle.vs", "Particle.fs");

	// Vertex shader
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID.Program, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID.Program, "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(programID.Program, "VP");

	// fragment shader
	GLuint TextureID = glGetUniformLocation(programID.Program, "myTextureSampler");


	static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	static GLubyte* g_particule_color_data = new GLubyte[MaxParticles * 4];

	for (int i = 0; i<MaxParticles; i++) {
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}

	GLuint Texture = loadDDS("particle.DDS");

	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};
	GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	GLuint particles_position_buffer;
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	GLuint particles_color_buffer;
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);



	//Game Loop
	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//check for events/input
		glfwPollEvents();
		DoMovement();

		if (keys[GLFW_KEY_1]) {
			levelData.switchTrack(1);
		}
		if (keys[GLFW_KEY_2]) {
			levelData.switchTrack(2);
		}

		// Do object movement

	
		levelData.moveAlongTrack(4, 0.1);
		levelData.moveAlongTrack(5, 0.1);



		//render
		glClearColor(0.47f, 0.67f, 0.98f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		programID.Use();

		// light properties


		glm::mat4 view = camera.GetViewMatrix();

		/*START OF PARTICLES*/
		/*START OF PARTICLES*/
		/*START OF PARTICLES*/
		/*START OF PARTICLES*/
		/*START OF PARTICLES*/
		/*START OF PARTICLES*/


		glm::vec3 CameraPosition(glm::inverse(view)[3]);
		glm::mat4 ProjectionMatrix = camera.GetProjectionMatrix();
		glm::mat4 ViewProjectionMatrix = ProjectionMatrix * view;


		// Generate 10 new particule each millisecond,
		// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
		// newparticles will be huge and the next frame even longer.
		int newparticles = (int)(deltaTime*10000.0);
		if (newparticles > (int)(0.016f*10000.0))
			newparticles = (int)(0.016f*10000.0);

		for (int i = 0; i<newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
			//ParticlesContainer[particleIndex].pos = glm::vec3(0, 0, -20.0f);
			ParticlesContainer[particleIndex].pos = glm::vec3(-4.0f, 17.0f, 43.0f);

			float spread = 1.5f;
			glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
			// Very bad way to generate a random direction; 
			// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
			// combined with some user-controlled parameters (main direction, spread, etc)
			glm::vec3 randomdir = glm::vec3(
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f
			);

			ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;


			// Very bad way to generate a random color
			ParticlesContainer[particleIndex].r = rand() % 256;
			ParticlesContainer[particleIndex].g = rand() % 256;
			ParticlesContainer[particleIndex].b = rand() % 256;
			ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

			ParticlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

		}



		// Simulate all particles
		int ParticlesCount = 0;
		for (int i = 0; i<MaxParticles; i++) {

			Particle& p = ParticlesContainer[i]; // shortcut

			if (p.life > 0.0f) {

				// Decrease life
				p.life -= deltaTime;
				if (p.life > 0.0f) {

					// Simulate simple physics : gravity only, no collisions
					p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)deltaTime * 0.5f;
					p.pos += p.speed * (float)deltaTime;
					p.cameradistance = glm::length2(p.pos - CameraPosition);
					//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

					// Fill the GPU buffer
					g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
					g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
					g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

					g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

					g_particule_color_data[4 * ParticlesCount + 0] = p.r;
					g_particule_color_data[4 * ParticlesCount + 1] = p.g;
					g_particule_color_data[4 * ParticlesCount + 2] = p.b;
					g_particule_color_data[4 * ParticlesCount + 3] = p.a;

				}
				else {
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}

				ParticlesCount++;

			}
		}
		SortParticles();

		//printf("%d ",ParticlesCount);


		// Update the buffers that OpenGL uses for rendering.
		// There are much more sophisticated means to stream data from the CPU to the GPU, 
		// but this is outside the scope of this tutorial.
		// http://www.opengl.org/wiki/Buffer_Object_Streaming


		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Use our shader
		glUseProgram(programID.Program);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// Same as the billboards tutorial
		glUniform3f(CameraRight_worldspace_ID, view[0][0], view[1][0], view[2][0]);
		glUniform3f(CameraUp_worldspace_ID, view[0][1], view[1][1], view[2][1]);

		glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : x + y + z + size => 4
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE,                 // type
			GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// These functions are specific to glDrawArrays*Instanced*.
		// The first parameter is the attribute buffer we're talking about.
		// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
		// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

									 // Draw the particules !
									 // This draws many times a small triangle_strip (which looks like a quad).
									 // This is equivalent to :
									 // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
									 // but faster.
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		/*END OF PARTICLES*/
		/*END OF PARTICLES*/
		/*END OF PARTICLES*/
		/*END OF PARTICLES*/
		/*END OF PARTICLES*/
		/*END OF PARTICLES*/


		ourShader.Use();
		ourShader.setVec3("light.position", lightPos);
		ourShader.setVec3("viewPos", lightPos);

		// light properties
		ourShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.5f);
		ourShader.setVec3("light.specular", 10.0f, 10.0f, 10.0f);

		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


		//Draw all the loaded models within the game EXCEPT mountianTop
		glEnable(GL_DEPTH_TEST);

		for (size_t i = 1; i < levelData.getCardinality(); i++)
		{

			if (i != 3) {

				ourShader.setVec3("material.specular", levelData.getObjectShininess(i), levelData.getObjectShininess(i), levelData.getObjectShininess(i));
				ourShader.setFloat("material.shininess", 64.0f);
				glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(levelData.getObjectPositioning(i)));
				glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(levelData.getObjectRotation(i)));

				levelData.getModel(i).Draw(ourShader);
			}
		}

		/* Don't update color or depth. */
		glDisable(GL_DEPTH_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		/* Draw 1 into the stencil buffer. */
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);


		/* Now draw the reflective lake surface
		as stencil value 1. */
		ourShader.setVec3("material.specular", levelData.getObjectShininess(2), levelData.getObjectShininess(2), levelData.getObjectShininess(2));
		ourShader.setFloat("material.shininess", 64.0f);
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(levelData.getObjectPositioning(2)));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(levelData.getObjectRotation(2)));

		levelData.getModel(2).Draw(ourShader);

		/*Re - enable update of color and depth. */
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		//glEnable(GL_DEPTH_TEST);

		/* Now, only render where stencil is set to 1. */
		glStencilFunc(GL_EQUAL, 1, 0xFF);  /* draw if stencil ==1 */
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);



		// Now draw the reflection
		glm::mat4 model = glm::scale(
			glm::translate(levelData.getObjectPositioning(3), glm::vec3(0, -40, 0)),
			glm::vec3(1, -1, 1)
		);

		ourShader.setVec3("material.specular", levelData.getObjectShininess(3), levelData.getObjectShininess(3), levelData.getObjectShininess(3));
		ourShader.setFloat("material.shininess", 64.0f);
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		levelData.getModel(3).Draw(ourShader);



		glDisable(GL_STENCIL_TEST);



		// Lastly, draw the top of the world
		glEnable(GL_DEPTH_TEST);

		ourShader.setVec3("material.specular", levelData.getObjectShininess(0), levelData.getObjectShininess(0), levelData.getObjectShininess(0));
		ourShader.setFloat("material.shininess", 64.0f);
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(levelData.getObjectPositioning(0)));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(levelData.getObjectRotation(0)));

		levelData.getModel(0).Draw(ourShader);



		// DRAW the frame

		view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//DRAW OPENGL
		glfwSwapBuffers(window);
	}


	glfwTerminate();

	return EXIT_SUCCESS;
}

//Takes a directional key press and moves camera accordingly
void DoMovement() {
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	
	//lightPos = camera.position;

	// Camera fly in sequence
	if (startSequence)
	{
		camera.updateFlyIn(deltaTime);

	}
}

//Registers key press
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (GLFW_PRESS == action) {
			keys[key] = true;
		}
		else if (GLFW_RELEASE == action) {
			keys[key] = false;
		}
	}
}

//Checks if the mouse has moved and moves the camera accordingly if so
void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}