#include "phdGuiVerSlider.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiVerSlider::phdGuiVerSlider(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	value = 0.0;
}

phdGuiVerSlider::~phdGuiVerSlider() {
	phdGuiControl::~phdGuiControl();
}

void phdGuiVerSlider::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	ofColor _border = getManager()->getBorderColor(this);

	drawFilledBorderRectangle(0,0,getWidth(),getHeight(), getManager()->getFillColor(this), _border);

	float _pos = (1.0-value) * getHeight();
	ofLine(0.0, _pos, getWidth(), _pos); 
	drawFilledBorderRectangle(0.0, _pos, getWidth(), getHeight()-_pos, _border, _border);

	ofSetColor(getManager()->getTextColor(this));
	//ofDrawBitmapString(caption, (getWidth()-(caption.size()*8.0))/2.0, getHeight() / 2.0 + 4.0);
	//ofDrawBitmapString(caption, (getWidth()-(caption.size()*8.0))/2.0, 12.0);

	string _str = "";
	for(int i = 0; i < caption.size(); i++) _str += caption.substr(i,1) + "\n";
	ofDrawBitmapString(_str, (getWidth()-(8.0))/2.0, 12.0);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiVerSlider::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiVerSlider::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
}

void phdGuiVerSlider::onMouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;
	if(!compareMouseEventArgs(argsPress, argsDrag)) {
		value = ofMap(args.y - this->getScreenY(), this->getHeight(), 0.0, 0.0, 1.0, true);

		phdGuiEventData _data(petOnDrag, this);
		ofNotifyEvent(guiEvent, _data, this);

		argsPress = argsDrag;
	}
}

void phdGuiVerSlider::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click

		phdGuiEventData _data(petOnClick, this);
		ofNotifyEvent(guiEvent, _data, this);

	}
}

void phdGuiVerSlider::onKeyPressed(ofKeyEventArgs & args) {
}
