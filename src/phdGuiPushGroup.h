#pragma once

#include "phdGuiManager.h"
#include "phdGuiPanel.h"
#include "phdGuiPushButton.h"

//--------------------------------------------------------------------------------------------------------------
enum phdGuiPushGroupAlignmentType { pbgaHorizontal, pbgaVertical };

//--------------------------------------------------------------------------------------------------------------
class phdGuiPushGroup : public phdGuiPanel {
protected:
	phdGuiPushGroupAlignmentType alignment;

public:
	phdGuiPushGroup(string _caption = "pushGroup", phdTreeNodeData * _nodedata = NULL);
	~phdGuiPushGroup();

	inline virtual string getClassName() { return "PHDGUIPUSHGROUP"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiPushGroup(); }

	// override this to setID for all buttons
	inline virtual void setID(int _id);
	inline virtual void setGroup(int _group);

	// these methods controls which button is pushed
	inline virtual float getValue(int _index = 0);
	inline virtual void setValue(float _value = 0.0, int _index = 0);
	inline virtual int getCount() { return this->getChildCount(); }

	inline virtual phdGuiPushButton * addButton(phdGuiPushButton * _button = NULL);
	inline virtual void setAlignment(phdGuiPushGroupAlignmentType _alignment = pbgaHorizontal, bool _force = false);

	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);

	virtual void onGuiEvent(phdGuiEventData &_eventData);
};
