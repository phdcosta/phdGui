#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "phdGui.h"

#define PM_MANAGER		1000
#define PM_DESIGNER		2000
#define PM_OUTPUT		3000
#define PM_SHAPEIN		4000
#define PM_SHAPEMODE	4100
#define PM_SHAPEOUT		5000
#define PM_MIRROR		5100
#define PM_CLIP			6000
#define PM_GRABBERS		6100
#define PM_GENERATORS	6200
#define PM_TRACK		7000
#define PM_MAPPING		8000

enum phdViewModeType { vmtControls, vmtProjectorContents, vmtProjectorWarpers, vmtInputGrabberWarpers };

class testApp : public ofBaseApp{

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		void onPopupClick(phdPopupMenuEventData &_eventData);

		void onGuiEvent(phdGuiEventData &_eventData);

		void updateOutputWindowSize(int _width, int _height);
		float outputWidth;
		float outputHeight;

		phdGuiManager gui;

		phdGuiVideoPanel * videoPanel;

		string msg, msgMovie;

		phdFader fadersBackground[3];
		phdFader fadersCircle[4];
		float centerX, centerY;

		double lastPos;
};