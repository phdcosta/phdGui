#include "phdGuiFloatField.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiFloatField::phdGuiFloatField(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	value = 0.0;
	fieldSize = 5;
	precision = 2;
}

phdGuiFloatField::~phdGuiFloatField() {
	caption = "";
}

void phdGuiFloatField::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	drawFilledBorderRectangle(0,0,getWidth(),getHeight(), getManager()->getFillColor(this), getManager()->getBorderColor(this));

	ofSetColor(getManager()->getTextColor(this));
	ofDrawBitmapString(ofToString(value,precision), 4, 12);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiFloatField::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiFloatField::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
}

void phdGuiFloatField::onMouseDragged(ofMouseEventArgs & args) {
}

void phdGuiFloatField::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click
		getManager()->getInputField().editFloat(getScreenX(), getScreenY(), getWidth(), getHeight(), this, 0, fieldSize+precision+1, precision);
		//masterInputField().editFloat(getScreenX(), getScreenY(), getWidth(), getHeight(), value, 12, precision);
	}
}

void phdGuiFloatField::onKeyPressed(ofKeyEventArgs & args) {
}
