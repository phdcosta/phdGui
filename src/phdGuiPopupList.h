#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiPopupList : public phdGuiControl {
protected:
	int curItem;

public:
	phdGuiPopupList(string _caption = "popupList", phdTreeNodeData * _nodedata = NULL);
	~phdGuiPopupList();

	inline virtual string getClassName() { return "PHDGUIPOPUPLIST"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiPopupList(); }

	// use these methods to control internal values
	inline virtual float getValue(int _index = 0) { return curItem; }
	inline virtual void setValue(float _value = 0.0, int _index = 0) {
		if(getPopupMenu() != NULL) {
			if(_value > -1 && _value < getPopupMenu()->size()) {
				curItem = (int)_value;
				phdGuiEventData _data(petOnChangeValue, this);
				ofNotifyEvent(guiEvent, _data, this);
			}
		}
	}
	inline virtual int getCount() { return 1; }

	// use these methods to control internal strings
	inline virtual string getString(int _index = 0) {
		if(getPopupMenu() != NULL) {
			if(curItem > -1 && curItem < getStringCount()) {
				return (*getPopupMenu())[curItem]->name;
			}
		}
		return "";
	}

	inline virtual void setString(int _index = 0, string _value = "") {
		if(getPopupMenu() != NULL) {
			int _index = getPopupMenu()->indexByName(_value);
			if(_index != -1) { 
				curItem = _index;
				phdGuiEventData _data(petOnChangeValue, this);
				ofNotifyEvent(guiEvent, _data, this);
			}
		}
	}
	inline virtual int getStringCount() {
		if(getPopupMenu() != NULL) return getPopupMenu()->size();
		return 0;
	}

	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);

	virtual void onPopupClick(phdPopupMenuEventData &_eventData);
};

