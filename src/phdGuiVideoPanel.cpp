#include "phdGuiVideoPanel.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiVideoPanel::phdGuiVideoPanel(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
}

phdGuiVideoPanel::~phdGuiVideoPanel() {
	video.closeMovie();
	phdGuiControl::~phdGuiControl();
}

void phdGuiVideoPanel::update() {
	video.update();
}

void phdGuiVideoPanel::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	if(hasBorder()) {
		drawFilledBorderRectangle(0, 0, getWidth(), getHeight(), getManager()->getFillColor(pcmtNormal), getManager()->getBorderColor(pcmtNormal));
	} else {
		drawFilledBorderRectangle(0, 0, getWidth(), getHeight(), getManager()->getFillColor(pcmtNormal), getManager()->getFillColor(pcmtNormal));
	}

	video.draw(0, 0, getWidth(), getHeight());

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiVideoPanel::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiVideoPanel::onMousePressed(ofMouseEventArgs & args) {
}

void phdGuiVideoPanel::onMouseDragged(ofMouseEventArgs & args) {
}

void phdGuiVideoPanel::onMouseReleased(ofMouseEventArgs & args) {
}

void phdGuiVideoPanel::onKeyPressed(ofKeyEventArgs & args) {
}
