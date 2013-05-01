#include "phdGuiHorSlider.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiHorSlider::phdGuiHorSlider(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	value = 0.0;
}

phdGuiHorSlider::~phdGuiHorSlider() {
	phdGuiControl::~phdGuiControl();
}

void phdGuiHorSlider::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	drawFilledBorderRectangle(0,0,getWidth(),getHeight(), getManager()->getFillColor(this), getManager()->getBorderColor(this));

	float _pos = value * getWidth();
	ofLine(_pos, 0.0, _pos, getHeight()); 
	drawFilledBorderRectangle(0.0, 0.0, _pos, getHeight(), getManager()->getBorderColor(this), getManager()->getBorderColor(this));

	ofSetColor(getManager()->getTextColor(this));
	ofDrawBitmapString(caption, (getWidth()-(caption.size()*8.0))/2.0, getHeight() / 2.0 + 4.0);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiHorSlider::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiHorSlider::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
}

void phdGuiHorSlider::onMouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;
	if(!compareMouseEventArgs(argsPress, argsDrag)) {
		value = ofMap(args.x - this->getScreenX(), 0, this->getWidth(), 0.0, 1.0, true);

		phdGuiEventData _data(petOnDrag, this);
		ofNotifyEvent(guiEvent, _data, this);

		argsPress = argsDrag;
	}
}

void phdGuiHorSlider::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click

		phdGuiEventData _data(petOnClick, this);
		ofNotifyEvent(guiEvent, _data, this);

	}
}

void phdGuiHorSlider::onKeyPressed(ofKeyEventArgs & args) {
}
