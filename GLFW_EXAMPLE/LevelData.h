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
};


GameObject objects[4];

class LevelData
{
public:

	LevelData() {

		//Ground object
		objects[0].positioning = glm::translate(objects[0].positioning, glm::vec3(-6.0f, -3.05f, 0.0f));
		objects[0].positioning = glm::scale(objects[0].positioning, glm::vec3(0.35f, 0.35f, 0.35f));
		objects[0].positioning = glm::rotate(objects[0].positioning, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		objects[0].model.load("models/hill1.obj");


		// HOUSE
		objects[3].positioning = glm::translate(objects[0].positioning, glm::vec3(-6.0f, -3.6f, -5.0f));
		objects[3].positioning = glm::scale(objects[0].positioning, glm::vec3(0.35f, 0.35f, 0.35f));
		
		objects[3].model.load("models/House.obj");


		// Carriage 1
		objects[1].positioning = glm::translate(objects[1].positioning, glm::vec3(3.0f, -4.5f, -4.0f));
		objects[1].positioning = glm::scale(objects[1].positioning, glm::vec3(0.5f, 0.5f, 0.5f));
		objects[1].model.load("models/Carriage.obj");

		// Carriage 2
		objects[2].positioning = glm::translate(objects[2].positioning, glm::vec3(3.0f, -4.5f, 3.0f));
		objects[2].positioning = glm::scale(objects[2].positioning, glm::vec3(0.5f, 0.5f, 0.5f));
		objects[2].model.load("models/Carriage.obj");

		

	}

	Model getModel(int id) 
	{
		return objects[id].model;
	}
 
	glm::mat4 getObjectPositioning(int id) {

		return objects[id].positioning;
	}

	int size() 
	{
		return sizeof(objects);
	}

private:

};

