#pragma once
class Track
{

	struct TrackSwitch
	{
		int incoming;
		int outgoingA;
		int outgoingB;
		int selected = 1;
	};

public:


	vector<vector<glm::vec3>> sections;
	//vector<glm::vec3> path;
	vector<int> nextSectionLookupForward;
	vector<int> nextSectionLookupBack;
	vector<TrackSwitch> trackSwitches;
	vector<TrackSwitch> backwardTrackSwitches;



	Track() {


		TrackSwitch t0;
		t0.incoming = 0;
		t0.outgoingA = 3;
		t0.outgoingB = 3;
		
		TrackSwitch t1;
		t1.incoming = 1;
		t1.outgoingA = 3;
		t1.outgoingB = 3;

		TrackSwitch t2;
		t2.incoming = 2;
		t2.outgoingA = -1;
		t2.outgoingB = -1;

		TrackSwitch t3;
		t3.incoming = 3;
		t3.outgoingA = 2;
		t3.outgoingB = 4;


		TrackSwitch t4;
		t4.incoming = 4;
		t4.outgoingA = 7;
		t4.outgoingB = 5;

		TrackSwitch t5;
		t5.incoming = 5;
		t5.outgoingA = 6;
		t5.outgoingB = 6;

		TrackSwitch t6;
		t6.incoming = 6;
		t6.outgoingA = 1;
		t6.outgoingB = 0;

		TrackSwitch t7;
		t7.incoming = 7;
		t7.outgoingA = 6;
		t7.outgoingB = 6;



		trackSwitches.push_back(t0);
		trackSwitches.push_back(t1);
		trackSwitches.push_back(t2);
		trackSwitches.push_back(t3);
		trackSwitches.push_back(t4);
		trackSwitches.push_back(t5);
		trackSwitches.push_back(t6);
		trackSwitches.push_back(t7);





		TrackSwitch b0;
		b0.incoming = 0;
		b0.outgoingA = 6;
		b0.outgoingB = 6;

		TrackSwitch b1;
		b1.incoming = 1;
		b1.outgoingA = 6;
		b1.outgoingB = 6;

		TrackSwitch b2;
		b2.incoming = 2;
		b2.outgoingA = 3;
		b2.outgoingB = 3;

		TrackSwitch b3;
		b3.incoming = 3;
		b3.outgoingA = 0;
		b3.outgoingB = 1;


		TrackSwitch b4;
		b4.incoming = 4;
		b4.outgoingA = 3;
		b4.outgoingB = 3;

		TrackSwitch b5;
		b5.incoming = 5;
		b5.outgoingA = 4;
		b5.outgoingB = 4;

		TrackSwitch b6;
		b6.incoming = 6;
		b6.outgoingA = 5;
		b6.outgoingB = 7;

		TrackSwitch b7;
		b7.incoming = 7;
		b7.outgoingA = 4;
		b7.outgoingB = 4;



		backwardTrackSwitches.push_back(b0);
		backwardTrackSwitches.push_back(b1);
		backwardTrackSwitches.push_back(b2);
		backwardTrackSwitches.push_back(b3);
		backwardTrackSwitches.push_back(b4);
		backwardTrackSwitches.push_back(b5);
		backwardTrackSwitches.push_back(b6);
		backwardTrackSwitches.push_back(b7);



	}


	int getNextSection(int currentTrack, int direction) {


		if (direction == 1) {

			if (trackSwitches.at(currentTrack).selected == 0) {
				return trackSwitches.at(currentTrack).outgoingA;
			}
			else {
				return trackSwitches.at(currentTrack).outgoingB;

			}
		}
		else {

			if (backwardTrackSwitches.at(currentTrack).selected == 0) {
				return backwardTrackSwitches.at(currentTrack).outgoingA;
			}
			else {
				return backwardTrackSwitches.at(currentTrack).outgoingB;

			}

		}

	}

	void addSection(vector<glm::vec3> input ) {


		sections.push_back(input);
	}



	vector<glm::vec3> getTrackSection(int i) {

		return sections.at(i);
	}

	void changeUpcomingSwitch(GLfloat direction, int currentSection, int selection ) {

		if (direction > 0) { // going forward

		trackSwitches.at(currentSection).selected = selection;

		} else{

			backwardTrackSwitches.at(currentSection).selected = selection;

		}
		std::cout << "Switch " << currentSection << ":" << selection << ":" << endl;


	}

	GLfloat getRotation(int segment, int nodeCurrent, int nextNode) {

		GLfloat value = atan2((sections.at(segment).at(nextNode).x - sections.at(segment).at(nodeCurrent).x), (sections.at(segment).at(nextNode).z - sections.at(segment).at(nodeCurrent).z));
		return value *(180.0 / 3.14159265);

	}
	

private:

};