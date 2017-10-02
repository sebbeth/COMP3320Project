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

#include "Model.h";

struct GameObject {


	glm::mat4 positioning;
	Model model;
	float spectacular;
};

const int cardinality = 10;
GameObject objects[cardinality];

class LevelData
{
public:

	LevelData() {


		//Ground object
		loadObject(0, "models/mountian4.obj", glm::vec3(-7.1f, 10.55f, -6.1f), 0.0f); // Terrain model is offset from zero by this magic value -7.1f, 10.55f, -6.1f so that the scene can be designed in Blender
	
		//LakeSurface
		loadObject(8, "models/lake.obj", glm::vec3(-43.9515f, -11.4736f, -61.1416f), 50.0f);


		// Calibration tree
		// x, z, -y
		loadObject(1, "models/basicTree.obj", glm::vec3(-60.9108f, -9.53202f, 68.0045f), 0.0f);

		loadObject(2, "models/House.obj", glm::vec3(-6.0f, 70.6f, -5.0f),0.0f);

		// Trees
		loadObject(3, "models/basicTree.obj", glm::vec3(56.178f, -0.943427f, 31.6635f), 0.0f);
		loadObject(4, "models/basicTree.obj", glm::vec3(62.9897f, -1.622170f, 31.6635f), 0.0f); // x, z, -y
		loadObject(5, "models/basicTree.obj", glm::vec3(58.0719f, -1.84353f, 36.3123f), 0.0f);
		loadObject(6, "models/basicTree.obj", glm::vec3(67.7139f, -7.78422f, 37.001f), 0.0f);
		loadObject(7, "models/basicTree.obj", glm::vec3(64.6147f, -2.3836f, 26.1538f), 0.0f);


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

		return objects[id].positioning;
	}

	float getObjectShininess(int id) 
	{
		return objects[id].spectacular;
	}

	int size() 
	{
		return sizeof(objects);
	}

private:

	void loadObject(int index, GLchar *modelPath, glm::vec3 positioning, float spectacular)
	{


		// HOUSE
		objects[index].spectacular = spectacular;
		objects[index].positioning = glm::translate(objects[index].positioning, positioning);
		objects[index].model.load(modelPath);
	}

};

