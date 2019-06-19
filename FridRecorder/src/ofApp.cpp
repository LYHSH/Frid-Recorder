#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_NOTICE);
	ofLogToConsole();

	readTimeLen = 1.0f;

	readEndFlag = false;

	videoMgr.setup("control/video.xml");

	{
		ofxXmlSettings xml;
		xml.load("ic.xml");
		int nums = xml.getNumTags("cmd");
		for (int i = 0; i < nums; i++)
		{
			string str = xml.getValue("cmd", "8888", i);
			icStrs.push_back(str);
		}

		readTimeLen = xml.getValue("time", 1.0f);
	}

	//全屏抢占窗口
	{
		ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
#pragma region 去除边框
		thiswnd = ofGetWin32Window();
		LONG lStyle = GetWindowLong(thiswnd, GWL_STYLE);
		SetWindowLong(thiswnd, GWL_STYLE, lStyle & (~(WS_CAPTION | WS_BORDER | WS_POPUP | WS_SYSMENU | WS_SIZEBOX)));
		SetWindowPos(thiswnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
#pragma endregion 去除边框

		debug = false;
	}

	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
	if (!debug)
	{
		
	}
	
	if (isReading)
	{
		if (ofGetElapsedTimef() - readTimer > readTimeLen || readEndFlag)
		{
			cout << numberStr << endl;

			for (unsigned int i = 0; i< icStrs.size();i++)
			{
				if (numberStr == icStrs[i])
				{
					videoMgr.play(0,i);
				}
			}

			reset();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if ('d' == key || 'D' == key)
	{
		debug = !debug;
		if (debug)
		{
			ofSetWindowShape(512, 384);
			SetWindowPos(thiswnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
		}
		else
		{
			ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
			SetWindowPos(thiswnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
		}
	}


	if (!isReading)
	{
		isReading = true;
		readTimer = ofGetElapsedTimef();
	}

	char temp = key;
	cout << temp << endl;

	if (temp != '\r')			//结尾标记
	{
		numberStr += ofToString(temp);
	}
	else
	{
		readEndFlag = true;
	}
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::reset()
{
	isReading = false;
	readTimer = ofGetElapsedTimef();

	numberStr = "";

	readEndFlag = false;
}
