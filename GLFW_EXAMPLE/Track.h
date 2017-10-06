#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <cmath>
#include <math.h> 
#include "TrackSegmentStraight.h"


const int numberOfSegments = 50;

class Track
{

public:

	TrackSegmentStraight segments[numberOfSegments];

	Track() {

	}

	void addSegment(int index, glm::vec3 a, glm::vec3 b) {

		segments[index] = TrackSegmentStraight(a, b);

	}

	TrackSegmentStraight checkSegmentOvershoot(int currentSegment) {

		// If the object has overshot the segment, set segment to the next one

		return segments[0];
	}

private:

};