#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetVerticalSync(true);

	mesh.setMode(OF_PRIMITIVE_POINTS);
	mLines.setMode(OF_PRIMITIVE_LINES);
	mLines.enableColors();
	mLines.enableIndices();

	ofEnableDepthTest();
	glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
	glPointSize(10); // make the points bigger

	threadOn = false;

	dumClusterBool = false;

	setupGui();

	interfacesList =  myShark.networkInterfaces;
	systemResponse = myShark.systemStream;

	hasPoints = false;

	showGui = true;

	fileFlag = false;

	//initialize these vectors
	uIP.clear();
	ipPoint.clear();

	amtRings = 0;
	amtIP = "0";
	oldOffsetTime = 0;

	rotationSpeed = 60;

}

//--------------------------------------------------------------
void ofApp::update() {

	//if reading live data, check if file exists, and check flag
	if (mode == 0) {
		if (fileFlag) 
		{
			ofFile checkFile(retrievedPath);
			
			if (checkFile.getSize() > 0 && !myData.loaded) {
				std::cout << "Data file exists!" << endl;
				myData.setup(checkFile.path());
				myData.startThread();
				fileFlag = false;
				oldOffsetTime = myShark.offsetTime;
			}
		}
	}

	//offset the current amount by the amount from past buffers
	if (myShark.offsetTime != oldOffsetTime && oldOffsetTime != 0 && myShark.offsetTime != 0) {
		oldOffsetTime = myShark.offsetTime;
		amtRings++;
	}
	//FIX THIS!!! It just up 150 lines every time it loops
	totCaptures = myData.totalLines + (amtRings * captureSize);
	
	if (myData.loaded) {
		sortData();
		myData.loaded = false;

		uniqueIP(dataLines);

		hasPoints = true;
	}

	drawConnections(uIP);

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofBackground(115, 115, 115);


	ofSetColor(ofColor::white);
	ofDrawBitmapString("S to begin network sniffing", 20, 20);

	//if (mode) {
	//	
	//	//ofDrawBitmapString(interfacesList, 20, 60);
	//	ofDrawBitmapString("Live Animation Mode", 20, 60);
	//}
	//else {
	//	ofDrawBitmapString("Ready to read data from file - Space bar to open file", 20, 60);
	//}
	ofDrawBitmapString("Press h to show/ hide additional info", 20, 50);
	
	int captInt = captureSize;
	if (showGui) {
		ofDrawBitmapString(ofToString(totCaptures) + " total captures", 20, 80);
		ofDrawBitmapString(amtIP + " total unique IP addresses", 20, 110);
		ofDrawBitmapString("Ellapsed time: " + ofToString(ofGetElapsedTimeMillis()), 20, 140);
		ofDrawBitmapString("Current offset time: " + ofToString(myShark.offsetTime), 20, 170);
		ofDrawBitmapString("r = reset points, c = clustering", 20, 200);
		ofDrawBitmapString("Ring Buffer Capture Size: " + ofToString(captInt), 20, 230);
	}




	//GUI does not display properly with depth enabled.
	//if (showGui) {
	//	gui.draw();
	//}

	cam.begin();

	if (!dumClusterBool) {
		ofRotateY((ofGetElapsedTimeMillis() / rotationSpeed) % 360);
	}

	ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2);
	//ofScale(2, 2, 2); // flip the y axis and zoom in a bit
	
	if (hasPoints) {
		//animate points
		animPoints(ipPoint, uIP);

		//draw text strings
		ofSetColor(ofColor::darkBlue);
		drawStrings(ipPoint);
	}

	//point cloud of all IPs captured
	mesh.draw();
	
	//color coded lines between IPs based on protocol
	mLines.draw();
	
	cam.end();

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
	case 'r':
		resetPoints(ipPoint);
		break;
	case 'h':
		showGui = !showGui;
		break;
	case 'c':
		dumClusterBool = !dumClusterBool;
		resetPoints(ipPoint);
		break;
	case ' ': //not needed with livecapture enabled
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

	//should add in a way to reassamble things that were split by accident like multiword phrases inside of []
	
}

void ofApp::uniqueIP(vector< vector<string> > uipInput) {
	bool isUnique = true;
	int wIP;

	// uIP position [0] is IP address, position [1] is time, position [2] proto, position [3] is destination IP if sender or x if receiver
	vector<string> tempIP;

	//loop through input
	for (int u1 = 0; u1 < uipInput.size(); u1++) {
		//loop through existing ip and check against input
		wIP = 2;
		for (int u2 = 0; u2 < uIP.size(); u2++) {
			if (uIP[u2][0] == uipInput[u1][2]) {
				isUnique = false;
				//if not unique update time stamp
				uIP[u2][1] = ofToString(myShark.offsetTime + (ofToInt(uipInput[u1][1]) * 1000)); //timecode + offset
				uIP[u2][2] = uipInput[u1][5]; //protocol
				uIP[u2][3] = uipInput[u1][4]; //destination
				if (dumClusterBool) {
					ipPoint[u2] = dumbClustering(uipInput[u1]);
				}
				break;
			}
		}
		if (isUnique) {
			tempIP.clear();
			tempIP.push_back(uipInput[u1][wIP]);
			tempIP.push_back(ofToString(myShark.offsetTime + (ofToInt(uipInput[u1][1]) * 1000))); //tshark captures time as a float of seconds.
			tempIP.push_back(uipInput[u1][5]); //protocol
			tempIP.push_back(uipInput[u1][4]); //destination
			uIP.push_back(tempIP);

			ofPoint thisPoint;

			if (dumClusterBool) {
				//ofPoint thisPoint(x, y, z);
				thisPoint.set(dumbClustering(uipInput[u1]));
			}
			else {
				//set coordinates
				int x = ofRandom(ofGetWidth());
				int y = ofRandom(ofGetHeight());
				int z = 300 - ofRandom(600);

				thisPoint.set(x, y, z);
			}

			ipPoint.push_back(thisPoint);

			mesh.addColor(ofColor::red);
			//ofVec3f pos(x, y, z);
			mesh.addVertex(thisPoint);
		}
		isUnique = true;	

		wIP = 4;
		for (int u2 = 0; u2 < uIP.size(); u2++) {
			if (uIP[u2][0] == uipInput[u1][4]) {
				isUnique = false;
				//if not unique update time stamp
				uIP[u2][1] = ofToString(myShark.offsetTime + (ofToInt(uipInput[u1][1]) * 1000));
				uIP[u2][2] = uipInput[u1][5]; //protocol
				uIP[u2][3] = "x"; //destination
				if (dumClusterBool) {
					ipPoint[u2] = dumbClustering(uipInput[u1]);
				}
				break;
			}
		}
		if (isUnique) {
			tempIP.clear();
			tempIP.push_back(uipInput[u1][wIP]);
			tempIP.push_back(ofToString(myShark.offsetTime + (ofToInt(uipInput[u1][1]) * 1000)));
			tempIP.push_back(uipInput[u1][5]); //protocol
			tempIP.push_back("x"); //destination
			uIP.push_back(tempIP);

			ofPoint thisPoint;

			if (dumClusterBool) {
				//ofPoint thisPoint(x, y, z);
				thisPoint.set(dumbClustering(uipInput[u1]));
			}
			else {
				//set coordinates
				int x = ofRandom(ofGetWidth());
				int y = ofRandom(ofGetHeight());
				int z = 300 - ofRandom(600);

				thisPoint.set(x, y, z);
			}

			ipPoint.push_back(thisPoint);

			mesh.addColor(ofColor::red);
			//ofVec3f pos(x, y, z);
			mesh.addVertex(thisPoint);

			
		}
		isUnique = true;
	}

	if (dumClusterBool) {
		resetPointCloud();
	}

	amtIP = ofToString(uIP.size());

}

void ofApp::animPoints(vector <ofPoint> drawPoints, vector <vector <string> > getTime) {
	for (int p = 0; p < drawPoints.size(); p++) {
		if (getTime[p][3] != "x") {
			if (ofGetElapsedTimeMillis() - ofToInt(getTime[p][1]) < 10000) {
				//ofDrawCircle(drawPoints[p], 1 + (drawSize(ofToInt(getTime[p][1]), ofToInt(getTime[p][2])) * .3));
				ofSpherePrimitive animP;
				animP.enableColors();
				//animP
				animP.setRadius(animSize(ofToInt(getTime[p][1])));
				animP.setPosition(drawPoints[p]);
				animP.drawWireframe();
			}
		}
	}
}

void ofApp::drawStrings(vector<ofPoint> drawPoints) {
	for (int p = 0; p < drawPoints.size(); p++) {
		ofDrawBitmapString(uIP[p][0], drawPoints[p]);
	}
}

void ofApp::drawConnections(vector< vector<string> > dCinput) {

	ofPoint drawSource;
	ofPoint drawDestination;

	mLines.clear();

	string proto;
	//ofSetColor(ofColor::green);

	for (int dc1 = 0; dc1 < dCinput.size(); dc1++) {
		//check that it's the sender and it's fresh
		//std::cout << dCinput[dc1][3] << endl;
		if (dCinput[dc1][3] != "x" && ofGetElapsedTimeMillis() - ofToInt(dCinput[dc1][1]) < 10000) { 

			//getCoordinates of source
			drawSource = ipPoint[dc1];
			proto = dCinput[dc1][2];

			//for (int dc2 = 0; dc2 < uIP.size(); dc2++) {
			//	if (uIP[dc2][0] == dataLines[dc1][2]) {
			//		drawSource = ipPoint[dc2];
			//		proto = dCinput[dc1][2];
			//		break;
			//	}
			//}

			
			//get coordinates of destination
			for (int dc2 = 0; dc2 < dCinput.size(); dc2++) {
				//if the destination matches anoter sender, get that position number
				if (dCinput[dc1][3] == dCinput[dc2][0]) {
					drawDestination = ipPoint[dc2];
					//std::cout << "destination! " << drawDestination.x << " " << drawDestination.y << endl;
					break;
				}
			}
			

			//drawDestination.set(0, 0, 0);
			protocolLine(proto);
			mLines.addVertex(drawSource);
			mLines.addVertex(drawDestination);
		}
	}
}

void ofApp::killShark() {
	ofSystem("Taskkill /IM tshark.exe /F");
}

int ofApp::animSize(int capTime) {
	int size;
	int timeDif = ofGetElapsedTimeMillis() - capTime;
	size = (0.00002 * (10000 - timeDif)) * (timeDif % 1000);
	
	return size;
}

void ofApp::protocolLine(string proto) {
	
	//maybe divide these up by transport layer type?

	if (proto == "DNS") {
		mLines.addColor(ofColor::green);
	}
	else if (proto == "TCP") {
		mLines.addColor(ofColor::blue);
	}
	else if (proto == "ARP") {
		mLines.addColor(ofColor::purple);
	}
	else if (proto == "DHCP") {
		mLines.addColor(ofColor::yellow);
	}
	else if (proto == "UDP") {
		mLines.addColor(ofColor::black);
	}
	else if (proto == "HTTP" || proto == "HTTPS") {
		mLines.addColor(ofColor::orange);

	}
	else if (proto == "SSL" || proto == "TLSv1.2") {
		mLines.addColor(ofColor::pink);
	}
	else if (proto == "QUIC") {
		mLines.addColor(ofColor::lightBlue);
	}
	else {	
		mLines.addColor(255);
	}
}

void ofApp::resetPoints(vector < ofPoint> oldPoints) {

	ipPoint.clear();

	//reset point cloud
	mesh.clear();

	for (int newP = 0; newP < oldPoints.size() ; newP++) {
		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());
		int z = 300 - ofRandom(600);

		ofPoint thisPoint(x, y, z);
		ipPoint.push_back(thisPoint);

		mesh.addColor(ofColor::red);
		//ofVec3f pos(x, y, z);
		mesh.addVertex(thisPoint);
	}

}

void ofApp::resetPointCloud() {
	mesh.clear();

	for (int rPC = 0; rPC < ipPoint.size(); rPC ++) {
		mesh.addColor(ofColor::red);
		//ofVec3f pos(x, y, z);
		mesh.addVertex(ipPoint[rPC]);
	}
}

ofPoint ofApp::dumbClustering(vector<string> clusterInput) {

	int protocolMult = 1;

	//std::cout << clusterInput[5] << endl;

	if (clusterInput[5] == "UDP" || clusterInput[5] == "TCP"){
		protocolMult = 2;
	} else if(clusterInput[5] == "HTTP" || clusterInput[5] == "HTTPS" || clusterInput[5] == "SSL"){
		protocolMult = 3;
	}
	else if (clusterInput[5] == "DNS" || clusterInput[5] == "DHCP") {
		protocolMult = 4;
	}

	int x = ofRandom(ofGetWidth() / 4) * protocolMult;
	int y = ofRandom(ofGetHeight() / 4) * protocolMult;
	int z = 300 - ((ofRandom(600) / 4) * protocolMult);

	ofPoint thisCluster(x, y, z);
	return thisCluster;
}