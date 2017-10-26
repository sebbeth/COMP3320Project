#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <cmath>
#include <math.h> 
#include "TrackSegmentStraight.h"


int numberOfSegments = 0;

class TrackOld
{

public:

	TrackSegmentStraight *segments[100];
	TrackOld *next;



	TrackOld() {

	}

	TrackOld(glm::vec3 input[], int cardinality) {

		int segmentCounter = 0;
		for (int i = 0; i < cardinality; i++)
		{
			this->segments[segmentCounter] = new TrackSegmentStraight(input[i], input[i+1]);

			segmentCounter++;
			std::cout << "(" << input[i].x << "," << input[i].y << "," << input[i].z << ")" << std::endl;
		}

		numberOfSegments = segmentCounter -1;
	}

	TrackOld(vector<glm::vec3> input, int cardinality) {

		int segmentCounter = 0;
		for (int i = 0; i < cardinality; i++)
		{
			this->segments[segmentCounter] = new TrackSegmentStraight(input[i], input[i + 1]);

			segmentCounter++;

		}

		numberOfSegments = segmentCounter - 1;
	}

	

	void addSegment(int index, glm::vec3 a, glm::vec3 b) {

		segments[index] = new TrackSegmentStraight(a, b);
	

	}

	void setNumberOfSegments(int input)
	{
		numberOfSegments = input;
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

	TrackOld* nextTrack() {

		if (next != nullptr) {
			return next;

		}
		else {
		}
	}




private:

};