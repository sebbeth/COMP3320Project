#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <cmath>
#include <math.h> 
#include "TrackSegmentStraight.h"


const int numberOfSegments = 3;

class Track
{

public:

	TrackSegmentStraight *segments[10];
	Track *next;

	Track() {

		for (size_t i = 0; i < numberOfSegments; i++)
		{
			//segments[i] = NULL;
		}

	}

	void addSegment(int index, glm::vec3 a, glm::vec3 b) {

		segments[index] = new TrackSegmentStraight(a, b);

	}



	int nextSegment(int current) {
		
		if (current + 1 == numberOfSegments)
		{
			return -1;
		}
		else {
			current + 1;
		}

	}

	Track* nextTrack() {

		if (next != nullptr) {
			return next;

		}
		else {
		}
	}




private:

};