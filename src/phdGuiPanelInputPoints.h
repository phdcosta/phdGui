#pragma once

#include "phdGuiManager.h"
#include "phdGuiPanel.h"

//--------------------------------------------------------------------------------------------------------------
enum phdGuiInputPointsMode { gipMouse, gipRect, gipQuad };

//--------------------------------------------------------------------------------------------------------------
class phdGuiPanelInputPoints : public phdGuiPanel {
protected:
	float pointsCoord[4][2];
	int pointsCount;
	int selPoint;
	phdGuiInputPointsMode pointsMode;

	inline float toAreaX(float _x) { return _x * (getWidth() -2) + 1; }
	inline float toAreaY(float _y) { return _y * (getHeight()-2 )+ 1; }

	inline float toOneX(float _x) { return (_x-1) / (getWidth() -2); }
	inline float toOneY(float _y) { return (_y-1) / (getHeight()-2); }

	int findNearestPoint(float _x, float _y);

	int dragPoint;
	int overPoint;
	float scrX, scrY;

public:
	phdGuiPanelInputPoints(string _caption = "pts", phdTreeNodeData * _nodedata = NULL);
	~phdGuiPanelInputPoints();

	inline virtual string getClassName() { return "PHDGUIPANELINPUTPOINTS"; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiPanelInputPoints(); }

	inline bool isContainer() { return true; }

	inline void setPointsMode(phdGuiInputPointsMode _value = gipMouse);
	inline phdGuiInputPointsMode getPointsMode() { return pointsMode; }

	inline int getPointsCount() { return pointsCount; }
	inline int getSelPoint() { return selPoint; }

	inline void resetPoints();

	inline void setPoint(int _index, float _x, float _y);
	inline float getCoord(int _index = 0, int _coord = 0) { return pointsCoord[_index][_coord]; }

	void drawPoints(float _x, float _y);

	virtual void draw(float _x, float _y);
	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);
};