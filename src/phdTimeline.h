#pragma once

#include "ofMain.h"

#include "phdDefinitions.h"
#include "phdColors.h"

//--------------------------------------------------------------------------------------------------------------
enum phdTimelineHitItemType { tliOutside, tliInside, tliStartMark, tliCursor, tliEndMark, tliButton };

class phdBaseTimelineData {
public:
	double startPos;
	double endPos;
	double curPos;
	double velocity;
	double duration;
	ofLoopType loopMode;
	phdDirectionMode dirMode;
	bool paused;
	float x, y, w, h;
	static const int btW = 15;
};

//--------------------------------------------------------------------------------------------------------------
class phdBaseTimeline {
protected:
	double startPos;
	double endPos;
	double curPos;
	double velocity;
	double duration;
	ofLoopType loopMode;
	phdDirectionMode dirMode;

	double lastPos;

	bool paused;

	float x, y, w, h;

	static const int btW = 20;
public:
	phdTimelineHitItemType hitInfo;

	phdBaseTimeline();

	virtual string getClassName() { return "PHDBASETIMELINE"; }

	void setLoopMode(ofLoopType _value) { loopMode =_value; }
	ofLoopType getLoopMode() { return loopMode; }

	void setDirMode(phdDirectionMode _value) { dirMode = _value; }
	phdDirectionMode getDirMode() { return dirMode; }

	double getDuration() { return duration; }
	void setDuration(double _value);

	double getStartPos() { return startPos; }
	void setStartPos(double _value);

	double getEndPos() { return endPos; }
	void setEndPos(double _value);

	double getCurPos() { return curPos; }
	void setCurPos(double _value);

	double getCurPosNormalized() {
		if(duration != 0.0) return ofMap(curPos, 0.0, duration, 0.0, 1.0, true);
		return 0.0;
	}

	void setCurPosNormalized(double _value) { curPos = ofMap(_value, 0.0, 1.0, 0.0, duration, true); }

	double getVelocity() { return velocity;  }
	void setVelocity(double _value);

	void moveBy(double _step);

	void setPaused(bool _value) { paused = _value; }
	bool isPaused() { return paused; }

	void play() { paused = false; }
	void pause() { paused = true; }
	void stop() { setCurPos(startPos); paused = true; }

	void setCoords(float _x, float _y, float _w, float _h) { x = _x; y = _y; w = _w; h = _h; }

	double getTimeByX(double _pos) {
		if(x+w-btW != 0.0) return ofMap(_pos, x, x+w-btW, 0.0, duration);
		return 0.0;
	}

	double getTimeByXNormalized(double _pos) {
		if(x+w-btW != 0.0) return ofMap(_pos, x, x+w-btW, 0.0, 1.0);
		return 0.0;
	}

	void draw(ofColor _fill, ofColor _border, ofColor _text);

	phdTimelineHitItemType updateHitTestInfo(float _x, float _y);
};
