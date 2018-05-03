#pragma once
#include "ofThread.h"
#include "ofBaseApp.h" //didn't list ofBaseApp as the base class when setting up the shark class...

class sharkThread : public ofThread
{

public:
	sharkThread();
	~sharkThread();

	//string systemResponse;
	string systemStream;

	string networkInterfaces;

	void setup(string isUp) {
		ofLogNotice("Thread: ", isUp);
		//networkInterfaces = tsharkInterfaces();
		//ofLogNotice("returned", networkInterfaces);

		systemStream = "begin";

		tsharkInterfaces();
	}

	void threadedFunction() {
		ofLogNotice("Begin", "Thread");
		//dumpcap();
		tshark();
		while (isThreadRunning()) {

			//std::cin << "test" <<"\n";
			//std::getline(std::cin, systemStream);

			//systemResponse = consoleBuffer;
			//consoleBuf.getLines(std::istream);
			//std::cin >> systemResponse;
			//std::cout << systemResponse << endl;
			//tsharkInterfaces();

		}
	}


	void tsharkInterfaces() {
		string tsharkPath = "cd C:\\\"Program Files\"\\Wireshark && tshark";
		//ofLogNotice("tsharkPath", tsharkPath);
		string option = "-D";
		string tsharkCmd = tsharkPath + " " + option;
		networkInterfaces = ofSystem(tsharkCmd);
		//interfacesList = ofSystem(tsharkCmd);
		//ofLogNotice(systemResponse);
	}

	void tshark() {
		string tsharkPath = "C:\\\"Program Files\"\\Wireshark && tshark ";
		string options = "-c 20 "; //packet count
		string fileName = "testfile" + ofToString(ofGetElapsedTimeMillis);
		string writeFile = "-P > \"C:\\Users\\Alex Nathanson\\Documents\\openFrameworks\\of_v0.9.8_vs_release\\apps\\myApps\\ofxWireshark\\bin\\data\\" + fileName + ".pcap\"";
		string tsharkCmd = "cd " + tsharkPath + options + writeFile;
		ofLogNotice("cmd", tsharkCmd);
		systemStream = ofSystem(tsharkCmd);
		ofLogNotice(systemResponse);
	}

	void dumpcap() {
		string tsharkPath = "C:\\\"Program Files\"\\Wireshark && dumpcap ";
		string tsharkCmd = "cd " + tsharkPath;
		ofLogNotice("cmd", tsharkCmd);
		ofSystem(tsharkCmd);
		//ofLogNotice(systemResponse);

		std::getline(std::cin, systemStream);
	}
};

