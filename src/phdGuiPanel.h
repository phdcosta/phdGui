#pragma once

#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiPanel : public phdGuiControl {
protected:
	bool mouseToParent;

public:
	phdGuiPanel(string _caption = "panel", phdTreeNodeData * _nodedata = NULL);
	~phdGuiPanel();

	inline virtual string getClassName() { return "PHDGUIPANEL"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiPanel(); }

	inline bool isContainer() { return true; }

	inline void setMouseToParent(bool _value) { mouseToParent = _value; }
	inline bool sendMouseToParent() { return mouseToParent; }

	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};