#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiPushButton : public phdGuiControl {
protected:
	bool pushed;

public:
	phdGuiPushButton(string _caption = "button", phdTreeNodeData * _nodedata = NULL);
	~phdGuiPushButton();

	inline virtual string getClassName() { return "PHDGUIPUSHBUTTON"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiPushButton(); }

	inline virtual void setPushed(bool _value) { pushed = _value; }
	inline bool isPushed() { return pushed; }

	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
