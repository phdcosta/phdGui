#include "phdGuiPopupList.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiPopupList::phdGuiPopupList(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	curItem = -1;
}

phdGuiPopupList::~phdGuiPopupList() {
	caption = "";
}

void phdGuiPopupList::draw(float _x, float _y) {
	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	drawFilledBorderRectangle(0,0,getWidth(),getHeight(), getManager()->getFillColor(this), getManager()->getBorderColor(this));
	drawBorderRectangle(getWidth()-18.0, 0, 18.0, getHeight(), getManager()->getBorderColor(this));

	ofSetColor(getManager()->getTextColor(this));
	drawBorderTriangle(getWidth()-15.0, 3, 14, getHeight()-6, ptdBottom);
	ofDrawBitmapString(getString(curItem), 4, 12);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiPopupList::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiPopupList::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
}

void phdGuiPopupList::onMouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;
	if(!compareMouseEventArgs(argsPress, argsDrag)) {
		argsPress = argsDrag;
	}
}

void phdGuiPopupList::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click

		if(getPopupMenu() != NULL) {
			ofAddListener(getPopupMenu()->popEvent, this, &phdGuiPopupList::onPopupClick);
			getPopupMenu()->popup(getScreenX(), getScreenY() + getHeight(), this, "popupList");
		}
	}
}

void phdGuiPopupList::onKeyPressed(ofKeyEventArgs & args) {
}

void phdGuiPopupList::onPopupClick(phdPopupMenuEventData &_eventData) {
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			curItem = _eventData.item->id - 12340;
			phdGuiEventData _data(petOnChangeValue, this);
			ofNotifyEvent(guiEvent, _data, this);
		}
		ofRemoveListener(_eventData.menu->popEvent, this, &phdGuiPopupList::onPopupClick);
	}
}
