#include "ofApp.h"
#include "ofxIdentificationMgr.h"
#include "ofxAppControl.h"
LONG oldProc;
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_NOTICE);
	ofLogToConsole();

	//校验部分
	{
		int nums = 1;
		ofxIdentificationMgr::setExtraInfo(ofToString(nums));
		ofxIdentificationMgr::setup();
	}

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

//	//全屏抢占窗口
//	{
//		ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
//#pragma region 去除边框
//		
//		LONG lStyle = GetWindowLong(thiswnd, GWL_STYLE);
//		SetWindowLong(thiswnd, GWL_STYLE, lStyle & (~(WS_CAPTION | WS_BORDER | WS_POPUP | WS_SYSMENU | WS_SIZEBOX)));
//		SetWindowPos(thiswnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
//#pragma endregion 去除边框
//
//		debug = false;
//	}
	thiswnd = ofGetWin32Window();


	{
		for (int i = 0;i < 10;i++)
		{
			hotKeys[i + 1000] = '0' + i;
		}

		LONG temp = (LONG)(onHotKey);
		oldProc = SetWindowLongPtr(thiswnd, -4, temp);		//注册系统事件

		registerHotKey();
	}

	{
		senderMgr.setup("remote.xml");
		if (senderMgr.size())
		{
			sender = senderMgr.getSender(0);
		}
		else
		{
			sender = NULL;
		}

		ofxXmlSettings xml;
		xml.load("remote.xml");

		int nums = xml.getNumTags("cmd");
		remoteCmds.resize(nums);
		for (int i = 0;i < nums; i++)
		{
			remoteCmds[i] = xml.getValue("cmd", "", i);
		}
	}


	{
		ofxAppControl::setup("frdCardRecorderV1.0_hui");
	}

	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
	{
		ofxAppControl::udpate();
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
					if (videoMgr.size() > 0)
					{
						videoMgr.play(0, i);
					}
					if (sender)
					{
						if (i >=0 && i < remoteCmds.size())
						{
							sender->sendData(remoteCmds[i].c_str(), remoteCmds[i].size(), true);
						}
					}
					
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
// 	if ('d' == key || 'D' == key)
// 	{
// 		debug = !debug;
// 		if (debug)
// 		{
// 			ofSetWindowShape(512, 384);
// 			SetWindowPos(thiswnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
// 		}
// 		else
// 		{
// 			ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
// 			SetWindowPos(thiswnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
// 		}
// 	}


	if (!isReading)
	{
		isReading = true;
		readTimer = ofGetElapsedTimef();
	}

	char temp = key;
	//cout << temp << endl;

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
	int value = ofToInt(msg.message);
	for (auto ite = hotKeys.begin(); ite != hotKeys.end(); ite++)
	{
		if (value == ite->first)
		{
			keyPressed(ite->second);
		}
	}
}

void ofApp::exit()
{
	unregisterHotKey();
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

void ofApp::registerHotKey()
{
	for (auto ite = hotKeys.begin(); ite != hotKeys.end(); ite++)
	{
		RegisterHotKey(ofGetWin32Window(), ite->first, NULL, ite->second);
	}
}

void ofApp::unregisterHotKey()
{
	for (auto ite = hotKeys.begin(); ite != hotKeys.end(); ite++)
	{
		UnregisterHotKey(ofGetWin32Window(), ite->first);
	}
}

void onHotKey(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_HOTKEY)
	{
	//	cout << "wparam" << wParam << std::endl;
	//	cout << "lParam" << lParam << std::endl;

		ofSendMessage(ofToString(wParam));
	}
	

	CallWindowProc((WNDPROC)oldProc, handle, message, wParam, lParam);
}
