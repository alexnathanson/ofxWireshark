#include "ofApp.h"

/*
To Do:
filtering
animation
loop tshark 
*/
//--------------------------------------------------------------
void ofApp::setup() {
	threadOn = false;

	setupGui();

	interfacesList =  myShark.networkInterfaces;
	systemResponse = myShark.systemStream;

	hasPoints = false;

	showGui = true;

	fileFlag = false;

	//initialize these vectors
	uIP.clear();
	ipPoint.clear();

}

//--------------------------------------------------------------
void ofApp::update() {

	//if reading live data, check if file exists, and check flag
	if (mode == 0) {
		if (fileFlag) 
		{
			ofFile checkFile(retrievedPath);
			//dataFile.path() = myShark.writeFullPath;
			//std::cout << dataFile.path() << endl;
			//if (dataFile.exists() && !myData.loaded) {
			if (checkFile.getSize() > 0 && !myData.loaded) {
				std::cout << "Data file exists!" << endl;
				myData.setup(checkFile.path());
				myData.startThread();
				fileFlag = false;
			}
		}
	}
	

	if (myData.loaded) {
		sortData();
		myData.loaded = false;

		uniqueIP(dataLines);

		//assignPoints(uIP.size());

		hasPoints = true;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofBackground(100, 100, 100);

	ofSetColor(ofColor::white);
	ofDrawBitmapString("S to capture data. K to kill the capture process.", 20, 20);

	if (mode) {
		
		//ofDrawBitmapString(interfacesList, 20, 60);
		ofSetColor(ofColor::darkBlue);
		ofDrawBitmapString("Live Animation Mode", 20, 60);
		//ofDrawBitmapString(systemResponse, 20, 400);
	}
	else {
		ofSetColor(ofColor::darkBlue);
		ofDrawBitmapString("Ready to read data from file - Space bar to open file", 20, 60);
	}

	ofDrawBitmapString(amtIP + " total unique IP addresses", 20, 100);


	if (hasPoints) {
		//draw points
		ofSetColor(ofColor::orangeRed);
		drawPoints(ipPoint);

		//draw connections
		ofSetColor(ofColor::green);
		drawConnections();

		//draw text strings
		ofSetColor(ofColor::darkBlue);
		drawStrings(ipPoint);
	}

	if (showGui) {
		gui.draw();
	}
	
}


void ofApp::setupGui() {
	gui.setup();
	gui.add(mode.setup("Live Capture (0)/ Read from file (1)", false));
	gui.add(limitCapture.setup("Limit Capture", true));
	gui.add(ringToggle.setup("Ring Buffer", true));
	gui.add(captureSize.setup("Capture Size", 150, 10, 1000));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	string liveDisplay = "";

	switch (key) {
	case 's':
		myShark.setup(limitCapture, captureSize, ringToggle);
		retrievedPath = myShark.writeFullPath; // must be before the thread starts other wise it wont be able to get the data!
		retrievedPath = retrievedPath.substr(1, retrievedPath.length() - 2);//remove the quotes on the front and back
		myShark.startThread();
		fileFlag = true;
		threadOn = true;
		//ofLogNotice("file location", myShark.writeFullPath);
		break;
	case 'e':
		myShark.stopThread();
		threadOn = false;
		break;
	case 'k':
		//kill the capture process;
		killShark();
		break;
	case 'p':
		myShark.pingTest();
		myShark.startShark = false;
		break;
	/*case 'r':
		myData.startThread();
		break;*/
	case 'h':
		showGui = !showGui;
		break;
	case ' ':
		//Open the Open File Dialog
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select data file");

		//Check if the user opened a file
		if (openFileResult.bSuccess) {
			std::cout << openFileResult.getPath() << endl;
			//We have a file, check it and process it
			myData.setup(openFileResult.getPath());
			myData.startThread();

		}
		else {
			ofLogVerbose("User hit cancel");
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::exit() {
	if (threadOn) {
		myShark.stopThread();
		killShark();
	}
}

void ofApp::sortData() {
	dataLines.clear();

	//split the string into a vector by spaces
	for (int ve = 0; ve < myData.linesOfData.size(); ve++) {
		dataLines.push_back(ofSplitString(myData.linesOfData[ve], " "));
	}

	//remove empty vectors
	for (int lre = 0; lre < dataLines.size(); lre++) {
		for (int re = dataLines[lre].size() - 1; re > -1; re--) {
			if (dataLines[lre][re].empty()) {
				dataLines[lre].erase(dataLines[lre].begin() + re);
			}
		}
	}

	//should add in a way to reassamble things that we split by accident like multiword phrases inside of []
	
	/*
	ofLogNotice("Test");
	int teeest = 7;
	for (int test = 0; test < dataLines[teeest].size(); test++) {
		std::cout << dataLines[teeest][test] << endl;
	}*/
	
}

void ofApp::uniqueIP(vector< vector<string> > uipInput) {
	bool isUnique = true;
	int wIP;

	//std::cout << uipInput[0][2] << endl;
	//std::cout << uipInput[0][4] << endl;

	//loop through input
	for (int u1 = 0; u1 < uipInput.size(); u1++) {
		//loop through existing ip and check against input
		wIP = 2;
		for (int u2 = 0; u2 < uIP.size(); u2++) {
			if (uIP[u2] == uipInput[u1][2]) {
				isUnique = false;
				break;
			}
		}
		if (isUnique) {
			uIP.push_back(uipInput[u1][wIP]);

			//set coordinates
			int x = ofRandom(ofGetWidth());
			int y = ofRandom(ofGetHeight());

			ofPoint thisPoint(x, y, 0);
			ipPoint.push_back(thisPoint);
		}
		isUnique = true;	

		wIP = 4;
		for (int u2 = 0; u2 < uIP.size(); u2++) {
			if (uIP[u2] == uipInput[u1][4]) {
				isUnique = false;
				break;
			}
		}
		if (isUnique) {
			uIP.push_back(uipInput[u1][wIP]);

			//set coordinates
			int x = ofRandom(ofGetWidth());
			int y = ofRandom(ofGetHeight());

			ofPoint thisPoint(x, y, 0);
			ipPoint.push_back(thisPoint);
		}
		isUnique = true;
	}

	amtIP = ofToString(uIP.size());

}

void ofApp::drawPoints(vector <ofPoint> drawPoints) {
	for (int p = 0; p < drawPoints.size(); p++) {
		ofNoFill();
		ofDrawCircle(drawPoints[p], 20);
		ofFill();
		ofDrawEllipse(drawPoints[p], 10, 10);
	}
}

void ofApp::drawStrings(vector <ofPoint> drawPoints) {
	for (int p = 0; p < drawPoints.size(); p++) {
		ofDrawBitmapString(uIP[p], drawPoints[p]);

	}
}

void ofApp::drawConnections() {

	ofPoint drawSource;
	ofPoint drawDestination;

	//ofSetColor(ofColor::green);

	for (int dc1 = 0; dc1 < dataLines.size(); dc1++) {
		//get coordinates of source
		for (int dc2 = 0; dc2 < uIP.size(); dc2++) {
			if (uIP[dc2] == dataLines[dc1][2]) {
				drawSource = ipPoint[dc2];
				break;
			}
		}

		//get coordinates of destination
		for (int dc2 = 0; dc2 < uIP.size(); dc2++) {
			if (uIP[dc2] == dataLines[dc1][4]) {
				drawDestination = ipPoint[dc2];
				break;
			}
		}

		ofDrawLine(drawSource, drawDestination);

	}
}

void ofApp::killShark() {
	ofSystem("Taskkill /IM tshark.exe /F");
}