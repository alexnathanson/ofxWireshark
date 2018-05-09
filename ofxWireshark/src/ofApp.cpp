#include "ofApp.h"

/*
To Do:
filtering
animation
visual differentation - sender/receiver, port, etc.
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
		ofDrawBitmapString("Live Animation Mode", 20, 60);
		//ofDrawBitmapString(systemResponse, 20, 400);
	}
	else {
		ofDrawBitmapString("Ready to read data from file - Space bar to open file", 20, 60);
	}

	ofDrawBitmapString(amtIP + " total unique IP addresses", 20, 100);
	ofDrawBitmapString("Ellapsed time: " + ofToString(ofGetElapsedTimeMillis()), 20, 140);
	ofDrawBitmapString("Current offset time: " + ofToString(myShark.offsetTime), 20, 180);




	if (hasPoints) {
		//draw points
		ofSetColor(ofColor::orangeRed);
		drawPoints(ipPoint, uIP);

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
	
}

void ofApp::uniqueIP(vector< vector<string> > uipInput) {
	bool isUnique = true;
	int wIP;

	//std::cout << uipInput[0][2] << endl;

	//for uIP position [0] is IP address, position [1] is time, should position [2] be send or receive?
	vector<string> tempIP;

	//loop through input
	for (int u1 = 0; u1 < uipInput.size(); u1++) {
		//loop through existing ip and check against input
		wIP = 2;
		for (int u2 = 0; u2 < uIP.size(); u2++) {
			if (uIP[u2][0] == uipInput[u1][2]) {
				isUnique = false;
				//if not unique update time stamp
				uIP[u2][1] = ofToString(myShark.offsetTime + (ofToInt(uipInput[u1][1]) * 1000));
				break;
			}
		}
		if (isUnique) {
			tempIP.clear();
			tempIP.push_back(uipInput[u1][wIP]);
			tempIP.push_back(ofToString(myShark.offsetTime + (ofToInt(uipInput[u1][1]) * 1000))); //tshark captures time as a float of seconds.
			uIP.push_back(tempIP);

			//set coordinates
			int x = ofRandom(ofGetWidth());
			int y = ofRandom(ofGetHeight());

			ofPoint thisPoint(x, y, 0);
			ipPoint.push_back(thisPoint);
		}
		isUnique = true;	

		wIP = 4;
		for (int u2 = 0; u2 < uIP.size(); u2++) {
			if (uIP[u2][0] == uipInput[u1][4]) {
				isUnique = false;
				//if not unique update time stamp
				uIP[u2][1] = ofToString(myShark.offsetTime + (ofToInt(uipInput[u1][1]) * 1000));
				break;
			}
		}
		if (isUnique) {
			tempIP.clear();
			tempIP.push_back(uipInput[u1][wIP]);
			tempIP.push_back(ofToString(myShark.offsetTime + (ofToInt(uipInput[u1][1]) * 1000)));
			uIP.push_back(tempIP);

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

void ofApp::drawPoints(vector <ofPoint> drawPoints, vector <vector <string> > getTime) {
	for (int p = 0; p < drawPoints.size(); p++) {
		ofNoFill();
		ofDrawCircle(drawPoints[p], drawSize(ofToInt(getTime[p][1])));
		ofFill();
		ofDrawCircle(drawPoints[p], 1 + (drawSize(ofToInt(getTime[p][1])) * .3));
	}
}

void ofApp::drawStrings(vector<ofPoint> drawPoints) {
	for (int p = 0; p < drawPoints.size(); p++) {
		ofDrawBitmapString(uIP[p][0], drawPoints[p]);
	}
}

void ofApp::drawConnections() {

	ofPoint drawSource;
	ofPoint drawDestination;

	string proto;
	//ofSetColor(ofColor::green);

	for (int dc1 = 0; dc1 < dataLines.size(); dc1++) {
		//get coordinates of source
		for (int dc2 = 0; dc2 < uIP.size(); dc2++) {
			if (uIP[dc2][0] == dataLines[dc1][2]) {
				drawSource = ipPoint[dc2];
				proto = dataLines[dc1][5];
				break;
			}
		}

		//get coordinates of destination
		for (int dc2 = 0; dc2 < uIP.size(); dc2++) {
			if (uIP[dc2][0] == dataLines[dc1][4]) {
				drawDestination = ipPoint[dc2];
				break;
			}
		}

		protocolLine(proto, drawSource, drawDestination);

	}
}

void ofApp::killShark() {
	ofSystem("Taskkill /IM tshark.exe /F");
}

int ofApp::drawSize(int capTime) {
	int size;

	if (ofGetElapsedTimeMillis() - capTime < 7000) {
		size = 20 * sin(ofGetElapsedTimeMillis() - capTime);
	}
	else {
		size = 1;
	}
	
	return size;
}

void ofApp::protocolLine(string proto, ofPoint srcP, ofPoint dstP) {
	
	//maybe divide these up by transport layer type?

	if (proto == "DNS") {
		ofSetColor(ofColor::red);
		ofDrawLine(srcP, dstP);
	}
	else if (proto == "TCP") {
		ofSetColor(ofColor::green);
		ofDrawLine(srcP, dstP);
	}
	else if (proto == "ARP") {
		ofSetColor(ofColor::black);
		ofDrawLine(srcP, dstP); //straight line
	}
	else if (proto == "DHCP") {
		ofSetColor(ofColor::hotPink);
		ofDrawLine(srcP, dstP);
	}
	else if (proto == "UDP") {
		ofSetColor(ofColor::yellow);
		ofDrawLine(srcP, dstP);
	}
	else if (proto == "HTTP" || proto == "HTTPS") {
		ofSetColor(ofColor::purple);
		ofDrawLine(srcP, dstP);
	}
	else if (proto == "SSL" || proto == "TLSv1.2") {
		ofSetColor(ofColor::turquoise);
		ofDrawLine(srcP, dstP);
	}
	else if (proto == "QUIC") {
		ofSetColor(ofColor::tan);
		ofDrawLine(srcP, dstP);
	}
	else {
		ofSetColor(ofColor::blue);
		ofDrawLine(srcP, dstP);
	}
}