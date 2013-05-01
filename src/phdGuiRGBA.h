#pragma once

#include "phdGuiManager.h"
#include "phdGuiPanel.h"
#include "phdGuiVerSlider.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiRGBA : public phdGuiPanel {
protected:
	phdGuiVerSlider * sliders[4];
	ofColor * color;

public:
	phdGuiRGBA(string _caption = "clip", phdTreeNodeData * _nodedata = NULL);
	~phdGuiRGBA();

	inline virtual string getClassName() { return "PHDGUIRGBA"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiRGBA(); }

	void setColor(ofColor * _color = NULL);

	// override this to setID for all sliders
	inline virtual void setID(int _id);
	inline virtual void setWidth(float _value);

	// use these methods to control internal values
	inline virtual float getValue(int _index = 0);
	inline virtual void setValue(float _value = 0.0, int _index = 0);
	inline virtual int getCount() { return 4; }

	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);

	virtual void onGuiEvent(phdGuiEventData &_eventData);
};
