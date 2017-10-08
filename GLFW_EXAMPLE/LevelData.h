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
#include "Track.h"

struct GameObject {



	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;

	glm::vec3 orientationVector;

	glm::vec3 position;
	
	float thetaRotation;
	float rotation;


	Model model;
	float specular;
	Track *track;
	int segmentOnTrack;
	double positionOnSegment;
};

const int cardinality = 20;

class LevelData
{
public:

	GameObject objects[cardinality];


	LevelData() {



		// x, z, -y

		//Ground object
		loadObject(0, "models/mountian5.obj", glm::vec3(-7.1f, 10.55f, -6.1f), 0.0f); // Terrain model is offset from zero by this magic value -7.1f, 10.55f, -6.1f so that the scene can be designed in Blender
	
		//LakeSurface
		loadObject(8, "models/lake.obj", glm::vec3(-43.9515f, -11.4736f, -61.1416f), 0.5f);


		// Calibration tree
		//loadObject(1, "models/basicTree.obj", glm::vec3(-60.9108f, -9.53202f, 68.0045f), 0.0f);

		//loadObject(2, "models/House.obj", glm::vec3(-6.0f, 70.6f, -5.0f),0.0f);

		// Trees
		loadObject(3, "models/basicTree.obj", glm::vec3(56.178f, -0.943427f, 31.6635f), 0.0f);
		loadObject(4, "models/basicTree.obj", glm::vec3(62.9897f, -1.622170f, 31.6635f), 0.0f); // x, z, -y
		loadObject(5, "models/basicTree.obj", glm::vec3(58.0719f, -1.84353f, 36.3123f), 0.0f);
		loadObject(6, "models/basicTree.obj", glm::vec3(67.7139f, -7.78422f, 37.001f), 0.0f);
		loadObject(7, "models/basicTree.obj", glm::vec3(64.6147f, -2.3836f, 26.1538f), 0.0f);
	

	


		Track *track = new Track;
		Track *track2 = new Track;
		track->next = track2;
		track2->next = track;

		track->addSegment(0,glm::vec3(-9.2,-10.6,76), glm::vec3(-50,-10,73));
		track->addSegment(1, glm::vec3(-50, -10, 73), glm::vec3(-67,-10,55.1));
		track->addSegment(2, glm::vec3(-67, -10, 55.1), glm::vec3(-65,-10.7,46));

		
		
		track2->addSegment(0, glm::vec3(-65, -10.7, 46), glm::vec3(-53, -10.1, 36));
		track2->addSegment(1, glm::vec3(-53, -10.1, 36), glm::vec3(-18,-10.6,55));
		track2->addSegment(2, glm::vec3(-18, -10.6, 55), glm::vec3(-9.2, -10.6, 76));


		loadObject(10, "models/basicTree.obj", track->segments[0]->pointA, 0.0f);
		loadObject(9, "models/basicTree.obj", track->segments[0]->pointB, 0.0f);
		loadObject(12, "models/basicTree.obj", track->segments[1]->pointB, 0.0f);
		
		loadObject(13, "models/basicTree.obj", track2->segments[0]->pointA, 0.0f);
		loadObject(14, "models/basicTree.obj", track2->segments[0]->pointB, 0.0f);
		loadObject(15, "models/basicTree.obj", track2->segments[1]->pointB, 0.0f);

		loadObject(11, "models/Arrow.obj", track->segments[0]->pointA, 0.0f);

		objects[11].track = track;
		objects[11].segmentOnTrack = 0;

		objects[11].thetaRotation = 0.0f;


	}

	float getNewRotationValue(float theta, float rotation) {

		if (abs(theta - rotation) > 90.0f) { 
			return theta;

		}

		if (theta > rotation) {

			return rotation + 1;
		}
		else if (theta < rotation) {
			return rotation - 1;

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

