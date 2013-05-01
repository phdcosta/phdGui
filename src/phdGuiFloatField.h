#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiFloatField : public phdGuiControl {
protected:
	float value;
	int fieldSize;
	int precision;

public:
	phdGuiFloatField(string _caption = "inputField", phdTreeNodeData * _nodedata = NULL);
	~phdGuiFloatField();

	inline virtual string getClassName() { return "PHDGUIFLOATFIELD"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiFloatField(); }

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

	void setFieldSize(int _size = 4, int _precision = 2) { fieldSize = _size; precision = _precision; }

	void draw(float _x, float _y);

	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
