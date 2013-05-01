#include "phdGuiVerFader.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiVerFader::phdGuiVerFader(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	value = 0.0;
	setFader(NULL);
}

phdGuiVerFader::~phdGuiVerFader() {
}

float phdGuiVerFader::getValue(int _index) {
	if(fader != NULL) {
		if(_index == 0) return fader->getValue();
		if(_index == 1) return fader->getDuration();
	}
	return value;
}

void phdGuiVerFader::setValue(float _value, int _index) {
	if(fader != NULL) {
		if(_index == 0) {
			fader->setValue(_value);
			value = _value;
		}
		if(_index == 1) fader->setDuration(_value);
	}
}

void phdGuiVerFader::update() {
	if(fader != NULL) {
		fader->update();
		value = fader->getValue();
	}
}

void phdGuiVerFader::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	if(fader != NULL) { value = fader->getValue(); }

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	if(fader != NULL) {
		fader->setCoords(getScreenX(), getScreenY(), getWidth(), getHeight());
		fader->draw(getManager()->getFillColor(this), getManager()->getBorderColor(this), getManager()->getTextColor(this));
	}

	ofSetColor(getManager()->getTextColor(this));
	string _str = "";
	for(int i = 0; i < caption.size(); i++) _str += caption.substr(i,1) + "\n";
	ofDrawBitmapString(_str, (getWidth()-(8.0))/2.0, 28.0);

	ofPopMatrix();
	ofPopStyle();
}

phdFaderHitTest phdGuiVerFader::updateHitTest(float _x, float _y) {

	hitFader = phfNone;

	if(fader != NULL) hitFader = fader->updateHitTest(_x, _y);

	return hitFader;
}

void phdGuiVerFader::onMouseMoved(ofMouseEventArgs & args) {

	updateHitTest(args.x, args.y);

}

void phdGuiVerFader::onMousePressed(ofMouseEventArgs & args) {

	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;

	updateHitTest(args.x, args.y);

	if(fader != NULL && hitFader == phfSlider) { fader->pause(); }
}

void phdGuiVerFader::onMouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;
	if(!compareMouseEventArgs(argsPress, argsDrag)) {

		if(hitFader == phfSlider || hitFader == phfDuration) {

			phdGuiEventData _data(petOnDrag, this);

			if(hitFader == phfDuration) {

				_data.tag = "DURATION";
				if(fader != NULL) fader->setPosDurationByCoord(args.x, args.y);

			} else if(hitFader == phfSlider) {

				_data.tag = "VALUE";

				if(fader != NULL) {
					fader->setValueByCoord(args.x, args.y);
					value = fader->getValue();
				}
			}

			ofNotifyEvent(guiEvent, _data, this);
		}
		argsPress = argsDrag;
	}
}

void phdGuiVerFader::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click

		if(hitFader == phfFadeIn || hitFader == phfFadeOut) {

			phdGuiEventData _data(petOnClick, this);

			if(hitFader == phfFadeIn) {
				_data.tag = "FADEIN";
				if(fader != NULL) fader->fadeIn();
			} else if(hitFader == phfFadeOut) {
				_data.tag = "FADEOUT";
				if(fader != NULL) fader->fadeOut();
			}

			ofNotifyEvent(guiEvent, _data, this);
		}
	}
}

void phdGuiVerFader::onKeyPressed(ofKeyEventArgs & args) {
}
