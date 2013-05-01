#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetEscapeQuitsApp(false);

	//===========================================================================================
	// NEEDS TO CALL THIS HERE TO SAVE APP DIR INSIDE OFUTILS.CPP
	//===========================================================================================
	ofSetDataPathRoot("data/");
	//===========================================================================================

	ofEnableArbTex();
	ofEnableAlphaBlending();

	outputWidth = ofGetWidth();
	outputHeight = ofGetHeight();

	phdPopupMenu * _popup = NULL;

	_popup = popupsManager().addMenu("PM_MANAGER", NULL);
	_popup->addItem("Add New Clip",			1000+0, NULL, -1);
	_popup->addItem("Add New Clip Track",	1000+1, NULL, -1);
	_popup->addItem("Add New Filter Pipe",	1000+2, NULL, -1);
	_popup->addItem("Add New Mixer",		1000+3, NULL, -1);
	_popup->addItem("Add New Designer",		1000+4, NULL, -1);
	ofAddListener(_popup->popEvent, this, &testApp::onPopupClick);

	gui.setPopupMenu(popupsManager().findByName("PM_MANAGER"));

	gui.setSize(1024, 768);

	// first panel
	//--------------------------------------------------------------
	gui.addControl(new phdGuiPanel("panel"));
	gui.control().setAlign(gamSize);
	gui.control().setRectArea(ofRandom(100,600),ofRandom(100,500),ofRandom(300,400),ofRandom(150,250));
	gui.control().setID(1000);

	gui.addControl(new phdGuiTitleBar("Background Faders"));
	gui.control().setAlign(gamTop);
	gui.control().setDimension(50,20);
	gui.control().setID(1001);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	static char * _rgbs[4] = { "red", "green", "blue", "size" };
	for(int i = 0; i < 3; i++) {
		gui.goUp(); // level up - back to panel
		phdGuiVerFader * _guiFader = (phdGuiVerFader*) gui.addControl(new phdGuiVerFader(_rgbs[i]));
		_guiFader->setFader(&fadersBackground[i]);
		gui.control().setDimension(20,50);
		gui.control().setAlign(i % 2 == 0 ? gamRight : gamLeft);
		gui.control().setID(1020 + i);
		ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);
	}

	gui.goUp(); // level up - back to panel
	videoPanel = (phdGuiVideoPanel*) gui.addControl(new phdGuiVideoPanel("video"));
	gui.control().setDimension(50,ofRandom(20,50));
	gui.control().setAlign(gamClient);
	gui.control().setID(1050);
	gui.control().setString("videos/fingers.mov", 0);
	gui.control().setValue(1.0, 0); // play
	gui.control().setValue(3.0, 3); // palindrome
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	gui.setCurrent(gui.getRoot()); // level up - back to panel

	// second panel
	//--------------------------------------------------------------
	gui.addControl(new phdGuiPanel("panel"));
	gui.control().setAlign(gamSize);
	gui.control().setRectArea(ofRandom(100,600),ofRandom(100,500),ofRandom(300,400),ofRandom(150,250));
	gui.control().setID(2000);

	gui.addControl(new phdGuiTitleBar("Circle Faders"));
	gui.control().setAlign(gamTop);
	gui.control().setDimension(50,20);
	gui.control().setID(2001);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	for(int i = 0; i < 4; i++) {
		gui.goUp(); // level up - back to panel
		phdGuiVerFader * _guiFader = (phdGuiVerFader*) gui.addControl(new phdGuiVerFader(_rgbs[i]));
		_guiFader->setFader(&fadersCircle[i]);
		gui.control().setDimension(20,50);
		gui.control().setAlign(i % 2 == 0 ? gamRight : gamLeft);
		gui.control().setID(2020 + i);
		ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);
	}

	gui.goUp(); // level up - back to panel
	gui.addControl(new phdGui2dSlider("000"));
	gui.control().setDimension(50,30);
	gui.control().setAlign(gamClient);
	gui.control().setID(2050);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	gui.setCurrent(gui.getRoot()); // level up - back to panel

	// third panel
	//--------------------------------------------------------------
	gui.addControl(new phdGuiPanel("panel"));
	gui.control().setAlign(gamSize);
	gui.control().setRectArea(ofRandom(100,600),ofRandom(100,500),ofRandom(300,400),ofRandom(220,250));
	gui.control().setID(3000);

	gui.addControl(new phdGuiTitleBar("Vertical Sliders"));
	gui.control().setAlign(gamTop);
	gui.control().setDimension(50,20);
	gui.control().setID(3001);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	for(int i = 0; i < 6; i++) {
		gui.goUp(); // level up - back to panel
		gui.addControl(new phdGuiVerSlider("slider A" + ofToString(i)));
		gui.control().setDimension(15,50);
		gui.control().setAlign(i % 2 == 0 ? gamRight : gamLeft);
		gui.control().setID(3020 + i);
		ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);
	}

	gui.goUp(); // level up - back to panel
	gui.addControl(new phdGuiButton("Button A"));
	gui.control().setDimension(50,ofRandom(20,50));
	gui.control().setAlign(gamBottom);
	gui.control().setID(3050);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	for(int i = 0; i < 6; i++) {
		gui.goUp(); // level up - back to panel
		gui.addControl(new phdGuiVerSlider("slider B" + ofToString(i)));
		gui.control().setDimension(15,50);
		gui.control().setAlign(i % 2 == 0 ? gamRight : gamLeft);
		gui.control().setID(3030 + i);
		ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);
	}

	gui.goUp(); // level up - back to panel
	gui.addControl(new phdGuiButton("Button B"));
	gui.control().setDimension(50,ofRandom(20,50));
	gui.control().setAlign(gamClient);
	gui.control().setID(3060);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	gui.setCurrent(gui.getRoot()); // level up - back to panel

	// fourth panel
	//--------------------------------------------------------------
	gui.addControl(new phdGuiPanel("panel"));
	gui.control().setAlign(gamSize);
	gui.control().setRectArea(ofRandom(100,600),ofRandom(100,500),ofRandom(300,400),ofRandom(150,250));
	gui.control().setID(4000);

	gui.addControl(new phdGuiTitleBar("Hor Sliders + RGBA"));
	gui.control().setAlign(gamTop);
	gui.control().setDimension(50,20);
	gui.control().setID(4001);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	for(int i = 0; i < 4; i++) {
		gui.goUp(); // level up - back to panel
		gui.addControl(new phdGuiHorSlider("Hor slider"));
		gui.control().setDimension(50,20);
		gui.control().setAlign(i % 2 == 0 ? gamBottom : gamTop);
		gui.control().setID(4020 + i);
		ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);
	}

	gui.goUp(); // level up - back to panel
	gui.addControl(new phdGuiRGBA("color"));
	gui.control().setDimension(80,80);
	gui.control().setWidth(80);
	gui.control().setAlign(gamClient);
	gui.control().setID(4050);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	gui.setCurrent(gui.getRoot()); // level up - back to panel

	// fifth panel
	//--------------------------------------------------------------
	gui.addControl(new phdGuiPanel("panel"));
	gui.control().setAlign(gamSize);
	gui.control().setRectArea(ofRandom(100,600),ofRandom(100,500),ofRandom(400,450),ofRandom(150,250));
	gui.control().setID(5000);

	gui.addControl(new phdGuiTitleBar("Buttons + 2dSlider"));
	gui.control().setAlign(gamTop);
	gui.control().setDimension(50,20);
	gui.control().setID(5001);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	phdGuiControlAlignmentMode _aligns[4] = { gamBottom, gamLeft, gamTop, gamRight };

	for(int i = 0; i < 8; i++) {
		gui.goUp(); // level up - back to panel
		gui.addControl(new phdGuiButton("Button 0" + ofToString(i)));
		gui.control().setDimension(ofRandom(75,80),ofRandom(20,30));
		gui.control().setAlign(_aligns[i % 4]);
		gui.control().setID(5020 + i);
		ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);
	}

	gui.goUp(); // level up - back to panel
	gui.addControl(new phdGui2dSlider("000"));
	gui.control().setDimension(50,30);
	gui.control().setAlign(gamClient);
	gui.control().setID(5050);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	gui.setCurrent(gui.getRoot()); // level up - back to panel

	// sixth panel
	//--------------------------------------------------------------
	gui.addControl(new phdGuiPanel("panel"));
	gui.control().setAlign(gamSize);
	gui.control().setRectArea(ofRandom(100,600),ofRandom(100,500),ofRandom(300,400),ofRandom(150,250));
	gui.control().setID(6000);

	gui.addControl(new phdGuiTitleBar("edit Fields + 2dSlider"));
	gui.control().setAlign(gamTop);
	gui.control().setDimension(50,20);
	gui.control().setID(6001);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	for(int i = 0; i < 3; i++) {
		gui.goUp(); // level up - back to panel
		gui.addControl(new phdGuiStringField("string"));
		gui.control().setDimension(50,20);
		gui.control().setAlign(i % 2 == 0 ? gamBottom : gamTop);
		gui.control().setID(6020 + i);
		ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

		gui.goUp(); // level up - back to panel
		gui.addControl(new phdGuiSpinFloatField("000"));
		gui.control().setDimension(50,20);
		gui.control().setAlign(i % 2 == 0 ? gamBottom : gamTop);
		gui.control().setID(6040 + i);
		ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);
	}

	gui.goUp(); // level up - back to panel
	gui.addControl(new phdGui2dSlider("000"));
	gui.control().setDimension(50,30);
	gui.control().setAlign(gamClient);
	gui.control().setID(6050);
	ofAddListener(gui.control().guiEvent, this, &testApp::onGuiEvent);

	centerX = ofGetWidth() / 2.0;
	centerY = ofGetHeight() / 2.0;

	msgMovie = "";
	lastPos = 0.0;
}

//--------------------------------------------------------------
void testApp::update(){

	gui.update();

	if(lastPos < videoPanel->getValue(2)) {
		msgMovie = " forward";
	} else if(lastPos > videoPanel->getValue(2)) {
		msgMovie = " backward";
	} else {
		msgMovie = " paused";
	}

	lastPos = videoPanel->getValue(2);
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(fadersBackground[0].getValue() * 255, fadersBackground[1].getValue() * 255, fadersBackground[2].getValue() * 255);

	ofSetColor(fadersCircle[0].getValue() * 255, fadersCircle[1].getValue() * 255, fadersCircle[2].getValue() * 255);
	ofCircle(centerX, centerY, fadersCircle[3].getValue() * 300);

	gui.draw();

	ofSetColor(255,255,255,255);

	ofSetColor(255,255,255,255);
	ofDrawBitmapString("press F10 to exit\n" + msgMovie + " :speed:" + ofToString(videoPanel->getValue(1)) + " :pos:" + ofToString(videoPanel->getValue(2)) + "::", 20, 20);
	ofDrawBitmapString(msg, 40, 50);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == ']') {

		updateOutputWindowSize(outputWidth + 128, outputHeight);

	} else if(key == '[') {

		updateOutputWindowSize(outputWidth - 128, outputHeight);

	} else if(key == '}') {

		updateOutputWindowSize(outputWidth, outputHeight + 128);

	} else if(key == '{') {

		updateOutputWindowSize(outputWidth, outputHeight - 128);

	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == OF_KEY_F10) OF_EXIT_APP(0);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::onPopupClick(phdPopupMenuEventData &_eventData) {
	if(_eventData.menu != NULL) {
		if(_eventData.eventType == petClickOnItem) {

			if(_eventData.item != NULL) msg = "clicked:" + _eventData.item->name;

		} else if(_eventData.eventType == petPopupSubMenu) {

		} else if(_eventData.eventType == petSetItemsBeforePopup) {

		} else if(_eventData.eventType == petClickOutside) {

			msg = "click outside: " + _eventData.menu->getName();

		}
	}
}

void testApp::onGuiEvent(phdGuiEventData &_eventData) {
	if(_eventData.control != NULL) {
		if(_eventData.eventType == petOnClick) {

			msg = "click: ";
			if(gui.getSelected() != NULL) msg += gui.getSelected()->getString(0);

		} else if(_eventData.eventType == petOnDrag) {

			msg = "drag: ";
			if(gui.getSelected() != NULL) msg += gui.getSelected()->getString(0);

			if(_eventData.control->getID() == 2050) {
				centerX = _eventData.control->getValue(0) * ofGetWidth();
				centerY = _eventData.control->getValue(1) * ofGetHeight();
			}

		} else if(_eventData.eventType == petOnChangeValue) {

			msg = "change value: ";
			if(gui.getSelected() != NULL) msg += gui.getSelected()->getString(0);

		} else if(_eventData.eventType == petOnChangeString) {

			msg = "change string: ";
			if(gui.getSelected() != NULL) msg += gui.getSelected()->getString(0);

		}

		for(int i = 0; i < _eventData.control->getCount(); i++) {
			msg += ":" + ofToString(_eventData.control->getValue(i));
		}

		msg += " id:" + ofToString(_eventData.control->getID());
	}
}

//--------------------------------------------------------------
void testApp::updateOutputWindowSize(int _width, int _height) {
	if((_width >= 512 && _height >= 384) && (outputWidth != _width || outputHeight != _height)) {
		outputWidth = _width;
		outputHeight = _height;
		ofSetWindowShape(_width, _height);
		ofSetWindowPosition(0,0);

		gui.setSize(_width, _height);
	}
}
