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

const int cardinality = 20;

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
		loadObject(2, "models/lake.obj", glm::vec3(-43.9515f, -11.4736f, -61.1416f), 0.1f); //-43.9515f, -11.4736f, -61.1416f

		//loadObject(3, "models/mountianMirror.obj", glm::vec3(-7.1f, 10.55f, -6.1f), 0.0f);




		/*

		track->addSegment(0,glm::vec3(-16.86,-11.4,78.5), glm::vec3(-79.4, -10.2, 50.79));
		track->addSegment(1, glm::vec3(-79.4,-10.2,50.79), glm::vec3(-92.79, -9.9, 3.7));
		track->setNumberOfSegments(2);
		
		
		track2->addSegment(0, glm::vec3(-92.79, -9.9, 3.7), glm::vec3(-58.5, -10.5, -31.6));
		track2->addSegment(1, glm::vec3(-58.5,-10.5,-31.6), glm::vec3(4.25, -5.4, 52.37));
		track2->addSegment(2, glm::vec3(4.25,-5.4,52.37), glm::vec3(-16.86, -11.4, 78.5));
		track2->setNumberOfSegments(3);


		track3->addSegment(0, glm::vec3(-92.79, -9.9, 3.7), glm::vec3(-60.61, -9.75, -43.25));
		track3->addSegment(1, glm::vec3(-60.61,-9.75,-43.25), glm::vec3(-2.1, -11.0, -64.6));
		track3->addSegment(2, glm::vec3(-2.1,-11,-64.6), glm::vec3(29.35, -8.7, -31.98));
		track3->addSegment(3, glm::vec3(29.35,-8.7,-31.98), glm::vec3(43.74, -6.55, 52.6));
		track3->addSegment(4, glm::vec3(43.74,-6.55,52.6), glm::vec3(23.2, -10.64, 75.6));
		track3->addSegment(5, glm::vec3(23.2,-10.64,75.6), glm::vec3(-16.86, -11.4, 78.5));

		track3->setNumberOfSegments(6);

		*/
	




		//vector<glm::vec3> v = objects[2].model.getVertices();

		glm::vec3 points[] = { glm::vec3(-9.2, -10.6, 76),
			glm::vec3(-53, -10.1, 36),
			glm::vec3(-67, -10, 55.1),
			glm::vec3(-68, -10, 56.1),
			glm::vec3(-69, -10, 56.1),
			glm::vec3(-70, -10, 56.1),
			glm::vec3(-71, -10, 56.1),
			glm::vec3(-72, -10, 56.1),
			glm::vec3(-73, -10, 56.1),
			glm::vec3(-74, -10, 56.1),
			glm::vec3(-65, -10.7, 46) };

		glm::vec3 cube[] = {
			glm::vec3(60.610809,  -53.240772,42.240074),
			glm::vec3(60.610809,  -53.240772,-85.561745),
			glm::vec3(60.610809,  74.561050,42.240074),
			glm::vec3(60.610809,  74.561050,-85.561745),
			glm::vec3(-67.191010,  -53.240772,42.240074),
			glm::vec3(-67.191010, -53.240772,-85.561745),
			glm::vec3(-67.191010,  74.561050,42.240074),
			glm::vec3(-67.191010,  74.561050,-85.561745)
		};

		glm::vec3 testPoints[] = {
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
			glm::vec3(11.512731, -20.194931, 58.544662)
			
		};

		



		loadObject(5, "models/CoalHopper.obj", glm::vec3(-6, -10.8, 77.1), 0.0f);
		loadObject(4, "models/SteamEngine.obj", track->segments[0]->pointA, 0.0f);

		loadObject(6, "models/SteamEngine.obj", glm::vec3(0,0,0), 0.0f);

		loadObject(11, "models/1.obj", glm::vec3(0, 0, 0), 0.0f);
		loadObject(12, "models/2.obj", glm::vec3(0, 0, 0), 0.0f);
		loadObject(13, "models/3.obj", glm::vec3(0, 0, 0), 0.0f);
		loadObject(14, "models/4.obj", glm::vec3(0, 0, 0), 0.0f);
		loadObject(15, "models/5.obj", glm::vec3(0, 0, 0), 0.0f);
		loadObject(16, "models/6.obj", glm::vec3(0, 0, 0), 0.0f);
		loadObject(17, "models/7.obj", glm::vec3(0, 0, 0), 0.0f);


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

