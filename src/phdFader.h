#pragma once

#include "ofMain.h"

#include "phdUtils.h"

#define PHDMAXFADERTIME 10.0
//--------------------------------------------------------------------------------------------------------------
enum phdFaderActionType { pbfaNone, pbfaFadeIn, pbfaFadeOut };

//--------------------------------------------------------------------------------------------------------------
enum phdFaderHitTest { phfNone, phfDuration, phfSlider, phfFadeIn, phfFadeOut };

//--------------------------------------------------------------------------------------------------------------
class phdFader {
protected:
	static const int btSz = 12;
	float posDuration;

	float x, y, w, h;

	float value;
	float duration;

	float currentTime;
	float futureEndTime;

	phdFaderActionType action;

	phdFaderHitTest hitFader;

	void recalculate();

public:
	phdFader(float _duration = 3.0);

	inline phdFaderActionType getAction() { return action; }

	inline float getDuration() { return duration; }
	inline void setDuration(float _duration) {
		duration = MAX(_duration, 0.010);
		posDuration = duration / PHDMAXFADERTIME;
		recalculate();
	}

	inline float getValue() { return value; }
	inline void setValue(float _value) {
		value = MAX(MIN(_value, 1.0), 0.0);
		recalculate();
	}

	void fadeIn(bool _restart = false);
	void fadeOut(bool _restart = false);
	void pause();

	void update();

	phdFaderHitTest updateHitTest(float _x, float _y);

	void setPosDurationByCoord(float _x, float _y);
	void setValueByCoord(float _x, float _y);

	void setCoords(float _x, float _y, float _w, float _h) { x = _x; y = _y; w = _w; h = _h; }
	void draw(ofColor _fill, ofColor _border, ofColor _text);
};