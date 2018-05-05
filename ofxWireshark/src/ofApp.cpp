#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	threadOn = false;

	myData.setup();
	myData.startThread();

	myShark.setup("We up");
	interfacesList =  myShark.networkInterfaces;
	systemResponse = myShark.systemStream;

	

}

//--------------------------------------------------------------
void ofApp::update() {

	/*vector<string> newVec;
	newVec.clear();*/

	if (myData.loaded) {
		dataLines.clear();

		//std::cout << myData.linesOfData[0] << endl;
		//std::cout << myData.linesOfData[0] << endl;
		//newVec = ofSplitString(myData.linesOfData[0], " ");
		
		//erase spaces 
		/*for (int test2 = newVec.size() - 1; test2 > -1 ; test2--) {
			if (newVec[test2].empty()) {
				newVec.erase(newVec.begin() + test2);
			}	
		}*/
		
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

		ofLogNotice("test");
		for (int test = 0; test < dataLines[0].size(); test++) {
			std::cout << dataLines[0][test] << endl;
		}
		myData.loaded = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(ofColor::darkBlue);
	ofDrawBitmapString(interfacesList, 20, 20);

	ofSetColor(ofColor::darkBlue);
	ofDrawBitmapString(systemResponse, 20, 400);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 's':
		myShark.startThread();
		threadOn = true;
		dataFile.path() = myShark.writeFullPath;
		ofLogNotice("file location", myShark.writeFullPath);
		break;
	case 'e':
		myShark.stopThread();
		threadOn = false;
		break;
	case 'p':
		myShark.startShark = false;
		break;
	case 'r':
		myData.startThread();
		break;
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
/*
void ofApp::tshark() {
	string tsharkPath = "C:\\\"Program Files\"\\Wireshark && tshark ";
	string options = "-c 1000 -P"; //packet count
	string fileName = "testfile" + ofToString(ofGetElapsedTimeMillis);
	string writeFile = " > \"C:\\Users\\Alex Nathanson\\Documents\\openFrameworks\\of_v0.9.8_vs_release\\apps\\myApps\\ofxWireshark\\bin\\data\\" + fileName + ".pcap\"";
	string tsharkCmd = "cd " + tsharkPath + options + " >> cout";// +writeFile;
	ofLogNotice("cmd", tsharkCmd);
	systemResponse = ofSystem(tsharkCmd);
	ofLogNotice(systemResponse);
}

void ofApp::dumpcap() {
	string tsharkPath = "C:\\\"Program Files\"\\Wireshark && tshark ";

	//string writeFile = " -w \"C:\\Users\\Alex Nathanson\\Documents\\wireshark\\" + fileName + ".pcap\"";
	string tsharkCmd = "cd " + tsharkPath + " -I stdout";
	//ofLogNotice("cmd", tsharkCmd);
	ofSystem(tsharkCmd);
	ofLogNotice(systemResponse);
}

void ofApp::tsharkInterfaces() {
	string tsharkPath = "cd C:\\\"Program Files\"\\Wireshark && tshark";
	//ofLogNotice("tsharkPath", tsharkPath);
	string option = "-D";
	string tsharkCmd = tsharkPath + " " + option;
	ofSystem(tsharkCmd);
	//interfacesList = ofSystem(tsharkCmd);
	//ofLogNotice(systemResponse);
}*/

void ofApp::exit() {
	if (threadOn) {
		myShark.stopThread();
	}
}

/*
void ofApp::getData() {

	//if (writeFullPath.getSize())

// reading files via ofFile or ofBuffer is not the same format as using ofSystem to interact with files
	string fileLocation = "C:\\Users\\Alex Nathanson\\Documents\\openFrameworks\\of_v0.9.8_vs_release\\apps\\myApps\\ofxWireshark\\ofxWireshark\\bin\\data\\tsharkData000000014000CC7A.txt";
	ofLogNotice() << "Reading Data from " << fileLocation << endl;

	ofBuffer myBuffer = ofBufferFromFile(fileLocation);// myShark.writeFullPath);

	//cout << myBuffer.getText(); // let's see what it says

	int totalLines;

	if (myBuffer.size()) {

		//	if (buffer.getLines().end() != lastLine) {
		//oldTextLines = buffer.size();
		//		lastLine = buffer.getLines().end();
		//	linesOfData.clear();
		for (ofBuffer::Line it = myBuffer.getLines().begin(), end = myBuffer.getLines().end(); it != end; ++it) {

			string line = *it;

			// copy the line to draw later
			// make sure its not a empty line
			if (line.empty() == false) {
				linesOfData.push_back(line);
			}
		}

		//throw out the last line of data, because it may be partial (only necessary if reading and writing data simultanously)
		linesOfData.erase(linesOfData.end() - 1);
		totalLines = linesOfData.size();

		ofLogNotice("Data Lines: " + ofToString(totalLines));

		// print out the line
		systemResponse = linesOfData[linesOfData.size() - 2];
		ofLogNotice("2nd to last", systemResponse);

		systemResponse = linesOfData[linesOfData.size() - 1];
		ofLogNotice("Last", systemResponse);

		//	}
	}
	else {
		ofLogNotice() << "Buffer does not contain anything" << endl;
	}
	
}*/