#include "phdFader.h"

//--------------------------------------------------------------------------------------------------
phdFader::phdFader(float _duration) {
	duration = _duration;
	value = 0.0;
	action = pbfaNone;

	posDuration = duration / PHDMAXFADERTIME; // 10 secs = max fader time
}

void phdFader::recalculate() {
	currentTime = ofGetElapsedTimef();
	if(action == pbfaFadeIn) {
		futureEndTime = currentTime + (duration * (1.0 - value));
	} else if(action == pbfaFadeOut) {
		futureEndTime = currentTime + (duration * value);
	}
}

void phdFader::fadeIn(bool _restart) {
	if(_restart) value = 0.0;
	action = pbfaFadeIn;
	recalculate();
}

void phdFader::fadeOut(bool _restart) {
	if(_restart) value = 1.0;
	action = pbfaFadeOut;
	recalculate();
}

void phdFader::pause() {
	action = pbfaNone;
}

void phdFader::update() {
	
	if(action == pbfaNone) return;

	currentTime = ofGetElapsedTimef();

	if(currentTime >= futureEndTime) {
		if(action == pbfaFadeIn) {
			value = 1.0;
		} else if(action == pbfaFadeOut) {
			value = 0.0;
		}
		action = pbfaNone;
		return;
	}

	if(action == pbfaFadeIn) {
		setValue(1.0 - ((futureEndTime - currentTime) / duration));
	} else if(action == pbfaFadeOut) {
		setValue((futureEndTime - currentTime) / duration);
	}
}

phdFaderHitTest phdFader::updateHitTest(float _x, float _y) {

	hitFader = phfNone;

	float _posX = _x - x;
	float _posY = _y - y;

	if(_posX >= 0 && _posX <= w/2.0 && _posY >= (1.0-posDuration)*(h-btSz*2)+btSz-2 && _posY <= (1.0-posDuration)*(h-btSz*2)+btSz+2) {
		hitFader = phfDuration;
	} else if(_posX >= 0 && _posX <= w && _posY >= 0.0 && _posY <= btSz) {
		hitFader = phfFadeIn;
	} else if(_posX >= 0 && _posX <= w && _posY >= h-btSz && _posY <= h) {
		hitFader = phfFadeOut;
	} else if(_posX >= 0 && _posX <= w && _posY >= btSz && _posY <= h-btSz) {
		hitFader = phfSlider;
	}

	return hitFader;
}

void phdFader::setPosDurationByCoord(float _x, float _y) {
	posDuration = ofMap(_y - y-btSz, h - btSz*2, 0.0, 0.0, 1.0, true);
	duration = posDuration * PHDMAXFADERTIME;
}

void phdFader::setValueByCoord(float _x, float _y) {
	value = ofMap(_y - y-btSz, h-btSz*2, 0.0, 0.0, 1.0, true);
}

void phdFader::draw(ofColor _fill, ofColor _border, ofColor _text) {

	drawFilledBorderRectangle(0,0,w,btSz, _fill, _border);
	drawFilledBorderRectangle(0,btSz,w,h-btSz*2, _fill, _border);
	drawFilledBorderRectangle(0,h-btSz,w,btSz, _fill, _border);

	float _pos = (1.0-value) * (h-btSz*2);
	drawFilledBorderRectangle(0.0, btSz+_pos, w, h-btSz*2-_pos, _border, _border);

	drawFilledBorderRectangle(0.0, (1.0-posDuration)*(h-btSz*2.0)+btSz-2, w/2.0, 4, _text, _text);

	ofSetColor(_text);

	float _x = 0.0;
	float _y = 0.0;

	drawBorderTriangle(_x+2, _y+2, w-4, btSz-4, ptdTop);
	drawBorderTriangle(_x+2, _y+h-btSz+2, w-4, btSz-4, ptdBottom);
	//ofDrawBitmapString(caption, (getWidth()-(caption.size()*8.0))/2.0, 12.0);
}
