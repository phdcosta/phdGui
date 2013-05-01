#include "phdGuiPanelInputPoints.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiPanelInputPoints::phdGuiPanelInputPoints(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	mouseToParent = false;
	setPointsMode(gipQuad);	resetPoints();
	setPointsMode(gipRect);	resetPoints();
}

phdGuiPanelInputPoints::~phdGuiPanelInputPoints() {
	phdGuiControl::~phdGuiControl();
}

int phdGuiPanelInputPoints::findNearestPoint(float _x, float _y) {
	int nearIndex = -1;
	float minDist = 1000000.0;
	for(int i = 0; i < pointsCount; i++) {
		float _ptDist = ofDist(toAreaX(pointsCoord[i][0]), toAreaY(pointsCoord[i][1]), _x, _y);
		if(_ptDist < 4.0 && _ptDist < minDist) {
			nearIndex = i;
			minDist = _ptDist;
		}
	}
	return nearIndex;
}

void phdGuiPanelInputPoints::setPointsMode(phdGuiInputPointsMode _value) {
	pointsMode = _value;
	if(pointsMode == gipMouse) pointsCount = 1;
	if(pointsMode == gipRect)  pointsCount = 2;
	if(pointsMode == gipQuad)  pointsCount = 4;
}

void phdGuiPanelInputPoints::resetPoints() {
	if(pointsMode == gipMouse) { pointsCoord[0][0] = 0.5; pointsCoord[0][1] = 0.5; }
	if(pointsMode == gipRect) {
		pointsCoord[0][0] = 0.0; pointsCoord[0][1] = 0.0;
		pointsCoord[1][0] = 1.0; pointsCoord[1][1] = 1.0;
	}
	if(pointsMode == gipQuad) {
		pointsCoord[0][0] = 0.0; pointsCoord[0][1] = 0.0;
		pointsCoord[1][0] = 1.0; pointsCoord[1][1] = 0.0;
		pointsCoord[2][0] = 1.0; pointsCoord[2][1] = 1.0;
		pointsCoord[3][0] = 0.0; pointsCoord[3][1] = 1.0;
	}
}

void phdGuiPanelInputPoints::setPoint(int _index, float _x, float _y) {
	if(_index > -1 && _index < pointsCount) {
		pointsCoord[_index][0] = MIN(MAX(_x, 0.0), 1.0);
		pointsCoord[_index][1] = MIN(MAX(_y, 0.0), 1.0);
	}
}

void phdGuiPanelInputPoints::drawPoints(float _x, float _y) {

	if(pointsMode == gipMouse) {

		float _ax = _x + toAreaX(pointsCoord[0][0]);
		float _ay = _y + toAreaY(pointsCoord[0][1]);

		ofLine(_ax-3, _ay, _ax+3, _ay);
		ofLine(_ax, _ay-3, _ax, _ay+3);

		ofSetColor(255,255,255,255); if(overPoint == 0) ofSetColor(255,0,0,255); ofCircle(_ax, _ay, 3);

	} else if(pointsMode == gipRect) {

		float _ax = _x + toAreaX(pointsCoord[0][0]);
		float _ay = _y + toAreaY(pointsCoord[0][1]);
		float _bx = _x + toAreaX(pointsCoord[1][0]);
		float _by = _y + toAreaY(pointsCoord[1][1]);

		ofLine(_ax, _ay, _bx, _ay);
		ofLine(_bx, _ay, _bx, _by);
		ofLine(_bx, _by, _ax, _by);
		ofLine(_ax, _by, _ax, _ay);

		ofSetColor(255,255,255,255); if(overPoint == 0) ofSetColor(255,0,0,255); ofCircle(_ax, _ay, 3);
		ofSetColor(255,255,255,255); if(overPoint == 1) ofSetColor(255,0,0,255); ofCircle(_bx, _by, 3);

	} else if(pointsMode == gipQuad) {

		float _ax = _x + toAreaX(pointsCoord[0][0]);
		float _ay = _y + toAreaY(pointsCoord[0][1]);
		float _bx = _x + toAreaX(pointsCoord[1][0]);
		float _by = _y + toAreaY(pointsCoord[1][1]);
		float _cx = _x + toAreaX(pointsCoord[2][0]);
		float _cy = _y + toAreaY(pointsCoord[2][1]);
		float _dx = _x + toAreaX(pointsCoord[3][0]);
		float _dy = _y + toAreaY(pointsCoord[3][1]);

		ofLine(_ax, _ay, _bx, _by);
		ofLine(_bx, _by, _cx, _cy);
		ofLine(_cx, _cy, _dx, _dy);
		ofLine(_dx, _dy, _ax, _ay);

		ofSetColor(255,255,255,255); if(overPoint == 0) ofSetColor(255,0,0,255); ofCircle(_ax, _ay, 3);
		ofSetColor(255,255,255,255); if(overPoint == 1) ofSetColor(255,0,0,255); ofCircle(_bx, _by, 3);
		ofSetColor(255,255,255,255); if(overPoint == 2) ofSetColor(255,0,0,255); ofCircle(_cx, _cy, 3);
		ofSetColor(255,255,255,255); if(overPoint == 3) ofSetColor(255,0,0,255); ofCircle(_dx, _dy, 3);
	}
}

void phdGuiPanelInputPoints::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	drawChildren(_x, _y); // draw contents first -> then draw point over it

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	drawFilledBorderRectangle(0, 0, getWidth(), getHeight(), getManager()->getFillColor(this), getManager()->getBorderColor(this));

	ofNoFill();
	ofSetColor(getManager()->getTextColor(this));

	drawPoints(0,0);

	ofPopMatrix();
	ofPopStyle();
}

void phdGuiPanelInputPoints::onMouseMoved(ofMouseEventArgs & args) {
	overPoint = findNearestPoint(args.x - getScreenX(), args.y - getScreenY());
}

void phdGuiPanelInputPoints::onMousePressed(ofMouseEventArgs & args) {
	scrX = getScreenX(); scrY = getScreenY();
	dragPoint = findNearestPoint(args.x - scrX, args.y - scrY);
}

void phdGuiPanelInputPoints::onMouseDragged(ofMouseEventArgs & args) {
	if(dragPoint != -1) {
		setPoint(dragPoint, toOneX(args.x-scrX), toOneY(args.y-scrY));
		phdGuiEventData _data(petOnChangeValue, this);
		ofNotifyEvent(guiEvent, _data, this);
	}
}

void phdGuiPanelInputPoints::onMouseReleased(ofMouseEventArgs & args) {
	dragPoint = -1;
}

void phdGuiPanelInputPoints::onKeyPressed(ofKeyEventArgs & args) {
}
