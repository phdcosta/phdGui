#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGui2dSlider : public phdGuiControl {
protected:
	float values[2];

public:
	phdGui2dSlider(string _caption = "2dSlider", phdTreeNodeData * _nodedata = NULL);
	~phdGui2dSlider();

	inline virtual string getClassName() { return "PHDGUI2DSLIDER"; }

	inline virtual phdGuiControl * createInstance() { return new phdGui2dSlider(); }

	// use these methods to control internal values
	inline virtual float getValue(int _index = 0) { return values[_index % 2]; }
	inline virtual void setValue(float _value = 0.0, int _index = 0) {
		if(values[_index % 2] != _value) {
			values[_index % 2] = _value;
			phdGuiEventData _data(petOnChangeValue, this);
			ofNotifyEvent(guiEvent, _data, this);
		}
	}
	inline virtual int getCount() { return 2; }

	void draw(float _x, float _y);

	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
