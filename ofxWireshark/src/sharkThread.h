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

	bool startShark = false;

	string writeTo;
	string writeFullPath;

	vector< string > linesOfData;

	int oldTextLines = 0;

	//ofBuffer::Line lastLine;

	void setup(string isUp) {
		ofLogNotice("Thread: ", isUp);
		//networkInterfaces = tsharkInterfaces();
		//ofLogNotice("returned", networkInterfaces);

		systemStream = "begin";

		tsharkInterfaces();

		startShark = false;

		writeTo = "tsharkData" + ofToString(ofGetElapsedTimeMillis);

	}

	void threadedFunction() {
		ofLogNotice("Begin", "Thread");
		//dumpcap();
		if (!startShark) {
			tshark();
			//pingTest();
			startShark = true;
		}

		while (isThreadRunning()) {

			//readText();
			
			
			//std::cin >> systemStream;
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
		string options = " -g -l -W n "; //packet count
		writeFullPath = "\"C:\\Users\\Alex Nathanson\\Documents\\openFrameworks\\of_v0.9.8_vs_release\\apps\\myApps\\ofxWireshark\\ofxWireshark\\bin\\data\\" + writeTo +".txt\"";
		string tsharkCmd = "cd " + tsharkPath + options + " > " + writeFullPath; // double ">>" would append the file
		ofLogNotice("cmd", tsharkCmd);
		systemStream = ofSystem(tsharkCmd);
		//ofLogNotice(systemResponse);
	}

	void dumpcap() {
		string tsharkPath = "C:\\\"Program Files\"\\Wireshark && dumpcap ";
		string tsharkCmd = "cd " + tsharkPath;
		ofLogNotice("cmd", tsharkCmd);
		ofSystem(tsharkCmd);
		//ofLogNotice(systemResponse);

		std::getline(std::cin, systemStream);
	}

	void pingTest() {
		//ofSystem("ping 127.0.0.1");
		ofLogNotice() << "pinging now" << endl;
		ofLogNotice() << ofSystem("ping 127.0.0.1");
	}

	void readText() {
		//ofBuffer textBuffer = ofBufferFromFile(writeFullPath);
		//int oldTextLines;
		//if (textBuffer.getLines > oldTextLines) {
		//	oldTextLines = textBuffer.getLines;

		//	linesOfData.clear();
		//	for (int line = 0; line < textBuffer.size(); line++) {
		//		//systemStream = line;
		//		linesOfData.push_back(textBuffer.getLines()[lines]);
		//	}
		//}

		//if (writeFullPath.getSize())
		ofBuffer buffer = ofBufferFromFile(writeFullPath);
		//int oldTextLines;

		if (buffer.size()) {
			
		//	if (buffer.getLines().end() != lastLine) {
					//oldTextLines = buffer.size();
		//		lastLine = buffer.getLines().end();
				//	linesOfData.clear();
				for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {

					string line = *it;

					// copy the line to draw later
					// make sure its not a empty line
					if (line.empty() == false) {
						linesOfData.push_back(line);
					}

					// print out the line
					systemStream = linesOfData[linesOfData.size()];
				}
		//	}
		}
	}
};

