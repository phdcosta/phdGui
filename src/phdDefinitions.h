#pragma once

#define PHDDEFAULTWIDTH 512.0
#define PHDDEFAULTHEIGHT 384.0

#define PHDUSEBLENDSHADER 1

#include "ofMain.h"

//--------------------------------------------------------------------------------------------------------------
//enum phdLoopMode { LOOP_NORMAL, LOOP_PINGPONG, NO_LOOP_STOP, NO_LOOP_PAUSE };
enum phdDirectionMode { MOVING_FORWARD, MOVING_BACKWARD };

//--------------------------------------------------------------------------------------------------------------
class phdPt2D {
public:
	float x, y;

	phdPt2D(float _x = 0.0, float _y = 0.0) {
		x = _x;
		y = _y;
	}
};

//--------------------------------------------------------------------------------------------------------------
class phdBaseParams {
public:
	float time;
	float position;

	float mouse[2];
	float resolution[2];

	float valuesA[4];
	float valuesB[4];

	float fftBands[16];
	float * audio;

	void * clip;

	phdBaseParams() {
		clip = NULL;
		audio = NULL;
	}
};

//--------------------------------------------------------------------------------------------------------------
class phdPairNameValue {
public:
	string name;
	string value;

	phdPairNameValue(string _name, string _value) {
		name = _name;
		value = _value;
	}

	void setSplitted(string _value) {
		vector<string> _items = ofSplitString(_value, "|", true, true);
		if(_items.size() > 1) {
			name = _items[0];
			value = _items[1];
		}
	}

	string getSplitted() {
		return name + "|" + value + "|";
	}
};

//--------------------------------------------------------------------------------------------------------------
enum phdHitTestInfoType {
	phtOutside, phtInside, phtEdge, phtVertex,
	phtBoundsCenter, phtBoundResizeH, phtBoundResizeV,
	phtMenu, phtTitleBar,
	phtVerticalScroll, phtHorizontalScroll, phtResize, phtCommand,
	phtInputLink, phtOutputLink,
	phtHotspot, phtClip, phtFilter,
	phtPopupMenu, phtPopupItem,
	phtGuiItem,
	phtTimeline, phtTLStartMark, phtTLEndMark, phtTLCursor,
	phtNone
};

//--------------------------------------------------------------------------------------------------------------
class phdHitTestInfo {
public:
	phdHitTestInfoType infoType;
	string label;
	int itemIndex;
	void * itemPtr;

	phdHitTestInfo() { clear(); }

	void clear() {
		infoType	= phtNone;
		label		= "";
		itemIndex	= -1;
		itemPtr		= NULL;
	}
};
