#pragma once
#include "ofThread.h"
#include "ofBaseApp.h" //didn't list ofBaseApp as the base class when setting up the shark class...

/*
file format
Number, time, source, destination, protocol, length, info
*/

class sharkThread : public ofThread
{

public:
	sharkThread();
	~sharkThread();

	//string systemResponse;
	string systemStream;

	string networkInterfaces;

	bool startShark = false;

	string writeTo;
	string writeFullPath;

	vector< string > linesOfData;

	int oldTextLines = 0;

	string cOpt;
	bool cBool;

	bool ringBool;

	void setup(bool cB, int cO, bool ringB) {
		ofLogNotice("Thread started");
		//networkInterfaces = tsharkInterfaces();
		//ofLogNotice("returned", networkInterfaces);

		systemStream = "begin";

		tsharkInterfaces();

		startShark = false;

		cBool = cB;
		cOpt = ofToString(cO);

		ringBool = ringB;

		writeTo = "tsharkData" + ofToString(currentDate()) + "_" + ofToString(currentTime());
		writeFullPath = "\"C:\\Users\\Alex Nathanson\\Documents\\openFrameworks\\of_v0.9.8_vs_release\\apps\\myApps\\ofxWireshark\\ofxWireshark\\bin\\data\\" + writeTo + ".txt\"";

	}

	void threadedFunction() {

		while (true) {
			if (!startShark) {

				tshark();

				if (!ringBool) {
					startShark = true;
				}
			
			}
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
		string cOption;

		string tsharkPath = "C:\\\"Program Files\"\\Wireshark && tshark ";

		//packet count
		if (cBool) {
			cOption = " -c " + cOpt;
		}
		else {
			cOption = "";
		}

		string options = cOption + " -g -l -W n"; 

		//string filters = "-f "predef:MyPredefinedHostOnlyFilter"";
		//writeFullPath = "\"C:\\Users\\Alex Nathanson\\Documents\\openFrameworks\\of_v0.9.8_vs_release\\apps\\myApps\\ofxWireshark\\ofxWireshark\\bin\\data\\" + writeTo +".txt\"";
		string tsharkCmd = "cd " + tsharkPath + options + " > " + writeFullPath; // double ">>" would append the file
		//ofLogNotice("cmd", tsharkCmd);
		ofSystem(tsharkCmd);

	}

	void pingTest() {
		ofLogNotice() << "pinging now" << endl;
		ofLogNotice() << ofSystem("ping 127.0.0.1");
	}

	void readText() {
		
		ofBuffer buffer = ofBufferFromFile(writeFullPath);

		if (buffer.size()) {
		
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {

				string line = *it;

				// copy the line to draw later
				// make sure its not a empty line
				if (line.empty() == false) {
					linesOfData.push_back(line);
				}

				// print out the line
				//systemStream = linesOfData[linesOfData.size()];
			}
		}
	}

	int currentDate() {
		int date = (ofGetYear() * 10000) + (ofGetMonth() * 100) + ofGetDay();
		return date;
	}

	int currentTime() {
		int time = (ofGetHours() * 10000) + (ofGetMinutes() * 100) + ofGetSeconds();
		return time;
	}

	void exit() {
		ofSystem("Taskkill /IM tshark.exe /F");
	}

};

