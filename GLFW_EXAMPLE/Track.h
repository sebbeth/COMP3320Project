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



	Track() {

		nextSectionLookupForward.push_back(3); // 0 -> 3
		nextSectionLookupForward.push_back(3); // 1 -> 3
		nextSectionLookupForward.push_back(-1); // 2 -> nill
		nextSectionLookupForward.push_back(2); // 3 -> 4
		nextSectionLookupForward.push_back(6); // 4 -> 6
		nextSectionLookupForward.push_back(6); // 5 -> 6
		nextSectionLookupForward.push_back(1); // 6 -> 0

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


	

		nextSectionLookupBack.push_back(3); // 0 -> 3
		nextSectionLookupBack.push_back(3); // 1 -> 3
		nextSectionLookupBack.push_back(-1); // 2 -> nill
		nextSectionLookupBack.push_back(4); // 3 -> 4
		nextSectionLookupBack.push_back(6); // 4 -> 6
		nextSectionLookupBack.push_back(6); // 5 -> 6
		nextSectionLookupBack.push_back(0); // 6 -> 0



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

		std::cout << "Switch " << currentSection << ":" << selection << ":" << endl;

		}

	}
	

private:

};