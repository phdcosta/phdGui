#include "phdGuiSpinFloatField.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiSpinFloatField::phdGuiSpinFloatField(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	value = 0.0;
	fieldSize = 3;
	precision = 2;
	maxVariation = 200.0;
	curVariation = 0.0;
}

phdGuiSpinFloatField::~phdGuiSpinFloatField() {
	caption = "";
}

void phdGuiSpinFloatField::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	drawFilledBorderRectangle(0,0,getWidth(),getHeight(),  getManager()->getFillColor(this),  getManager()->getBorderColor(this));
	drawBorderRectangle(getWidth()-5.0, 0, 5.0, getHeight(),  getManager()->getBorderColor(this));

	ofSetColor( getManager()->getTextColor(this));
	ofDrawBitmapString(ofToString(value + curVariation, precision), 4, 12);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiSpinFloatField::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiSpinFloatField::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;

	curVariation = 0.0;
}

void phdGuiSpinFloatField::onMouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;
	if(!compareMouseEventArgs(argsPress, argsDrag)) {
		float dx = this->getScreenX() + (this->getWidth() / 2.0) - args.x;
		float dy = this->getScreenY() + (this->getHeight() / 2.0) - args.y;
		float dist = sqrtf(dx*dx+dy*dy);
		curVariation = mapEaseQuartic(dy > 0 ? dist : -dist, 500.0, maxVariation); // 500px = maxDistance

		phdGuiEventData _data(petOnDrag, this);
		ofNotifyEvent(guiEvent, _data, this);

		argsPress = argsDrag;
	}
}

void phdGuiSpinFloatField::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click
		getManager()->getInputField().editFloat(getScreenX(), getScreenY(), getWidth()-5.0, getHeight(), this, 0, fieldSize+precision+1, precision);

		phdGuiEventData _data(petOnClick, this);
		ofNotifyEvent(guiEvent, _data, this);

	} else { // drag finish
		value += curVariation;
		curVariation = 0.0;
		phdGuiEventData _data(petOnChangeValue, this);
		ofNotifyEvent(guiEvent, _data, this);
	}
}

void phdGuiSpinFloatField::onKeyPressed(ofKeyEventArgs & args) {
}
