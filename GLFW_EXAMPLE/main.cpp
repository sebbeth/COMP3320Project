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
#include <vector>

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
#include "PhysicsObject.h"
#include "Track.h"
	
//#include "btBulletDynamicsCommon.h"


const GLint WIDTH = 1280, HEIGHT = 720; //1280, HEIGHT = 720;

int SCREEN_WIDTH, SCREEN_HEIGHT;

//Camera Movement
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

//Default Camera Variables
Camera camera(glm::vec3(0.0f, 80.0f, 450.0f));
//Camera camera(glm::vec3(0.0f, 80.0f, 80.0f));

GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;
bool startSequence = false;

//Time starts at 0
GLfloat deltaTime = 0.0f;
//GLfloat lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(36.0, 200, 200);


Track track;



const int numberOfTrains = 1;

PhysicsObject trains[numberOfTrains];

/********* /TEST **********/


int lookAt = 0;

bool vertexEquality(glm::vec3 a, glm::vec3 b) {

	if ((abs(b.x - a.x) < 0.5) &&
		(abs(b.y - a.y) < 0.5) &&
		(abs(b.z - a.z) < 0.5)) {

		return true;
	}
	else {
		return false;
	}

}

bool safeNodeLookup(int index, int segmentIndex) {

	if ((index >= track.sections.at(segmentIndex).size()) || (index <= 0)) {

		std::cout << "NOT SAFE" << endl;
		return false;
	}
	return true;

}

glm::vec3 getDirectonVector(glm::vec3 position, glm::vec3 target) {


	double x = (abs(target.x - position.x));
	double y = (abs(target.y - position.y));
	double z = (abs(target.z - position.z));

	double length = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	// Get the normalized vector
	double x1 = ((target.x - position.x)) / length;
	double y1 = ((target.y - position.y)) / length;
	double z1 = ((target.z - position.z)) / length;

	return glm::vec3(x1, y1, z1);

}

//Particles
const int MaxParticles = 20; //50 25
const int MaxParticlesTrain = 20;
GameObject ParticlesContainer[MaxParticles];
GameObject ParticlesContainerTrain[MaxParticlesTrain];
int LastUsedParticle = 0;
int LastUsedParticleTrain = 0;

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
// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int FindUnusedParticleTrain() {

	for (int i = LastUsedParticle; i<MaxParticlesTrain; i++) {
		if (ParticlesContainerTrain[i].life < 0) {
			LastUsedParticleTrain = i;
			return i;
		}
	}

	for (int i = 0; i<LastUsedParticleTrain; i++) {
		if (ParticlesContainerTrain[i].life < 0) {
			LastUsedParticleTrain = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void SortParticles() {
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}
void SortParticlesTrain() {
	std::sort(&ParticlesContainerTrain[0], &ParticlesContainerTrain[MaxParticlesTrain]);
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

	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	Shader ourShader("shaders/4.1.lighting_maps.vs", "shaders/4.1.lighting_maps.fs");
	Shader particleShader("shaders/core.vs", "shaders/core.frag");


	// Load the level data object, it contans all the data for the game's initial state.

	LevelData levelData;


	/**** LOAD TRACKS */


	track.addSection(levelData.objects[11].model.getVertices());
	track.addSection(levelData.objects[12].model.getVertices());
	track.addSection(levelData.objects[13].model.getVertices());
	track.addSection(levelData.objects[14].model.getVertices());
	track.addSection(levelData.objects[15].model.getVertices());
	track.addSection(levelData.objects[16].model.getVertices());
	track.addSection(levelData.objects[17].model.getVertices());
	track.addSection(levelData.objects[18].model.getVertices());


	
	
	trains[0].currentSection = 2;
	trains[0].currentNode = 80;
	trains[0].gameObjectIndex = 6;
	trains[0].offset = glm::vec3(0.8, 0.1, 0.8);
	trains[0].position = track.getTrackSection(trains[0].currentSection).at(trains[0].currentNode);
	
	
	/*
	trains[1].currentSection = 2;
	trains[1].currentNode = 82;
	trains[1].gameObjectIndex = 5;
	trains[1].offset = glm::vec3(0.8, -0.1, 0);
	trains[1].position = track.getTrackSection(trains[1].currentSection).at(trains[1].currentNode);
	*/
	

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Camera fly in sequence
	if (startSequence)
	{
		camera.startFlyIn();
	}


	static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	int modelNo = 1;
	for (int i = 0; i<MaxParticles; i++) {
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
		//ParticlesContainer[i].model.load("models/particle.obj");
		if (modelNo == 1) {
			ParticlesContainer[i].model.load("models/smoke1.obj");
			modelNo++;
		}
		else if (modelNo == 2) {
			ParticlesContainer[i].model.load("models/smoke2.obj");
			modelNo++;
		}
		else if (modelNo == 3) {
			ParticlesContainer[i].model.load("models/smoke3.obj");
			modelNo++;
		}
		else{
			ParticlesContainer[i].model.load("models/smoke4.obj"); 
			modelNo = 1;
		}
	}

	int modelNoTrain = 1;
	for (int i = 0; i<MaxParticlesTrain; i++) {
		ParticlesContainerTrain[i].life = -1.0f;
		ParticlesContainerTrain[i].cameradistance = -1.0f;
		//ParticlesContainer[i].model.load("models/particle.obj");
		if (modelNoTrain == 1) {
			ParticlesContainerTrain[i].model.load("models/smoke1.obj");
			modelNoTrain++;
		}
		else{
			ParticlesContainerTrain[i].model.load("models/smoke2.obj");
			modelNoTrain = 1;
		}
	}

	//glm::vec3 particlePosition = glm::vec3(-58.0f,0.0f, 40.0f);
	glm::vec3 particlePosition = glm::vec3(0,0,0);
	glm::vec3 particleOffsetPosition = glm::vec3(-51.48, -8.3, 46.82);
	glm::vec3 particleOffsetPositionTrain = glm::vec3(0, 0, 0);
	glm::mat4 defaultTranslationMatrix = ParticlesContainer[0].translationMatrix;

	GLfloat lastFrame = glfwGetTime();
	
	//Game Loop
	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//check for events/input
		glfwPollEvents();
		DoMovement();

		for (int i = 0; i < numberOfTrains; i++)
		{

			if (keys[GLFW_KEY_MINUS]) {

				
					trains[i].velocity += 0.01;

					if (abs(trains[i].velocity) >= trains[i].maxVelocity) {
						trains[i].velocity = trains[i].maxVelocity;
					}
			
			


			}
			if (keys[GLFW_KEY_EQUAL]) {
				trains[i].velocity -= 0.01;
				if (trains[i].velocity <= -trains[i].maxVelocity) {
					trains[i].velocity = -trains[i].maxVelocity;
				}
			}

			if (keys[GLFW_KEY_SPACE]) {
				if (trains[i].velocity > 0.01) {
					trains[i].velocity -= 0.01;

				}
				else {

					if (abs(trains[i].velocity) < 0.01) {
						trains[i].velocity = 0;
					}
					else {
						trains[i].velocity += 0.01;

					}

				}
			}
		}

		if (keys[GLFW_KEY_1]) {
			track.changeUpcomingSwitch(trains[0].velocity, trains[0].currentSection, 1);
		}

		if (keys[GLFW_KEY_2]) {
			track.changeUpcomingSwitch(trains[0].velocity, trains[0].currentSection, 0);
		}


		/*********************** Bullet ******************************/
		/*
		dynamicsWorld->stepSimulation(1 / 60.f, 10);

		btTransform trans;
		
		fallRigidBody->setLinearVelocity(btVector3(1, 0, 0));

		fallRigidBody->getMotionState()->getWorldTransform(trans);
		*/
		

		/**************************************************/
		// Do object movement
	
		//levelData.moveAlongTrack(4, 0.1);
		//levelData.moveAlongTrack(5, 0.1);

		particleOffsetPositionTrain = levelData.objects[4].position;
		//render
		glClearColor(0.47f, 0.67f, 0.98f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 ProjectionMatrix = camera.GetProjectionMatrix();
		glm::vec3 CameraPosition(glm::inverse(view)[3]);
		glm::mat4 ViewProjectionMatrix = ProjectionMatrix * view;

		// Generate 10 new particule each millisecond,
		// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
		// newparticles will be huge and the next frame even longer.
		int newparticles = (int)(deltaTime*100.0);
		if (newparticles > (int)(0.016f*100.0))
			newparticles = (int)(0.016f*100.0);

		for (int i = 0; i < newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			GameObject& p = ParticlesContainer[particleIndex];
			p.life = 1.5f; // This particle will live 5 seconds.
			p.specular = 0.0f;
			p.position = particlePosition;
			p.translationMatrix = defaultTranslationMatrix;
			p.thetaRotation = 0.0f;
			p.rotation = 0.0f;
			p.orientationVector = glm::vec3(0, 1.0f, 0);


			float spread = 0.3f;
			glm::vec3 maindir = glm::vec3(1.0f, 1.0f, 1.0f);
			// Very bad way to generate a random direction; 
			// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
			// combined with some user-controlled parameters (main direction, spread, etc)
			/*
			glm::vec3 randomdir = glm::vec3(
				(rand() % 2000 - 100.0f) / 100.0f,
				//0.0f,
				(rand() % 2000 - 100.0f) / 100.0f,
				//0.0f
				(rand() % 2000 - 100.0f) / 100.0f
			);
			*/
			glm::vec3 randomdir = glm::vec3(
				(rand() % 4000 - 1000.0f) / 800.0f,
				//0.0f,
				(rand() % 4000 - 1000.0f) / 800.0f,
				//0.0f
				(rand() % 4000 - 1000.0f) / 800.0f
			);

			p.speed = maindir + randomdir*spread;


			p.size = (rand() % 1000) / 2000.0f + 0.1f;

			//particlePosition += glm::vec3(0.0f, 5.0f, 0.0f);
		}

		// Generate 10 new particule each millisecond,
		// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
		// newparticles will be huge and the next frame even longer.
		int newparticlesTrain = (int)(deltaTime*100.0);
		if (newparticlesTrain > (int)(0.016f*100.0))
			newparticlesTrain = (int)(0.016f*100.0);

		for (int i = 0; i < newparticlesTrain; i++) {
			int particleIndexTrain = FindUnusedParticleTrain();
			GameObject& p = ParticlesContainerTrain[particleIndexTrain];
			p.life = 0.3f; // This particle will live 5 seconds.
			p.specular = 0.0f;
			p.position = particlePosition;
			p.translationMatrix = defaultTranslationMatrix;
			p.thetaRotation = 0.0f;
			p.rotation = 0.0f;
			p.orientationVector = glm::vec3(0, 1.0f, 0);


			float spread = 0.3f;
			glm::vec3 maindir = glm::vec3(-1.0f, 1.0f, -1.0f);
			// Very bad way to generate a random direction; 
			// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
			// combined with some user-controlled parameters (main direction, spread, etc)
			/*
			glm::vec3 randomdir = glm::vec3(
				(rand() % 2000 - 100.0f) / 100.0f,
				//0.0f,
				(rand() % 2000 - 100.0f) / 100.0f,
				//0.0f
				(rand() % 2000 - 100.0f) / 100.0f
			);
			*/
			glm::vec3 randomdir = glm::vec3(
				(rand() % 4000 - 1000.0f) / 800.0f,
				//0.0f,
				(rand() % 4000 - 1000.0f) / 800.0f,
				//0.0f
				(rand() % 4000 - 1000.0f) / 800.0f
			);

			p.speed = maindir + randomdir*spread;


			p.size = (rand() % 1000) / 2000.0f + 0.1f;

			//particlePosition += glm::vec3(0.0f, 5.0f, 0.0f);
		}

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

			if ( (i != 3) ) {

				ourShader.setVec3("material.specular", levelData.getObjectShininess(i), levelData.getObjectShininess(i), levelData.getObjectShininess(i));
				ourShader.setFloat("material.shininess", 64.0f);
				glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(levelData.getObjectPositioning(i)));
			//	glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(levelData.getObjectRotation(i)));

				levelData.getModel(i).Draw(ourShader);
			}
		}
		ourShader.setVec3("light.specular", 0.0f, 0.0f, 0.0f);

		// Simulate all particles
		int ParticlesCount = 0;
		for (int i = 0; i<MaxParticles; i++) {

			GameObject& p = ParticlesContainer[i]; // shortcut

			if (p.life > 0.0f) {

				// Decrease life
				p.life -= deltaTime;
				if (p.life > 0.0f) {

					// Simulate simple physics : gravity only, no collisions
					p.speed += glm::vec3(0.0f, 2.0f, 0.01f) * (float)deltaTime * 0.1f; //-9.81
					p.position += p.speed * (float)deltaTime * 0.1f; 
					p.cameradistance = glm::length2(p.position - CameraPosition);
					p.translationMatrix = glm::translate(p.translationMatrix, p.position);
					glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::translate(p.translationMatrix, particleOffsetPosition)));
					p.model.Draw(ourShader);
				}
				else {
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}

				ParticlesCount++;

			}
		}
		SortParticles();

	

		ourShader.setVec3("light.specular", 10.0f, 10.0f, 10.0f);

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

			/*************************************/
			/*************************************/
			/*************************************/
			/*************************************/

		
		for (int i = 0; i < numberOfTrains; i++)
		{


			if (trains[0].velocity >= 0) { // Going forward

				if (trains[i].currentSection != -1) {
					if (vertexEquality(trains[i].position, track.getTrackSection(trains[i].currentSection).at(trains[i].currentNode + 1))) {


						trains[i].currentNode++;

						if (trains[i].currentNode == track.getTrackSection(trains[i].currentSection).size() - 1) {

							trains[i].currentSection = track.getNextSection(trains[i].currentSection, 1);


							if (trains[i].currentSection == -1) {
								trains[0].velocity = 0;

								trains[i].currentNode = trains[i].currentNode - 1;
							}
							else {

								trains[i].currentNode = 0;

							}


						}
					}
				}
			}
			else { // Going backward

				if (trains[i].currentSection != -1) {

					int nextNode = trains[i].currentNode - 1;
					if (safeNodeLookup(nextNode, trains[i].currentSection)) {
						if (vertexEquality(trains[i].position, track.getTrackSection(trains[i].currentSection).at(nextNode))) {


							trains[i].currentNode--;

							if (trains[i].currentNode == 1) {

								trains[i].currentSection = track.getNextSection(trains[i].currentSection, -1);


								if (trains[i].currentSection == -1) {
									trains[0].velocity = 0;

									trains[i].currentNode = trains[i].currentNode + 1;
								}
								else {

									trains[i].currentNode = track.sections.at(trains[i].currentSection).size();

								}


							}
						}
					}
				}
			}

			// Generate the transition matrix from position
			// transitionMatrix = glm::lookAt(train.position,( train.getIteratedPosition(track1->path.at(train.currentNode + 1)) * scale), glm::vec3(0, 1, 0));

			/*
			glm::mat4 transitionMatrix = glm::lookAt(train.position,train.position + track1->path.at(train.currentNode + 1), glm::vec3(0, 10, 0));

			 transitionMatrix = glm::translate(transitionMatrix, train.getIteratedPosition(track1->path.at(train.currentNode + 1)));
			 */
			 /*
			 glm::mat4 direction = glm::lookAt(
				 train.position,
				 train.position + track1->path.at(train.currentNode + 1),
				 glm::vec3(0, 10, 0)
			 );*/
			glm::mat4 output = glm::mat4();
			glm::mat4  direction = glm::mat4();
			if (safeNodeLookup(trains[i].currentNode + 1, trains[i].currentSection)) { //train.currentSection != -1
				/*
				direction = glm::lookAt(
					glm::vec3(0, 0, 0),
					trains[i].getRotation(track.getTrackSection(trains[i].currentSection).at(trains[i].currentNode + 1)),
					glm::vec3(0, 1, 0)
				);*/

			}
			glm::mat4 translation;

			if (trains[0].velocity >= 0) {
				translation = glm::translate(glm::mat4(), trains[i].getIteratedPosition(track.getTrackSection(trains[i].currentSection).at(trains[i].currentNode + 1)));

			}
			else {
				translation = glm::translate(glm::mat4(), trains[i].getIteratedPosition(track.getTrackSection(trains[i].currentSection).at(trains[i].currentNode - 1)));


			}
			if (safeNodeLookup(trains[i].currentNode + 2, trains[i].currentSection)) { //train.currentSection != -1

				GLfloat rotation = track.getRotation(trains[i].currentSection, trains[i].currentNode, trains[i].currentNode + 2);

				if (rotation != trains[i].currentRotation) {

					trains[i].rotationTarget = rotation;
				}

			}

			translation = glm::rotate(translation, trains[i].rotationTarget + 90.0f, glm::vec3(0, 1, 0));


			glm::mat4 offset = glm::translate(glm::mat4(), trains[i].offset);

			output = translation * offset;

			ourShader.setVec3("material.specular", levelData.getObjectShininess(trains[i].gameObjectIndex), levelData.getObjectShininess(trains[i].gameObjectIndex), levelData.getObjectShininess(trains[i].gameObjectIndex));
			ourShader.setFloat("material.shininess", 64.0f);

			glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(output));

			levelData.getModel(trains[i].gameObjectIndex).Draw(ourShader);






			if (i == 0) { // Just do this once per tick

			// Simulate all particles
				for (int i = 0; i < MaxParticlesTrain; i++) {

					GameObject& p = ParticlesContainerTrain[i]; // shortcut

					if (p.life > 0.0f) {

						// Decrease life
						p.life -= deltaTime;
						if (p.life > 0.0f) {

							// Simulate simple physics : gravity only, no collisions
							p.speed += glm::vec3(0.0f, 2.0f, 0.01f) * (float)deltaTime * 0.1f; //-9.81
							p.position += p.speed * (float)deltaTime;
							p.cameradistance = glm::length2(p.position - CameraPosition);
							p.translationMatrix = glm::translate(translation, p.position + glm::vec3(1.8, 0.6, -0.2));
							//glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::translate(p.translationMatrix, trains[0].position)));
							glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(p.translationMatrix));

							p.model.Draw(ourShader);
						}
						else {
							// Particles that just died will be put at the end of the buffer in SortParticles();
							p.cameradistance = -1.0f;
						}
					}
				}
				SortParticlesTrain();

			}

		}



		
			/************************************/
			/*************************************/
			/*************************************/
			/*************************************/

		// DRAW the frame

		view = camera.GetViewMatrix();
		
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



		//DRAW OPENGL
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	/********* Close down bullet ******/
	/*
	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;

	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;


	delete fallShape;

	delete groundShape;


	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

	*/

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