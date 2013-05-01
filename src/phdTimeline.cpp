#include "phdBaseTimeline.h"

#include "phdUtils.h"

//--------------------------------------------------------------------------------------------------
phdBaseTimeline::phdBaseTimeline() {
	loopMode = OF_LOOP_NORMAL;
	dirMode = MOVING_FORWARD;
	startPos = endPos = curPos = velocity = duration = lastPos = 0.0;
	paused = true;
}

void phdBaseTimeline::setDuration(double _value) {
	if(_value == 0.0) {
		startPos = endPos = curPos = velocity = duration = lastPos = 0.0;
	} else {
		duration = _value;
		setStartPos(startPos);
		if(endPos == 0.0) setEndPos(duration); else setEndPos(endPos);
		setCurPos(curPos);
	}
}

void phdBaseTimeline::setStartPos(double _value) {

	startPos = _value;

	if(startPos < 0.0) startPos = 0.0;
	if(startPos > endPos) startPos = endPos;
	if(startPos > duration) startPos = duration;
}

void phdBaseTimeline::setEndPos(double _value) {

	endPos = _value;

	if(endPos > duration) endPos = duration;
	if(endPos < startPos) endPos = startPos;
	if(endPos < 0.0) endPos = 0.0;
}

void phdBaseTimeline::setCurPos(double _value) {

	lastPos = curPos;

	curPos = _value;

	if(duration == 0.0) { curPos = 0.0; return; }

	if(startPos == endPos) { curPos = startPos; return; }
/*
	if(curPos >= startPos && curPos <= endPos) { // inside limits
		if(loopMode == OF_LOOP_PALINDROME && (startPos == 0.0 || endPos == duration)) {
			if(dirMode == MOVING_BACKWARD && curPos > lastPos) {
				curPos = startPos + (endPos - curPos);
				dirMode = MOVING_FORWARD;
				//lastPos = curPos;
				return;
			} else if(dirMode == MOVING_FORWARD && curPos < lastPos) {
				curPos = endPos - (curPos - startPos);
				dirMode = MOVING_BACKWARD;
				//lastPos = curPos;
				return;
			}
		}
	}
*/
	while(curPos < startPos || curPos > endPos) {
		if(loopMode == OF_LOOP_NORMAL) {
			if(curPos < startPos) {
				curPos = endPos - (startPos - curPos);
			} else if(curPos > endPos) {
				curPos = startPos + (curPos - endPos);
			}
		} else if(loopMode == OF_LOOP_PALINDROME) {
			if(curPos <= startPos) {
				curPos = startPos + (startPos - curPos);
				dirMode = MOVING_FORWARD;
			} else if(curPos >= endPos) {
				curPos = endPos - (curPos - endPos);
				dirMode = MOVING_BACKWARD;
			}
		} else if(loopMode == OF_LOOP_NONE) {
			if(curPos <= startPos) {
				pause();
				curPos = startPos;
			} else if(curPos >= endPos) {
				pause();
				curPos = endPos;
			}
		} else if(loopMode == OF_LOOP_NONE) {
			if(curPos <= startPos || curPos >= endPos) {
				pause();
				curPos = startPos;
			}
		}
	}
}

void phdBaseTimeline::setVelocity(double _value) {
	velocity = fabs(_value);
}

void phdBaseTimeline::moveBy(double _step) {
	if(paused) return;
	setCurPos(curPos + (_step * velocity * (dirMode == MOVING_FORWARD ? 1.0 : -1.0)));
}

void phdBaseTimeline::draw(ofColor _fill, ofColor _border, ofColor _text) {

	if(duration == 0.0) {
		drawFilledBorderRectangle(x, y, w, h, _fill, _border);
		return;
	}

	float _h2 = h / 2.0;

	drawFilledBorderRectangle(x, y, w-btW+5.0, h, _fill, _border);
	drawFilledBorderRectangle(x+w-btW+5.0, y, btW-5.0, h, _fill, _border);

	ofSetColor(_text);

	if(isPaused()) { ofDrawBitmapString(">", x+w-10, y+_h2+4.0); } else { ofDrawBitmapString("#", x+w-10, y+_h2+4.0); }

	float _pos = 0.0;

	ofSetColor(_border);

	if(duration != 0.0) _pos = ofMap(startPos, 0.0, duration, 5.0, w-btW);
	ofLine(x + _pos - 5.0, y, x + _pos - 5.0, y+h);
	ofLine(x + _pos - 5.0, y, x + _pos, y+_h2);
	ofLine(x + _pos - 5.0, y+h, x + _pos, y+_h2);

	if(duration != 0.0) _pos = ofMap(endPos, 0.0, duration, 5.0, w-btW);
	ofLine(x + _pos + 5.0, y, x + _pos + 5.0, y + h);
	ofLine(x + _pos + 5.0, y, x + _pos, y+_h2);
	ofLine(x + _pos + 5.0, y + h, x + _pos, y+_h2);

	if(duration != 0.0) _pos = ofMap(curPos, 0.0, duration, 5.0, w-btW);
	ofLine(x + _pos, y, x + _pos, y + h);
}

phdTimelineHitItemType phdBaseTimeline::updateHitTestInfo(float _x, float _y) {

	float _pos = 0.0;

	if(duration != 0.0) _pos = ofMap(startPos, 0.0, duration, 5.0, w-btW);
	if(_x >= x + _pos - 5.0 && _x <= x + _pos && _y >= y - h && _y <= y + h) return tliStartMark;

	if(duration != 0.0) _pos = ofMap(endPos, 0.0, duration, 5.0, w-btW);
	if(_x >= x + _pos && _x <= x + _pos + 5.0 && _y >= y - h && _y <= y + h) return tliEndMark;

	if(duration != 0.0) _pos = ofMap(curPos, 0.0, duration, 5.0, w-btW);
	if(_x >= x + _pos - 3.0 && _x <= x + _pos + 3.0 && _y >= y - h && _y <= y + h) return tliCursor;

	if(_x >= x + w-btW+5.0 && _x <= x + w && _y >= y - h && _y <= y + h) return tliButton;

	if(_x >= x && _x <= x + w && _y >= y && _y <= y + h) return tliInside;

	return tliOutside;
}
