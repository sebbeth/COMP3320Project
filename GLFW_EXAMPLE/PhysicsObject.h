#pragma once
class PhysicsObject
{

public:

	glm::vec3 position;
	glm::vec3 rotation;
	GLfloat rotationTarget;
	GLfloat currentRotation;
	int currentNode;
	int currentSection;
	GLfloat velocity;
	GLfloat maxVelocity;
	int gameObjectIndex;
	glm::vec3 offset;

	PhysicsObject() {
		currentNode = 0;
		currentRotation = 0;
		velocity = 0.0;
		maxVelocity = 0.7;
		//offset = glm::vec3(0.8, 0, 0.8);
	}

	glm::vec3 getIteratedPosition(glm::vec3 nextNode) {


		
		position = position + (getDirectonVector(position, nextNode) * abs( velocity));



		return position;
	}

	glm::vec3 getRotation(glm::vec3 nextNode) {

		glm::vec3 output;

		output.x = nextNode.x - position.x;
		output.y = nextNode.y - position.y;	
		output.z = nextNode.z - position.z;
		
		return output;
	}


	GLfloat getDeltaRotation() {


		if (abs(rotationTarget - currentRotation) < 1) {
			currentRotation = rotationTarget;
			return currentRotation;
		}

		if (rotationTarget > currentRotation) {
			currentRotation = currentRotation + abs(rotationTarget - currentRotation) / 2;
		}
		else {
			currentRotation = currentRotation - abs(rotationTarget - currentRotation) / 2;

		}

		std::cout << "TARGET " << rotationTarget << ":" << currentRotation << endl;


		return currentRotation;

	}


	

private:



	bool vertexEquality(glm::vec3 a, glm::vec3 b) {

		if ((abs(b.x - a.x) < 0.5) &&
			(abs(b.y - a.y) < 0.5) &&
			(abs(b.z - a.z) < 0.5)) {

			return true;
		}
		else {
			return false;
		}

	}



	glm::vec3 getDirectonVector(glm::vec3 position, glm::vec3 target) {


		double x = (abs(target.x - position.x));
		double y = (abs(target.y - position.y));
		double z = (abs(target.z - position.z));

		double length = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

		// Get the normalized vector
		double x1 = ((target.x - position.x)) / length;
		double y1 = ((target.y - position.y)) / length;
		double z1 = ((target.z - position.z)) / length;

		return glm::vec3(x1, y1, z1);

	}

};