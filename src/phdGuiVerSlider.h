#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiVerSlider : public phdGuiControl {
protected:
	float value;

public:
	phdGuiVerSlider(string _caption = "verSlider", phdTreeNodeData * _nodedata = NULL);
	~phdGuiVerSlider();

	inline virtual string getClassName() { return "PHDGUIVERSLIDER"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiVerSlider(); }

	// use these methods to control internal values
	inline virtual float getValue(int _index = 0) { return value; }
	inline virtual void setValue(float _value = 0.0, int _index = 0) {
		if(value != _value) {
			value = _value;
			phdGuiEventData _data(petOnChangeValue, this);
			ofNotifyEvent(guiEvent, _data, this);
		}
	}
	inline virtual int getCount() { return 1; }

	void draw(float _x, float _y);

	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
