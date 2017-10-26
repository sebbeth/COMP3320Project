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
//GLfloat lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(36.0, 200, 200);

/********* TEST **********/



vector<glm::vec3> p1 = { 
	glm::vec3(-59.008778, -21.538118, -19.200142),
	glm::vec3(-60.600636, -21.480276, -17.909760),
	glm::vec3(-62.699883, -21.405802, -16.001411),
	glm::vec3(-65.193413, -21.317478, -13.587547),
	glm::vec3(-67.968124, -21.218090, -10.780632),
	glm::vec3(-70.910919, -21.110420, -7.693100),
	glm::vec3(-73.908707, -20.997252, -4.437416),
	glm::vec3(-76.848366, -20.881369, -1.126015),
	glm::vec3(-79.616798, -20.765554, 2.128639),
	glm::vec3(-82.100906, -20.652594, 5.214096),
	glm::vec3(-84.187592, -20.545269, 8.017899),
	glm::vec3(-85.763756, -20.446365, 10.427612),
	glm::vec3(-86.716286, -20.358664, 12.330780),
	glm::vec3(-87.197273, -20.281971, 14.565346),
	glm::vec3(-87.195847, -20.248388, 17.221851),
	glm::vec3(-86.784378, -20.251280, 20.216206),
	glm::vec3(-86.035233, -20.284006, 23.464310),
	glm::vec3(-85.020775, -20.339931, 26.882072),
	glm::vec3(-83.813370, -20.412418, 30.385387),
	glm::vec3(-82.485374, -20.494831, 33.890171),
	glm::vec3(-81.109154, -20.580530, 37.312321),
	glm::vec3(-79.757088, -20.662880, 40.567741),
	glm::vec3(-78.501526, -20.735245, 43.572338),
	glm::vec3(-77.414833, -20.790985, 46.242020),
	glm::vec3(-76.569397, -20.823467, 48.492683),
	glm::vec3(-76.182701, -20.831802, 49.575211),
	glm::vec3(-75.777779, -20.837505, 50.631088),
	glm::vec3(-75.345894, -20.841976, 51.652061),
	glm::vec3(-74.878372, -20.846619, 52.629879),
	glm::vec3(-74.366486, -20.852835, 53.556290),
	glm::vec3(-73.801521, -20.862028, 54.423042),
	glm::vec3(-73.174782, -20.875601, 55.221886),
	glm::vec3(-72.477562, -20.894955, 55.944565),
	glm::vec3(-71.701141, -20.921494, 56.582832),
	glm::vec3(-70.836830, -20.956619, 57.128429),
	glm::vec3(-69.875908, -21.001736, 57.573109),
	glm::vec3(-68.809654, -21.058243, 57.908615),
	glm::vec3(-67.088005, -21.123375, 58.561977),
	glm::vec3(-65.253815, -21.139757, 59.649292),
	glm::vec3(-63.330830, -21.116659, 61.094395),
	glm::vec3(-61.342808, -21.063349, 62.821125),
	glm::vec3(-59.313492, -20.989096, 64.753326),
	glm::vec3(-57.266632, -20.903172, 66.814827),
	glm::vec3(-55.225983, -20.814842, 68.929482),
	glm::vec3(-53.215298, -20.733379, 71.021118),
	glm::vec3(-51.258320, -20.668051, 73.013573),
	glm::vec3(-49.378811, -20.628128, 74.830696),
	glm::vec3(-47.600517, -20.622877, 76.396317),
	glm::vec3(-45.947189, -20.661572, 77.634270),
	glm::vec3(-45.044235, -20.711208, 77.999397),
	glm::vec3(-43.688370, -20.802084, 78.253433),
	glm::vec3(-41.970345, -20.925234, 78.418877),
	glm::vec3(-39.980930, -21.071690, 78.518204),
	glm::vec3(-37.810879, -21.232483, 78.573906),
	glm::vec3(-35.550949, -21.398649, 78.608452),
	glm::vec3(-33.291904, -21.561222, 78.644333),
	glm::vec3(-31.124498, -21.711233, 78.704033),
	glm::vec3(-29.139494, -21.839716, 78.810028),
	glm::vec3(-27.427654, -21.937704, 78.984810),
	glm::vec3(-26.079731, -21.996231, 79.250854),
	glm::vec3(-25.186493, -22.006332, 79.630653),
	glm::vec3(-22.720322, -21.960157, 81.177719),
	glm::vec3(-20.135668, -21.945036, 82.462166),
	glm::vec3(-17.463581, -21.952623, 83.472771),
	glm::vec3(-14.735105, -21.974575, 84.198318),
	glm::vec3(-11.981285, -22.002544, 84.627579),
	glm::vec3(-9.233167, -22.028187, 84.749344),
	glm::vec3(-6.521796, -22.043158, 84.552383),
	glm::vec3(-3.878220, -22.039110, 84.025482),
	glm::vec3(-1.333482, -22.007700, 83.157425),
	glm::vec3(1.081371, -21.940582, 81.936974),
	glm::vec3(3.335293, -21.829411, 80.352921),
	glm::vec3(5.397241, -21.665842, 78.394043),
	glm::vec3(6.620483, -21.524229, 76.889557),
	glm::vec3(7.602528, -21.369530, 75.355881),
	glm::vec3(8.376806, -21.206709, 73.793610),
	glm::vec3(8.976755, -21.040730, 72.203339),
	glm::vec3(9.435809, -20.876556, 70.585670),
	glm::vec3(9.787406, -20.719154, 68.941193),
	glm::vec3(10.064980, -20.573486, 67.270493),
	glm::vec3(10.301965, -20.444519, 65.574188),
	glm::vec3(10.531797, -20.337215, 63.852852),
	glm::vec3(10.787912, -20.256540, 62.107086),
	glm::vec3(11.103745, -20.207458, 60.337494),
	glm::vec3(11.512731, -20.194931, 58.544662)};



Track track;




PhysicsObject train(glm::vec3(-82.138649, - 20.348156, 11.300604));


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
const int MaxParticles = 100;
const int MaxParticlesTrain = 50;
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

//	track1.path = p1;


	/*
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	//dynamicsWorld->setGravity(btVector3(0, -10, 0));


	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	btCollisionShape* fallShape = new btSphereShape(1);


	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);
	







	*/



	// BULLET





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
	glm::vec3 particleOffsetPosition = glm::vec3(-58.7f, 2.0f, 40.5f);
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

		if (keys[GLFW_KEY_1]) {
			//levelData.switchTrack(1);
			train.velocity += 0.01;
			/*
			if (abs(velocity) >= maxVelocity) {
				velocity = maxVelocity;
			}*/


		}
		if (keys[GLFW_KEY_2]) {
			//levelData.switchTrack(2);
			train.velocity -= 0.01;
			if (train.velocity < 0) {
				train.velocity = 0;
			}
			
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
					p.position += p.speed * (float)deltaTime; 
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

		// Simulate all particles
		for (int i = 0; i<MaxParticlesTrain; i++) {

			GameObject& p = ParticlesContainerTrain[i]; // shortcut

			if (p.life > 0.0f) {

				// Decrease life
				p.life -= deltaTime;
				if (p.life > 0.0f) {

					// Simulate simple physics : gravity only, no collisions
					p.speed += glm::vec3(0.0f, 2.0f, 0.01f) * (float)deltaTime * 0.1f; //-9.81
					p.position += p.speed * (float)deltaTime; 
					p.cameradistance = glm::length2(p.position - CameraPosition);
					p.translationMatrix = glm::translate(p.translationMatrix, p.position);
					glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::translate(p.translationMatrix, particleOffsetPositionTrain)));
					p.model.Draw(ourShader);
				}
				else {
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}
			}
		}
		SortParticlesTrain();

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

		
			if (train.velocity >= 0) { // Going forward

				if (vertexEquality(train.position, track.getTrackSection(train.currentSection).at(train.currentNode + 1))) {

					std::cout << "Next" << endl;
					train.currentNode++;

					if (train.currentNode == track.getTrackSection(train.currentSection).size() - 1) {
						std::cout << "END" << endl;
						train.currentSection = track.getNextSection(train.currentSection, 1);
						std::cout << "New " << train.currentSection <<  endl;
						train.currentNode = 0;

					}
				}
			}
			else { // Going backward

				if (vertexEquality(train.position, track.getTrackSection(train.currentSection).at(train.currentNode - 1))) {

					std::cout << "Prev" << endl;
					train.currentNode--;

					if (train.currentNode == 0) {
						train.currentNode = track.getTrackSection(train.currentSection).size() - 1;
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

			


			glm::mat4 direction = glm::lookAt(
				glm::vec3(0, 0, 0),
				train.getRotation(track.getTrackSection(train.currentSection).at(train.currentNode + 1)),
				glm::vec3(0, 1, 0)
			);

			glm::mat4 translation = glm::translate(glm::mat4(), train.getIteratedPosition(track.getTrackSection(train.currentSection).at(train.currentNode + 1)));

			glm::mat4 output =  translation * direction;


			ourShader.setVec3("material.specular", levelData.getObjectShininess(6), levelData.getObjectShininess(6), levelData.getObjectShininess(6));
			ourShader.setFloat("material.shininess", 64.0f);

			glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(output));

			levelData.getModel(6).Draw(ourShader);

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