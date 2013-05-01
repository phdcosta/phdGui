#include "phdGuiPanel.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiPanel::phdGuiPanel(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	mouseToParent = false;
}

phdGuiPanel::~phdGuiPanel() {
	phdGuiControl::~phdGuiControl();
}

void phdGuiPanel::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	if(hasBorder()) {
		drawFilledBorderRectangle(0, 0, getWidth(), getHeight(), getManager()->getFillColor(pcmtNormal), getManager()->getBorderColor(pcmtNormal));
	} else {
		drawFilledBorderRectangle(0, 0, getWidth(), getHeight(), getManager()->getFillColor(pcmtNormal), getManager()->getFillColor(pcmtNormal));
	}

	//ofSetColor(getManager()->getTextColor(this));
	//ofDrawBitmapString(caption, (getWidth()-(caption.size()*8.0))/2.0, getHeight() / 2.0 + 4.0);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiPanel::onMouseMoved(ofMouseEventArgs & args) {
	if(sendMouseToParent() && getParent() != NULL) getParent()->onMouseMoved(args);
}

void phdGuiPanel::onMousePressed(ofMouseEventArgs & args) {
	if(sendMouseToParent() && getParent() != NULL) getParent()->onMousePressed(args);
}

void phdGuiPanel::onMouseDragged(ofMouseEventArgs & args) {
	if(sendMouseToParent() && getParent() != NULL) getParent()->onMouseDragged(args);
}

void phdGuiPanel::onMouseReleased(ofMouseEventArgs & args) {
	if(sendMouseToParent() && getParent() != NULL) getParent()->onMouseReleased(args);
}

void phdGuiPanel::onKeyPressed(ofKeyEventArgs & args) {
}
