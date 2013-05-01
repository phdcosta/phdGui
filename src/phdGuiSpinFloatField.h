#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiSpinFloatField : public phdGuiControl {
protected:
	float value;
	int fieldSize;
	int precision;

	float curVariation;
	float maxVariation;

	float mapEaseQuartic(float value, float sizeIn, float sizeOut = 1.0) {
		float r = value / sizeIn; r = (r*r*r*r);
		if(value < 0.0) r = -r;
		return MIN(sizeOut, MAX(-sizeOut, r * sizeOut));
	}

public:
	phdGuiSpinFloatField(string _caption = "inputField", phdTreeNodeData * _nodedata = NULL);
	~phdGuiSpinFloatField();

	inline virtual string getClassName() { return "PHDGUISPINFLOATFIELD"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiSpinFloatField(); }

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
	void setVariation(float _value) { maxVariation = _value; }

	void draw(float _x, float _y);

	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
