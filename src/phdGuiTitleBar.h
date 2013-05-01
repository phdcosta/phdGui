#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiTitleBar : public phdGuiControl {
protected:

public:
	phdGuiTitleBar(string _caption = "titleBar", phdTreeNodeData * _nodedata = NULL);
	~phdGuiTitleBar();

	inline virtual string getClassName() { return "PHDGUITITLEBAR"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiTitleBar(); }

	inline bool isContainer() { return true; }

	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
