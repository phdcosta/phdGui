#include "phdGuiRGBA.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiRGBA::phdGuiRGBA(string _caption, phdTreeNodeData * _nodedata) {

	caption = _caption;
	setDimension(60,40);

	setColor(NULL);

	sliders[0] = (phdGuiVerSlider*) addLastChild(new phdGuiVerSlider("r"));
	sliders[0]->setDimension(12,15);
	sliders[0]->setAlign(gamLeft);
	sliders[0]->setValue(1.0, 0);
	ofAddListener(sliders[0]->guiEvent, this, &phdGuiRGBA::onGuiEvent);

	sliders[1] = (phdGuiVerSlider*) addLastChild(new phdGuiVerSlider("g"));
	sliders[1]->setDimension(12,15);
	sliders[1]->setAlign(gamLeft);
	sliders[1]->setValue(1.0, 0);
	ofAddListener(sliders[1]->guiEvent, this, &phdGuiRGBA::onGuiEvent);

	sliders[2] = (phdGuiVerSlider*) addLastChild(new phdGuiVerSlider("b"));
	sliders[2]->setDimension(12,15);
	sliders[2]->setAlign(gamLeft);
	sliders[2]->setValue(1.0, 0);
	ofAddListener(sliders[2]->guiEvent, this, &phdGuiRGBA::onGuiEvent);

	sliders[3] = (phdGuiVerSlider*) addLastChild(new phdGuiVerSlider("a"));
	sliders[3]->setDimension(12,15);
	sliders[3]->setAlign(gamLeft);
	sliders[3]->setValue(1.0, 0);
	ofAddListener(sliders[3]->guiEvent, this, &phdGuiRGBA::onGuiEvent);
}

phdGuiRGBA::~phdGuiRGBA() {
}

void phdGuiRGBA::setColor(ofColor * _color) {

	color = _color;

	if(color != NULL) {
		if(sliders[0] != NULL) sliders[0]->setValue(color->r / 255.0);
		if(sliders[1] != NULL) sliders[1]->setValue(color->g / 255.0);
		if(sliders[2] != NULL) sliders[2]->setValue(color->b / 255.0);
		if(sliders[3] != NULL) sliders[3]->setValue(color->a / 255.0);
	}
}

// override this to setID for all sliders
void phdGuiRGBA::setID(int _id) {
	if(_id != id) {
		id = _id;
		for(int i = 0; i < 4; i++) {
			if(sliders[i] != NULL) { sliders[i]->setID(id); }
		}
	}
}

void phdGuiRGBA::setWidth(float _value) {

	float _width = 15;
	int _count = getChildCount();
	if(_count != 0) _width = (getWidth() - (_count*(getManager()->getGap()+0.5))) / (float)_count;

	phdGuiControl * _cursor = getFirstChild();
	while(_cursor != NULL) {
		_cursor->setWidth(_width);
		_cursor = _cursor->getNextSibling();
	}
}

// use these methods to control internal values
float phdGuiRGBA::getValue(int _index) {
	if(sliders[_index % 4] != NULL) return sliders[_index % 4]->getValue(0);
	return 0.0;
}

void phdGuiRGBA::setValue(float _value, int _index) {
	if(sliders[_index % 4] != NULL && sliders[_index % 4]->getValue(0) != _value) {

		sliders[_index % 4]->setValue(_value, 0);

		if(color != NULL) color->v[_index % 4] = ofMap(_value, 0, 255, 0, 1.0, true);

		phdGuiEventData _data(petOnChangeValue, this);
		ofNotifyEvent(guiEvent, _data, this);
	}
}

void phdGuiRGBA::draw(float _x, float _y) {
	drawChildren(_x,_y);
}

void phdGuiRGBA::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiRGBA::onMousePressed(ofMouseEventArgs & args) {
}

void phdGuiRGBA::onMouseDragged(ofMouseEventArgs & args) {
}

void phdGuiRGBA::onMouseReleased(ofMouseEventArgs & args) {
}

void phdGuiRGBA::onKeyPressed(ofKeyEventArgs & args) {
}

void phdGuiRGBA::onGuiEvent(phdGuiEventData &_eventData) {
	if(_eventData.control != NULL) {
		if(_eventData.eventType == petOnDrag) { // drag on RGBA slider

			if(color != NULL && _eventData.control != NULL) {
				for(int i = 0; i < 4; i++) {
					if(_eventData.control == sliders[i]) color->v[i] = ofMap(sliders[i]->getValue(), 0, 1, 0, 255, true);
				}
			}

			phdGuiEventData _data(petOnChangeValue, this);
			ofNotifyEvent(guiEvent, _data, this);
		}
	}
}
