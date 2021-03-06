#pragma once

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"

#include <cmath>
#include <math.h> 

#include "Model.h";
//#include "TrackSegmentStraight.h"
#include "TrackOld.h"

struct GameObject {



	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::vec3 orientationVector;
	glm::vec3 position;
	float thetaRotation;
	float rotation;
	Model model;
	float specular;
	TrackOld *track;
	int segmentOnTrack;
	double positionOnSegment;
	//Particles
	float life;
	float cameradistance;
	float size;
	glm::vec3 speed;
	bool operator<(const GameObject& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

const int cardinality = 30;

//Track *track;
//Track *track2;
//Track *track3;

class LevelData
{
public:

	GameObject objects[cardinality];


	LevelData() {



		// x, z, -y

		//Ground object

		//-7.1f, 10.55f, -6.1f
		loadObject(0, "models/mountian6.obj", glm::vec3(0,0,0), 0.0f); // Terrain model is offset from zero by this magic value -7.1f, 10.55f, -6.1f so that the scene can be designed in Blender
		loadObject(1, "models/mountianBottom.obj", glm::vec3(-0,0,0), 0.0f);

		//LakeSurface
		loadObject(2, "models/lake.obj", glm::vec3(-36.4f, -22.0f, -55.2f), 0.1f); //-43.9515f, -11.4736f, -61.1416f

		loadObject(3, "models/mountianMirror.obj", glm::vec3(0,0,0), 0.0f);


		loadObject(5, "models/CoalHopper.obj", glm::vec3(0,0,0), 0.0f);

		loadObject(6, "models/SteamEngine.obj", glm::vec3(0,0,0), 0.0f);
		loadObject(7, "models/SteamEngine.obj", glm::vec3(0, 0, 0), 0.0f);


		loadObject(11, "models/1.obj", glm::vec3(10, 0, 0), 0.0f);
		loadObject(12, "models/2.obj", glm::vec3(10, 0, 0), 0.0f);
		loadObject(13, "models/3.obj", glm::vec3(10, 0, 0), 0.0f);
		loadObject(14, "models/4.obj", glm::vec3(10, 0, 0), 0.0f);
		loadObject(15, "models/5.obj", glm::vec3(10, 0, 0), 0.0f);
		loadObject(16, "models/6.obj", glm::vec3(10, 0, 0), 0.0f);
		loadObject(17, "models/7.obj", glm::vec3(10, 0, 0), 0.0f);
		loadObject(18, "models/8.obj", glm::vec3(10, 0, 0), 0.0f);


		/*
		objects[4].track = track;
		objects[4].segmentOnTrack = 0;
		objects[4].thetaRotation = 0.0f;

		objects[5].track = track;
		objects[5].segmentOnTrack = 0;
		objects[5].thetaRotation = 0.0f;
		*/
	



	}

	float getNewRotationValue(float theta, float rotation) {

		if (abs(theta - rotation) > 90.0f) { 
			return theta;
		}

		if (theta > rotation) {

			return rotation + 0.5 + abs((abs(theta - rotation)) / theta);
		}
		else if (theta < rotation) {
			return rotation - 0.5 - abs((abs(theta - rotation)) / theta);

		}
		else {
			return theta;
		}

	}

	
	
	void moveAlongTrack(int index, double delta) {

		// Set positionOnSegment and calculate new positioning and rotation

		objects[index].positionOnSegment = delta;
		int segmentIndex = objects[index].segmentOnTrack;

		glm::vec3 translationDelta = objects[index].track->segments[segmentIndex]->calculateDistanceToMoveVector(delta);

		objects[index].thetaRotation = objects[index].track->segments[segmentIndex]->getSegmentAngle() + 90.0f;

		if (objects[index].thetaRotation != objects[index].rotation) {
			objects[index].rotation = getNewRotationValue(objects[index].thetaRotation, objects[index].rotation);
		}

		objects[index].orientationVector = objects[index].track->segments[segmentIndex]->getSegmentNormal();

		objects[index].translationMatrix = glm::translate(objects[index].translationMatrix, translationDelta);

		objects[index].position = objects[index].position + translationDelta;

		// Now check if we need to move to a new track segment		
		if (objects[index].track->segments[segmentIndex]->objectHasOvershotThisSegment(objects[index].position)) {
			// Has overshot

			objects[index].segmentOnTrack = objects[index].track->nextSegment(segmentIndex);

			// Move object to the start of next segment so it can't drift.
			int nextSegmentOnTrack = objects[index].track->nextSegment(segmentIndex);
			if (nextSegmentOnTrack == -1) 
			{ 
				// Go to next track
				objects[index].track = objects[index].track->nextTrack();
				objects[index].segmentOnTrack = 0;
			}
			else {
				objects[index].segmentOnTrack = nextSegmentOnTrack;
			}
		}
	}

	float getVectorMagnitude(glm::vec3 v) {

		return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
	}


	float calculateAngleBetweenVectors(glm::vec3 a, glm::vec3 b) {


		float dotProduct = a.x * b.x + a.y * b.y + a.z * b.z;
	
		float value = dotProduct / (getVectorMagnitude(a) * getVectorMagnitude(b));


		float output = acos (value) * (180.0 / 3.14159265);
		

		std::cout << value << ":" << output << endl;

		return output;
	}



	
	int getCardinality()
	{
		return cardinality;
	}

	Model getModel(int id) 
	{
		return objects[id].model;
		return objects[id].model;
	}

	
	glm::mat4 getObjectPositioning(int id) 
	{

		return objects[id].translationMatrix;
	}

	

	glm::mat4 getObjectRotation(int id)
	{

		return glm::rotate(objects[id].translationMatrix, objects[id].rotation, objects[id].orientationVector);

	
	}

	void translateObject(int id, glm::vec3 input)
	{

		objects[id].translationMatrix = glm::translate(objects[id].translationMatrix, input);
	}

	float getObjectShininess(int id) 
	{
		return objects[id].specular;
	}

	int size() 
	{
		return sizeof(objects);
	}

private:

	void loadObject(int index, GLchar *modelPath, glm::vec3 positioning, float specular)
	{


		objects[index].specular = specular;
		objects[index].position = positioning;
		objects[index].thetaRotation = 0.0f;
		objects[index].rotation = 0.0f;

		objects[index].orientationVector = glm::vec3(0,1.0f,0);

		objects[index].translationMatrix = glm::translate(objects[index].translationMatrix, positioning);
		objects[index].model.load(modelPath);
	}

};

