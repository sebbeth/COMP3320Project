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

		double x = (abs(pointA.x - pointB.x)) * distance;
		double y = (abs(pointA.y - pointB.y)) * distance;
		double z = (abs(pointA.z - pointB.z)) * distance;

		return glm::vec3(x, y, z);

	}

	double getSegmentLength() {

		double x = (abs(pointA.x - pointB.x));
		double y = (abs(pointA.y - pointB.y));
		double z = (abs(pointA.z - pointB.z));

		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	}

	glm::vec3 calculateDistanceToMoveVector(double distance) {

		double length = getSegmentLength();

		// Get the normalized vector
		double x = (abs(pointA.x - pointB.x)) / length;
		double y = (abs(pointA.y - pointB.y)) / length;
		double z = (abs(pointA.z - pointB.z)) / length;

		// Now multiply the normalized result buy scalar distance

		x = x * distance;
		y = y * distance;
		z = z * distance;

		return glm::vec3(x, y, z);

	}

private:
};