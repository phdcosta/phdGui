#include "phdGuiPushButton.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiPushButton::phdGuiPushButton(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	pushed = false;
}

phdGuiPushButton::~phdGuiPushButton() {
	phdGuiControl::~phdGuiControl();
}

void phdGuiPushButton::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	if(isPushed()) {
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

void phdGuiPushButton::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiPushButton::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
}

void phdGuiPushButton::onMouseDragged(ofMouseEventArgs & args) {
}

void phdGuiPushButton::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click
		if(getParent() != NULL) {
			phdGuiControl * _cursor = getParent()->getFirstChild();
			while(_cursor != NULL) {
				if(_cursor->getClassName() == "PHDGUIPUSHBUTTON") {
					phdGuiPushButton * _button = (phdGuiPushButton*) _cursor;
					if(_cursor->getGroup() == this->getGroup()) _button->setPushed(false);
				}
				_cursor = _cursor->getNextSibling();
			}
			this->setPushed(true);

			phdGuiEventData _data(petOnClick, this);
			ofNotifyEvent(guiEvent, _data, this);
		}
	}
}

void phdGuiPushButton::onKeyPressed(ofKeyEventArgs & args) {
}
