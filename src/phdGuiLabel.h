#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiLabel : public phdGuiControl {
protected:

public:
	phdGuiLabel(string _caption = "button", phdTreeNodeData * _nodedata = NULL);
	~phdGuiLabel();

	inline virtual string getClassName() { return "PHDGUILABEL"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiLabel(); }

	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
