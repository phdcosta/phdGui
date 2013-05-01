#pragma once

#include "phdGui.h"
#include "phdGuiPanel.h"

//--------------------------------------------------------------------------------------------------------------
enum phdTimelineValueType { ptlvStartPos, ptlvEndPos, ptlvCurPos, ptlvVelocity, ptlvDuration, ptlvLoopMode, ptlvDirMode };

//--------------------------------------------------------------------------------------------------------------
class phdGuiHorTimeline : public phdGuiPanel {
protected:
	phdBaseTimeline * timeline;

//	static const int btSz = 15;
	phdTimelineHitItemType hitTimeline;
	phdTimelineHitItemType updateHitTest(float _x, float _y);

public:
	phdGuiHorTimeline(string _caption = "horTimeline", phdTreeNodeData * _nodedata = NULL);
	~phdGuiHorTimeline();

	inline virtual string getClassName() { return "PHDGUIHORTIMELINE"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiVerFader(); }

	inline void setTimeline(phdBaseTimeline * _value = NULL) {
		timeline = _value;
//		if(timeline != NULL) value = timeline->getCurPosNormalized();
	}

	inline virtual float getValue(int _index = 0);
	inline virtual void setValue(float _value = 0.0, int _index = 0);
	inline virtual int getCount() { return 2; } // slider value and duration

	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};
