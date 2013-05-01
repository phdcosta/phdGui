#pragma once

#include "phdGuiManager.h"
#include "phdFader.h"
#include "phdGuiVerSlider.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiVerFader : public phdGuiVerSlider {
protected:
	phdFader * fader;
	phdFaderHitTest hitFader;
	phdFaderHitTest updateHitTest(float _x, float _y);

public:
	phdGuiVerFader(string _caption = "verFader", phdTreeNodeData * _nodedata = NULL);
	~phdGuiVerFader();

	inline virtual string getClassName() { return "PHDGUIVERFADER"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiVerFader(); }

	inline void setFader(phdFader * _value = NULL) { fader = _value; if(fader != NULL) value = fader->getValue(); }

	inline virtual float getValue(int _index = 0);
	inline virtual void setValue(float _value = 0.0, int _index = 0);
	inline virtual int getCount() { return 2; } // slider value and duration

	virtual void update();
	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
