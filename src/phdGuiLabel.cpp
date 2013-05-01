#include "phdGuiLabel.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiLabel::phdGuiLabel(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
}

phdGuiLabel::~phdGuiLabel() {
	phdGuiControl::~phdGuiControl();
}

void phdGuiLabel::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	ofSetColor(getManager()->getTextColor(this));
	ofDrawBitmapString(caption, 1.0, 11.0);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiLabel::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiLabel::onMousePressed(ofMouseEventArgs & args) {
}

void phdGuiLabel::onMouseDragged(ofMouseEventArgs & args) {
}

void phdGuiLabel::onMouseReleased(ofMouseEventArgs & args) {
}

void phdGuiLabel::onKeyPressed(ofKeyEventArgs & args) {
}
