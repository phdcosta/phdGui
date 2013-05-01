#pragma once

#include "ofMain.h"

#include "phdGui.h"
#include "phdBaseClip.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiClip : public phdGuiPanel {
protected:
	phdGuiPanel * clipPanel;
	phdGuiPanelInputPoints * clipArea;
	phdGuiHorTimeline * timeline;
	phdGuiPanel * toolArea;
	phdGuiButton * btnShowBar;
	phdGuiRGBA * colorSliders;
	phdGuiSpinFloatField * speedField;
	phdGuiPushGroup * loopModeGroup;
	phdGuiPushGroup * dirModeGroup;
	phdGuiPushGroup * mouseInputGroup;
	phdGuiPushGroup * cropModeGroup;
	phdGuiVerFader * transictionSlider;

	phdGuiFloatField * clipW;
	phdGuiFloatField * clipH;

	phdGuiPopupList * faderMode;

	phdBaseClip * clip;
	string labelInfo;

	static const int toolBarHeight = 120;
	static const int defaultW = 200;
	static const int defaultH = 160;

	void resetPointers();

public:
	phdGuiClip(string _caption = "clip", phdTreeNodeData * _nodedata = NULL);
	~phdGuiClip();

	inline virtual string getClassName() { return "PHDGUICLIP"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiClip(); }

	void setClip(phdBaseClip * _value = NULL);
	phdBaseClip * getClip() { return clip; }

	void setTransictionsMenu(phdPopupMenu * _popup) {
		if(faderMode != NULL) { faderMode->setPopupMenu(_popup); }
	}

	//void setGenerator(phdBaseGenerator * _value);

	virtual void update();
	virtual void draw(float _x, float _y);

	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);

	virtual void onGuiEvent(phdGuiEventData &_eventData);
};

//--------------------------------------------------------------------------------------------------------------
class phdGuiSmallClip : public phdGuiPanel {
protected:
	static const int defaultW = 90;
	static const int defaultH = 60;

	phdGuiPanel * clipArea;
	phdGuiHorTimeline * timeline;
	phdGuiVerFader * transictionSlider;

	phdBaseClip * clip;
	string labelInfo;

	void resetPointers();

public:
	phdGuiSmallClip(string _caption = "smallClip", phdTreeNodeData * _nodedata = NULL);
	~phdGuiSmallClip();

	inline virtual string getClassName() { return "PHDGUISMALLCLIP"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiSmallClip(); }

	void setClip(phdBaseClip * _value = NULL);
	phdBaseClip * getClip() { return clip; }

//	void setGenerator(phdBaseGenerator * _value);

	virtual void update();
	virtual void draw(float _x, float _y);

	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);

	virtual void onGuiEvent(phdGuiEventData &_eventData);
};
