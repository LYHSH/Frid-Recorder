#pragma once

#include "ofMain.h"
#include "ofxRemoteVideoControlMgr.h"
#include "ofxXmlSettings.h"
#include "ofxDataSenderMgr.h"

void onHotKey(HWND handle,UINT message, WPARAM wParam, LPARAM lParam);

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();
		
		void reset();
		
		map<int, char> hotKeys;
		void registerHotKey();
		void unregisterHotKey();

		float readTimeLen;
		float readTimer;
		bool isReading;


		string numberStr;


		vector<string> icStrs;

		ofxRemoteVideoControlMgr videoMgr;

		HWND thiswnd;
		bool debug;

		bool readEndFlag;


		vector<string> remoteCmds;
		ofxDataSenderMgr senderMgr;
		ofxDataSender * sender;
};
