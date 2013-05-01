#include "phdGuiButton.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiButton::phdGuiButton(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	isMouseDown = false;
}

phdGuiButton::~phdGuiButton() {
	phdGuiControl::~phdGuiControl();
}

void phdGuiButton::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	if(isMouseDown) {
		drawFilledBorderRectangle(0, 0, getWidth(), getHeight(), getManager()->getBorderColor(this), getManager()->getBorderColor(this));
	} else {
		drawFilledBorderRectangle(0, 0, getWidth(), getHeight(), getManager()->getFillColor(this), getManager()->getBorderColor(this));
	}
	ofSetColor(getManager()->getTextColor(this));
	ofDrawBitmapString(caption, (getWidth()-(caption.size()*8.0))/2.0, getHeight() / 2.0 + 4.0);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiButton::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiButton::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
	isMouseDown = true;
}

void phdGuiButton::onMouseDragged(ofMouseEventArgs & args) {
}

void phdGuiButton::onMouseReleased(ofMouseEventArgs & args) {
	isMouseDown = false;
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click

		phdGuiEventData _data(petOnClick, this);
		ofNotifyEvent(guiEvent, _data, this);

	}
}

void phdGuiButton::onKeyPressed(ofKeyEventArgs & args) {
}
