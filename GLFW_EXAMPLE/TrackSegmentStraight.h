/*
	COMP3320
	Project: Low Poly Train Simulation
	Final Module 1
	File name: TrackSegmentStraight.h
	Students:
		Jack Ratcliffe 3236537
		Sebastian Brown 3220619
*/
#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <cmath>
#include <math.h> 
#include "glm/gtc/matrix_transform.hpp"


#include <iostream> //cout

class TrackSegmentStraight
{
public:

	glm::vec3 pointA;
	glm::vec3 pointB;

	TrackSegmentStraight() {

	}

	TrackSegmentStraight(glm::vec3 pointA, glm::vec3 pointB) {
		this->pointA = pointA;
		this->pointB = pointB;

	}

	glm::vec3 calculatePositionOnSegment(double distance) {

		double x = ((pointB.x - pointA.x)) * distance;
		double y = ((pointB.y - pointA.y)) * distance;
		double z = ((pointB.z - pointA.z)) * distance;

		return glm::vec3(x, y, z);

	}

	double getSegmentLength() {

		double x = (abs(pointA.x - pointB.x));
		double y = (abs(pointA.y - pointB.y));
		double z = (abs(pointA.z - pointB.z));

		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	}


	glm::vec3 getSegmentNormal() {

		glm::vec3 crossProduct = glm::cross(glm::vec3(1.0f, 0, 0), getPathUnitVector());

		//return crossProduct;
		return glm::vec3(0, 1.0f, 0);

	}

	float getSegmentAngle() {

		float value =  atan2( (pointA.x - pointB.x) , (pointA.z - pointB.z));
		return value *(180.0 / 3.14159265) ;

	}

	glm::vec3 calculateDistanceToMoveVector(double distance) {

		double length = getSegmentLength();

		// Get the normalized vector
		double x = ((pointB.x - pointA.x)) / length;
		double y = ((pointB.y - pointA.y)) / length;
		double z = ((pointB.z - pointA.z)) / length;

		// Now multiply the normalized result by scalar distance

		x = x * distance;
		y = y * distance;
		z = z * distance;

		return glm::vec3(x, y, z);

	}

	glm::vec3 getPathUnitVector() {
	
		double length = getSegmentLength();

		// Get the normalized vector
		double x = ((pointB.x - pointA.x)) / length;
		double y = ((pointB.y - pointA.y)) / length;
		double z = ((pointB.z - pointA.z)) / length;

		return glm::vec3(x, y, z);
	}

	glm::vec3 calculateDeltaAttitude(glm::vec3 currentAttitude) {

		// Turn object so that it is parallel with this path.

		glm::vec3 unitVector = getPathUnitVector();

		double x = unitVector.x - currentAttitude.x;
		double y = unitVector.y - currentAttitude.y;
		double z = unitVector.z - currentAttitude.z;

		return glm::vec3(x,y,z);

	}

	bool objectHasOvershotThisSegment(glm::vec3 position) {


		double a = (abs(pointA.x - position.x));
		double b = (abs(pointA.y - position.y));
		double c = (abs(pointA.z - position.z));

		double objectDistanceFromStartVector = sqrt(pow(a, 2) + pow(b, 2) + pow(c, 2));


		if (objectDistanceFromStartVector > getSegmentLength())
		{
			return true;
		}
		else {
			return false;
		}

	}

	

private:
};