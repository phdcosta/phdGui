#include "phdGuiTitleBar.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiTitleBar::phdGuiTitleBar(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	align = gamTop;
}

phdGuiTitleBar::~phdGuiTitleBar() {
	phdGuiControl::~phdGuiControl();
}

void phdGuiTitleBar::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	drawFilledBorderRectangle(0, 0, getWidth(), getHeight(),  getManager()->getFillColor(this),  getManager()->getBorderColor(this));

	ofSetColor( getManager()->getTextColor(this));
	ofDrawBitmapString(caption, (getWidth()-(caption.size()*8.0))/2.0, getHeight() / 2.0 + 4.0);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiTitleBar::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiTitleBar::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
}

void phdGuiTitleBar::onMouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;
	if(!compareMouseEventArgs(argsPress, argsDrag)) {
		if(getParent() != NULL) {
			float dx = argsDrag.x - argsPress.x;
			float dy = argsDrag.y - argsPress.y;
			getParent()->setPosition(getParent()->getX() + dx, getParent()->getY() + dy);

			phdGuiEventData _data(petOnDrag, this);
			ofNotifyEvent(guiEvent, _data, this);
		}
		argsPress = argsDrag;
	}
}

void phdGuiTitleBar::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click

		phdGuiEventData _data(petOnClick, this);
		ofNotifyEvent(guiEvent, _data, this);

	}
}

void phdGuiTitleBar::onKeyPressed(ofKeyEventArgs & args) {
}
