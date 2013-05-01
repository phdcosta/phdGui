#include "phdGuiStringField.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiStringField::phdGuiStringField(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	value = "";
}

phdGuiStringField::~phdGuiStringField() {
	caption = value = "";
}

void phdGuiStringField::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	drawFilledBorderRectangle(0,0,getWidth(),getHeight(),  getManager()->getFillColor(this),  getManager()->getBorderColor(this));

	ofSetColor( getManager()->getTextColor(this));
	ofDrawBitmapString(value, 4, 12);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiStringField::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiStringField::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
}

void phdGuiStringField::onMouseDragged(ofMouseEventArgs & args) {
}

void phdGuiStringField::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click
		getManager()->getInputField().editString(getScreenX(), getScreenY(), getWidth(), getHeight(), this, 0, 30);
		//masterInputField().editString(getScreenX(), getScreenY(), getWidth(), getHeight(), value, 30);
	}
}

void phdGuiStringField::onKeyPressed(ofKeyEventArgs & args) {

}
