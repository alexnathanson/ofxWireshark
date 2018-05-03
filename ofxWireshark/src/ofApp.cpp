#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	threadOn = false;

	myShark.setup("We up");
	interfacesList =  myShark.networkInterfaces;
	systemResponse = myShark.systemStream;
}

//--------------------------------------------------------------
void ofApp::update() {
	/*
	resources
	https://www.wireshark.org/lists/wireshark-users/200811/msg00012.html
	https://osqa-ask.wireshark.org/questions/38939/pipe-tshark-output-to-java-program
	*/
	//std::cin >> systemResponse;
	systemResponse = myShark.systemStream;
}

/*
void ofApp::threadedFunction() {
	tshark();

	while (isThreadRunning()) {

		//std::cin << "test" <<"\n";
		std::getline(std::cin, consoleBuffer);

		systemResponse = consoleBuffer;
		//consoleBuf.getLines(std::istream);
		//std::cin >> systemResponse;
		//std::cout << systemResponse << endl;
		//tsharkInterfaces();

	}
}*/

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
		break;
	case 'e':
		myShark.stopThread();
		threadOn = false;
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