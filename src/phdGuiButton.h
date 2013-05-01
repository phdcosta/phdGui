#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiButton : public phdGuiControl {
protected:
	bool isMouseDown;

public:
	phdGuiButton(string _caption = "button", phdTreeNodeData * _nodedata = NULL);
	~phdGuiButton();

	inline virtual string getClassName() { return "PHDGUIBUTTON"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiButton(); }

	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
