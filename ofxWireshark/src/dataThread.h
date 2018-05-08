#pragma once
#include "ofThread.h"
#include "ofBaseApp.h"

class dataThread : public ofThread
{
public:
	dataThread();
	~dataThread();

	bool loaded;

	string fileLocation;
	int totalLines;

	int currentLine;

	vector<string> linesOfData;

	ofBuffer myBuffer;
	ofBuffer storedBuffer;

	bool init;

	void setup(string filePath) {
		//fileLocation = "C:\\Users\\Alex Nathanson\\Documents\\openFrameworks\\of_v0.9.8_vs_release\\apps\\myApps\\ofxWireshark\\ofxWireshark\\bin\\data\\tsharkData000000014000CC7A.txt";
		fileLocation = filePath;

		currentLine = 0;
		ofLogNotice() << "Setting up!" << endl;

		init = true;

		loaded = false;

	}

	void threadedFunction() {

		while (isThreadRunning()) {
			if (loaded == false) {
				
				getFile();
				loaded = true;
			}
			
			ofSleepMillis(30);
		}
	}


	void getFile() {

		
		myBuffer = ofBufferFromFile(fileLocation);// myShark.writeFullPath);
		
		if (init) {

			// reading files via ofFile or ofBuffer is not the same format as using ofSystem to interact with files
			ofLogNotice() << "Reading Data from " << fileLocation << endl;

			storedBuffer = ofBufferFromFile(fileLocation);
			init = false;
			//linesOfData.clear();
			arrayFile(storedBuffer);
			loaded = false;
		}

		//this will crash with big files!!!
		/*if (storedBuffer.end() != myBuffer.end() ) {
			storedBuffer = myBuffer;
			arrayFile(storedBuffer);
		}*/

	}

	void arrayFile(ofBuffer aBuffer) {

		if (aBuffer.size()) {

			linesOfData.clear();

			for (ofBuffer::Line it = aBuffer.getLines().begin(), end = aBuffer.getLines().end(); it != end; ++it) {

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

			//ofLogNotice("Data Lines: " + ofToString(totalLines));
			//ofLogNotice("First Line: " + linesOfData[0]);


		}
		else {
			ofLogNotice() << "Buffer does not contain anything" << endl;
		}
	}
};

