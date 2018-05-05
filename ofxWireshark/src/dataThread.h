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

	vector<string> linesOfData;

	void setup() {
		fileLocation = "C:\\Users\\Alex Nathanson\\Documents\\openFrameworks\\of_v0.9.8_vs_release\\apps\\myApps\\ofxWireshark\\ofxWireshark\\bin\\data\\tsharkData000000014000CC7A.txt";
	}

	void threadedFunction() {
		getFile();
		loaded = true;
	}


	void getFile() {

		// reading files via ofFile or ofBuffer is not the same format as using ofSystem to interact with files
		ofLogNotice() << "Reading Data from " << fileLocation << endl;

		ofBuffer myBuffer = ofBufferFromFile(fileLocation);// myShark.writeFullPath);

		linesOfData.clear();

		//std::cout << myBuffer.getText(); // let's see what it says

		if (myBuffer.size()) {

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
			ofLogNotice("First Line: " + linesOfData[0]);


		}
		else {
			ofLogNotice() << "Buffer does not contain anything" << endl;
		}
	}
};

