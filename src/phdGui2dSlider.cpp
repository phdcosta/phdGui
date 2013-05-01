#include "phdGui2dSlider.h"

//--------------------------------------------------------------------------------------------------------------
phdGui2dSlider::phdGui2dSlider(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	values[0] = 0.0;
	values[1] = 0.0;
}

phdGui2dSlider::~phdGui2dSlider() {
	phdGuiControl::~phdGuiControl();
}

void phdGui2dSlider::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	drawFilledBorderRectangle(0,0,getWidth(),getHeight(), getManager()->getFillColor(this), getManager()->getBorderColor(this));

	//ofSetColor(getManager()->getTextColor(this));
	//ofDrawBitmapString(caption, (getWidth()-(caption.size()*8.0))/2.0, getHeight() / 2.0 + 4.0);

	float _posX = values[0] * getWidth();
	ofLine(_posX, 0.0, _posX, getHeight()); 

	float _posY = values[1] * getHeight();
	ofLine(0.0, _posY, getWidth(), _posY); 

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGui2dSlider::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGui2dSlider::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
}

void phdGui2dSlider::onMouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;
	if(!compareMouseEventArgs(argsPress, argsDrag)) {
		values[0] = ofMap(args.x - this->getScreenX(), 0, this->getWidth(), 0.0, 1.0, true);
		values[1] = ofMap(args.y - this->getScreenY(), 0.0, this->getHeight(), 0.0, 1.0, true);

		phdGuiEventData _data(petOnDrag, this);
		ofNotifyEvent(guiEvent, _data, this);

		argsPress = argsDrag;
	}
}

void phdGui2dSlider::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click

		phdGuiEventData _data(petOnClick, this);
		ofNotifyEvent(guiEvent, _data, this);

	}
}

void phdGui2dSlider::onKeyPressed(ofKeyEventArgs & args) {
}
