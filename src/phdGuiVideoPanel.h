#pragma once

#include "ofMain.h"
#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiVideoPanel : public phdGuiControl {
protected:
	ofVideoPlayer video;
	string fileName;
	float playing;

public:
	phdGuiVideoPanel(string _caption = "videoPanel", phdTreeNodeData * _nodedata = NULL);
	~phdGuiVideoPanel();

	inline virtual string getClassName() { return "PHDGUIVIDEOPANEL"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiVideoPanel(); }

	inline bool isContainer() { return true; }

	inline virtual void setString(string _value, int _index = 0) {
		if(_value != fileName) {
			fileName = _value;
			video.loadMovie(fileName);
		}
	}

	inline virtual string getString(int _index = 0) {
		return fileName;
	}

	inline virtual void setValue(float _value, int _index = 0) {
		if(_index == 0 && _value != 0) {
			video.setPaused(false); video.play();
		} else if(_index == 0 && _value == 0) {
			video.setPaused(true);
		} else if(_index == 1 && _value != video.getSpeed()) {
			video.setSpeed(_value);
		} else if(_index == 2 && _value != video.getPosition()) {
			video.setPosition(_value);
		} else if(_index == 3 && _value == 1) {
			video.setLoopState(OF_LOOP_NONE);
		} else if(_index == 3 && _value == 2) {
			video.setLoopState(OF_LOOP_NORMAL);
		} else if(_index == 3 && _value == 3) {
			video.setLoopState(OF_LOOP_PALINDROME);
		}
	}

	inline virtual float getValue(int _index = 0) {
		if(_index == 0) {
			if(video.isPaused()) return 0.0;
			if(!video.isPaused()) return 1.0;
		} else if(_index == 1) {
			return video.getSpeed();
		} else if(_index == 2) {
			return video.getPosition();
		} else if(_index == 3) {
			return (float)(int) video.getLoopState();
		}
		return 0.0;
	}

	virtual void update();
	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};