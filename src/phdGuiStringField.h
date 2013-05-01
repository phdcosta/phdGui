#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiStringField : public phdGuiControl {
protected:
	int fieldSize;
	string value;

public:
	phdGuiStringField(string _caption = "inputField", phdTreeNodeData * _nodedata = NULL);
	~phdGuiStringField();

	inline virtual string getClassName() { return "PHDGUISTRINGFIELD"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiStringField(); }

	// use these methods to control internal strings
	inline virtual string getString(int _index = 0) { return value; }
	inline virtual void setString(string _value = "", int _index = 0) {
		if(value != _value) {
			value = _value;
			phdGuiEventData _data(petOnChangeValue, this);
			ofNotifyEvent(guiEvent, _data, this);
		}
	}
	inline virtual int getStringCount() { return 1; }

	void setFieldSize(int _size = 20) { fieldSize = _size; }

	void draw(float _x, float _y);

	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
