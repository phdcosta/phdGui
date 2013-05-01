#include "phdGuiClip.h"

#define IDSHOWGUI			1100
#define IDTRANSICTION		1150
#define IDFADERMODE			1160
#define IDCOLORBASE			1200
#define IDSPEED				1300
#define IDLOOPMODE			1400
#define IDDIRMODE			1450
#define IDMOUSEINPUTMODE	1500
#define IDCROPMODE			1520
#define IDPOINTS			1550
#define IDCLIPW				1610
#define IDCLIPH				1620
#define IDTIMELINE			1700

//--------------------------------------------------------------------------------------------------------------
phdGuiClip::phdGuiClip(string _caption, phdTreeNodeData * _nodedata) {

	caption = _caption;
	setRectArea(200, 800, defaultW, defaultH + toolBarHeight + 12 + 15);

	resetPointers();

	setClip(NULL);

	// title bar -------------------------------------------
	phdGuiTitleBar * _title = (phdGuiTitleBar*) this->addFirstChild(new phdGuiTitleBar(caption));
	_title->setDimension(20, 15);

	btnShowBar = (phdGuiButton*) _title->addFirstChild(new phdGuiButton("G"));
	btnShowBar->setDimension(15,15);
	btnShowBar->setAlign(gamRight);
	btnShowBar->setID(IDSHOWGUI);

	ofAddListener(btnShowBar->guiEvent, this, &phdGuiClip::onGuiEvent);

	// tool bar -------------------------------------------
	this->toolArea = (phdGuiPanel*) this->addLastChild(new phdGuiPanel("toolbar"));
	this->toolArea->setDimension(20, toolBarHeight);
	this->toolArea->setAlign(gamBottom);

	// transiction fader-------------------------------------------
	transictionSlider = (phdGuiVerFader*) toolArea->addLastChild(new phdGuiVerFader("T"));
	transictionSlider->setDimension(20, 20);
	transictionSlider->setAlign(gamLeft);
	transictionSlider->setID(IDTRANSICTION);
	if(getClip() != NULL) transictionSlider->setFader(&getClip()->fader);

	// fader -------------------------------------------
	faderMode = (phdGuiPopupList*) toolArea->addLastChild(new phdGuiPopupList("faderMode"));
	faderMode->setID(IDFADERMODE);
	faderMode->setDimension(20, 17);
	faderMode->setAlign(gamBottom);
	ofAddListener(faderMode->guiEvent, this, &phdGuiClip::onGuiEvent);

	// clip dimensions W e H -------------------------------------------
	phdGuiPanel * _panel = (phdGuiPanel*) toolArea->addLastChild(new phdGuiPanel("clipSize"));
	_panel->setDimension(70, 20);
	_panel->setAlign(gamBottom);
	_panel->setBorder(false);

	phdGuiLabel * _label = (phdGuiLabel*) _panel->addLastChild(new phdGuiLabel("clip W:"));
	_label->setDimension(56,15);
	_label->setAlign(gamLeft);

	clipW = (phdGuiFloatField*) _panel->addLastChild(new phdGuiFloatField("cw"));
	clipW->setID(IDCLIPW);
	clipW->setDimension(8*5+4,15);
	clipW->setAlign(gamLeft);
	clipW->setFieldSize(4,0);
	if(getClip() != NULL) clipW->setValue(getClip()->getWidth(), 0);

	ofAddListener(clipW->guiEvent, this, &phdGuiClip::onGuiEvent);

	_label = (phdGuiLabel*) _panel->addLastChild(new phdGuiLabel("H:"));
	_label->setDimension(16,15);
	_label->setAlign(gamLeft);

	clipH = (phdGuiFloatField*) _panel->addLastChild(new phdGuiFloatField("ch"));
	clipH->setID(IDCLIPH);
	clipH->setDimension(8*5+4,15);
	clipH->setAlign(gamLeft);
	clipH->setFieldSize(4,0);
	if(getClip() != NULL) clipH->setValue(getClip()->getHeight(), 0);

	ofAddListener(clipH->guiEvent, this, &phdGuiClip::onGuiEvent);

	// mouse input Mode -------------------------------------------
	_panel = (phdGuiPanel*) toolArea->addLastChild(new phdGuiPanel("cropMode"));
	_panel->setDimension(70, 20);
	_panel->setAlign(gamBottom);
	_panel->setBorder(false);

	mouseInputGroup = (phdGuiPushGroup*) _panel->addLastChild(new phdGuiPushGroup("mouseMode"));
	mouseInputGroup->setID(IDMOUSEINPUTMODE);
	mouseInputGroup->setGroup(IDMOUSEINPUTMODE);
	mouseInputGroup->setDimension(60,20);
	mouseInputGroup->setAlign(gamLeft);
	mouseInputGroup->setAlignment(pbgaHorizontal, false);

	mouseInputGroup->addButton(new phdGuiPushButton("M"));
	mouseInputGroup->addButton(new phdGuiPushButton("R"));
	mouseInputGroup->addButton(new phdGuiPushButton("Q"));

	mouseInputGroup->setValue(0.0, 0);

	ofAddListener(mouseInputGroup->guiEvent, this, &phdGuiClip::onGuiEvent);

	// Crop mode -------------------------------------------
	cropModeGroup = (phdGuiPushGroup*) _panel->addLastChild(new phdGuiPushGroup("cropMode"));
	cropModeGroup->setID(IDCROPMODE);
	cropModeGroup->setGroup(IDCROPMODE);
	cropModeGroup->setDimension(80,18);
	cropModeGroup->setAlign(gamBottom);
	cropModeGroup->setAlignment(pbgaHorizontal, false);

	cropModeGroup->addButton(new phdGuiPushButton("No"));
	cropModeGroup->addButton(new phdGuiPushButton("Rct"));
	cropModeGroup->addButton(new phdGuiPushButton("Quad"));

	cropModeGroup->setValue(0.0, 0);

	ofAddListener(cropModeGroup->guiEvent, this, &phdGuiClip::onGuiEvent);

	// RGBA sliders -------------------------------------------
	colorSliders = (phdGuiRGBA*) toolArea->addLastChild(new phdGuiRGBA("rgba"));
	colorSliders->setID(IDCOLORBASE);
	colorSliders->setDimension(56, 20);
	colorSliders->setAlign(gamLeft);
	if(getClip() != NULL) colorSliders->setColor(&getClip()->baseColor);

	// loop mode group -------------------------------------------
	_panel = (phdGuiPanel*) toolArea->addLastChild(new phdGuiPanel("loopCtrl"));
	_panel->setDimension(70, 20);
	_panel->setAlign(gamBottom);
	_panel->setBorder(false);

	dirModeGroup = (phdGuiPushGroup*) _panel->addLastChild(new phdGuiPushGroup("dirMode"));
	dirModeGroup->setID(IDDIRMODE);
	dirModeGroup->setGroup(IDDIRMODE);
	dirModeGroup->setDimension(40,18);
	dirModeGroup->setAlign(gamLeft);
	dirModeGroup->setAlignment(pbgaHorizontal, false);

	dirModeGroup->addButton(new phdGuiPushButton(">"));
	dirModeGroup->addButton(new phdGuiPushButton("<"));

	if(getClip() != NULL) dirModeGroup->setValue((int)getClip()->getLoopMode(), 0);

	ofAddListener(dirModeGroup->guiEvent, this, &phdGuiClip::onGuiEvent);

	loopModeGroup = (phdGuiPushGroup*) _panel->addLastChild(new phdGuiPushGroup("loopMode"));
	loopModeGroup->setID(IDLOOPMODE);
	loopModeGroup->setGroup(IDLOOPMODE);
	loopModeGroup->setDimension(60,18);
	loopModeGroup->setAlign(gamClient);
	loopModeGroup->setAlignment(pbgaHorizontal, false);

	loopModeGroup->addButton(new phdGuiPushButton("::"));
	loopModeGroup->addButton(new phdGuiPushButton("><"));
	loopModeGroup->addButton(new phdGuiPushButton("<>"));

	if(getClip() != NULL) loopModeGroup->setValue((int)getClip()->getLoopMode(), 0);

	ofAddListener(loopModeGroup->guiEvent, this, &phdGuiClip::onGuiEvent);

	// some labels -------------------------------------------
	_label = (phdGuiLabel*) toolArea->addLastChild(new phdGuiLabel(" dir  loopMode"));
	_label->setDimension(100,12);
	_label->setAlign(gamBottom);

	_label = (phdGuiLabel*) toolArea->addLastChild(new phdGuiLabel(" speed"));
	_label->setDimension(50,15);
	_label->setAlign(gamLeft);

	// speed field -------------------------------------------
	speedField = (phdGuiSpinFloatField*) toolArea->addLastChild(new phdGuiSpinFloatField("speed"));
	speedField->setID(IDSPEED);
	speedField->setFieldSize(2,2);
	speedField->setVariation(10);
	speedField->setDimension(12,17);
	speedField->setAlign(gamClient);
	speedField->setValue(1.0, 0); // speed limits = 0.0 -> 10.0

	ofAddListener(speedField->guiEvent, this, &phdGuiClip::onGuiEvent);

	// timeline area -------------------------------------------
	timeline = (phdGuiHorTimeline*) this->addLastChild(new phdGuiHorTimeline("timeline"));
	timeline->setID(IDTIMELINE);
	timeline->setDimension(20,12);
	timeline->setAlign(gamBottom);
	if(getClip() != NULL) timeline->setTimeline(&getClip()->timeline);
	ofAddListener(timeline->guiEvent, this, &phdGuiClip::onGuiEvent);

	// clip area -------------------------------------------
	clipPanel = (phdGuiPanel*) this->addLastChild(new phdGuiPanel("clipArea"));
	clipPanel->setDimension(20,20);
	clipPanel->setAlign(gamClient);

	this->clipArea = (phdGuiPanelInputPoints*) clipPanel->addLastChild(new phdGuiPanelInputPoints("clipPoints"));
	this->clipArea->setID(IDPOINTS);
	this->clipArea->setPosition(0,0);
	this->clipArea->setDimension(defaultW, defaultH);
	this->clipArea->setAlign(gamSize);
	this->clipArea->setMouseToParent(false);
	ofAddListener(clipArea->guiEvent, this, &phdGuiClip::onGuiEvent);
}

phdGuiClip::~phdGuiClip() {

	ofRemoveListener(btnShowBar->guiEvent, this, &phdGuiClip::onGuiEvent);
	ofRemoveListener(loopModeGroup->guiEvent, this, &phdGuiClip::onGuiEvent);
	ofRemoveListener(dirModeGroup->guiEvent, this, &phdGuiClip::onGuiEvent);
	ofRemoveListener(speedField->guiEvent, this, &phdGuiClip::onGuiEvent);

	ofRemoveListener(cropModeGroup->guiEvent, this, &phdGuiClip::onGuiEvent);
	ofRemoveListener(mouseInputGroup->guiEvent, this, &phdGuiClip::onGuiEvent);

	ofRemoveListener(clipH->guiEvent, this, &phdGuiClip::onGuiEvent);
	ofRemoveListener(clipW->guiEvent, this, &phdGuiClip::onGuiEvent);

	ofRemoveListener(timeline->guiEvent, this, &phdGuiClip::onGuiEvent);

	ofRemoveListener(clipArea->guiEvent, this, &phdGuiClip::onGuiEvent);

	caption = "";
	setClip(NULL);

	resetPointers();
}

void phdGuiClip::resetPointers() {

	clipPanel = NULL;
	clipArea = NULL;
	timeline = NULL;
	toolArea = NULL;
	btnShowBar = NULL;
	colorSliders = NULL;
	speedField = NULL;
	loopModeGroup = NULL;
	dirModeGroup = NULL;
	mouseInputGroup = NULL;
	cropModeGroup = NULL;
	transictionSlider = NULL;
	faderMode = NULL;
	clipW = NULL;
	clipH = NULL;
}

void phdGuiClip::setClip(phdBaseClip * _value) {

	clip = _value;

	if(clip != NULL) {
		transictionSlider->setFader(&getClip()->fader);
		clipW->setValue(getClip()->getWidth(), 0);
		clipH->setValue(getClip()->getHeight(), 0);
		dirModeGroup->setValue((int)getClip()->getLoopMode(), 0);
		loopModeGroup->setValue((int)getClip()->getLoopMode(), 0);
		colorSliders->setColor(&getClip()->baseColor);
		timeline->setTimeline(&getClip()->timeline);
	} else {
		if(transictionSlider != NULL) transictionSlider->setFader(NULL);
		if(colorSliders != NULL) colorSliders->setColor(NULL);
		if(timeline != NULL) timeline->setTimeline(NULL);
	}
}

/*
void phdGuiClip::setGenerator(phdBaseGenerator * _value) {

	clip.setGenerator(_value);

	if(_value != NULL) {
		this->setHeight(defaultH + toolBarHeight + 12 + 15);
		if(clipArea != NULL) {
			//setHeight(getBestHeightbyFixedW(_value->getWidth(), _value->getHeight(), 200)  + toolBarHeight + 12);
		}
		labelInfo = "W:" + ofToString(_value->getWidth(),  0);
		labelInfo+= "H:" + ofToString(_value->getHeight(), 0);
	} else {
		labelInfo = "W:0 H:0";
		this->setHeight(defaultH / 4 + toolBarHeight + 12 + 15);
	}
}
*/

void phdGuiClip::update() {

	// mark to update only once
	if(clip != NULL) {
		clip->markToUpdate();
		clip->update();
	}

	updateChildren();
}

void phdGuiClip::draw(float _x, float _y) {

	ofPushStyle();

	if(timeline != NULL && getClip() != NULL && getClip()->getDuration() != 0) { // draw only timelines with duration != 0
		timeline->setVisible(true);
	} else {
		timeline->setVisible(false);
		timeline->setHeight(12);
	}

	drawChildren(_x,_y); // draw controls inside this gui clip

	if(getClip() != NULL) {

		float s = getMinScale(clipPanel->getWidth(), clipPanel->getHeight(), getClip()->getWidth(), getClip()->getHeight());
		float c = getClip()->getWidth() * s;
		float d = getClip()->getHeight() * s;

		if(getClip()->getGenerator() != NULL) {
			s = getMinScale(clipPanel->getWidth(), clipPanel->getHeight(), getClip()->getGenerator()->getWidth(), getClip()->getGenerator()->getHeight());
			c = getClip()->getGenerator()->getWidth() * s;
			d = getClip()->getGenerator()->getHeight() * s;
		}

		float a = clipPanel->getScreenX();
		float b = clipPanel->getScreenY();

		float sx = (clipPanel->getWidth() - c) / 2.0;
		float sy = (clipPanel->getHeight()- d) / 2.0;

		clipArea->setPosition(sx,sy);
		clipArea->setDimension(c,d);

		getClip()->drawInput(sx+a+1.0, sy+b+1.0, c-2.0, d-2.0, false);  // dont crop => draw all input space

		clipArea->drawPoints(sx+a,sy+b); // draw points over input image

		ofSetColor(getClip()->baseColor);

		float ow = defaultW;
		float oh = getBestHeightbyFixedW(getClip()->getWidth(), getClip()->getHeight(), ow);
		getClip()->draw(this->getScreenX() + this->getWidth(), this->getScreenY() + 20.0, ow, oh);

		ofSetColor(255,192,128,255);
		drawBorderRectangle(this->getScreenX() + this->getWidth(), this->getScreenY() + 20.0, ow, oh);
	}
	ofPopStyle();
}

void phdGuiClip::onMouseMoved(ofMouseEventArgs & args) {

}

void phdGuiClip::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
}

void phdGuiClip::onMouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;
	if(!compareMouseEventArgs(argsPress, argsDrag)) {
		argsPress = argsDrag;
	}
}

void phdGuiClip::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click
	}
}

void phdGuiClip::onKeyPressed(ofKeyEventArgs & args) {
}

void phdGuiClip::onGuiEvent(phdGuiEventData &_eventData) {
	if(_eventData.control != NULL) {
		if(_eventData.eventType == petOnClick && _eventData.control->getID() == IDSHOWGUI) {
			if(toolArea != NULL) {

				if(!toolArea->isVisible()) 
					this->setDimension(this->getWidth(), this->getHeight() + toolArea->getHeight());
				else
					this->setDimension(this->getWidth(), this->getHeight() - toolArea->getHeight());

				toolArea->setVisible(!toolArea->isVisible());

			}
		} else if(_eventData.control->getID() == IDTRANSICTION && _eventData.control == transictionSlider) {

			if(_eventData.tag == "FADEIN") {
			} else if(_eventData.tag == "FADEOUT") {
			} else if(_eventData.tag == "DURATION") {
			} else if(_eventData.tag == "VALUE") {
			}

		} else if(_eventData.control->getID() == IDFADERMODE && _eventData.control == faderMode) {

			if(clip != NULL) clip->setFaderFilter(fadersManager()[faderMode->getValue(0)]);

		} else if(_eventData.control->getID() == IDCOLORBASE) {

		} else if(_eventData.eventType == petOnClick && _eventData.control->getID() == IDTIMELINE) {

			if(getClip() != NULL) {
				if(_eventData.tag == "PLAY") {
					getClip()->play();
				} else if(_eventData.tag == "PAUSE") {
					getClip()->pause();
				}
			}

		} else if(_eventData.eventType == petOnChangeValue && _eventData.control->getID() == IDSPEED) {

			if(getClip() != NULL && _eventData.control->getValue(0) != getClip()->getVelocity()) {
				getClip()->setVelocity(_eventData.control->getValue(0));
			}
		} else if(_eventData.control->getID() == IDDIRMODE) {

			phdGuiPushGroup * _group = (phdGuiPushGroup*) _eventData.control;

			int _item = (int)_group->getValue(0);
			if(getClip() != NULL && _item >= 0 && _item <= 1) {
				getClip()->setDirMode((phdDirectionMode)_item);
			}

		} else if(_eventData.control->getID() == IDLOOPMODE) {

			phdGuiPushGroup * _group = (phdGuiPushGroup*) _eventData.control;

			int _item = (int)_group->getValue(0) + 1;
			if(getClip() != NULL && _item >= 1 && _item <= 3) {
				getClip()->setLoopMode((ofLoopType)_item);
			}

		} else if(_eventData.eventType == petOnChangeValue && _eventData.control->getID() == IDMOUSEINPUTMODE) {

			phdGuiPushGroup * _group = (phdGuiPushGroup*) _eventData.control;

			int _item = (int)_group->getValue(0);

			if(getClip() != NULL && _item >= 0 && _item <= 2) {

				// defne mouse input mode
				clipArea->setPointsMode((phdGuiInputPointsMode) _item);

				// define points coord
				if(_item == 0) {

					clipArea->setPoint(0, getClip()->mouse[0], getClip()->mouse[1]);

				} else if(_item == 1) {

					clipArea->setPoint(0, getClip()->cropA[0], getClip()->cropA[1]);
					clipArea->setPoint(1, getClip()->cropB[0], getClip()->cropB[1]);

				} else if(_item == 2) {
					double _a, _b;
					getClip()->warper.getSource(0, _a, _b); clipArea->setPoint(0, _a / getClip()->getInputWidth(), _b / getClip()->getInputHeight());
					getClip()->warper.getSource(1, _a, _b); clipArea->setPoint(1, _a / getClip()->getInputWidth(), _b / getClip()->getInputHeight());
					getClip()->warper.getSource(2, _a, _b); clipArea->setPoint(2, _a / getClip()->getInputWidth(), _b / getClip()->getInputHeight());
					getClip()->warper.getSource(3, _a, _b); clipArea->setPoint(3, _a / getClip()->getInputWidth(), _b / getClip()->getInputHeight());
				}
			}

		} else if(_eventData.eventType == petOnChangeValue && _eventData.control->getID() == IDCROPMODE) {

			phdGuiPushGroup * _group = (phdGuiPushGroup*) _eventData.control;

			int _item = (int)_group->getValue(0);

			if(getClip() != NULL && _item >= 0 && _item <= 2) {
				// define crop mode => generator => crops to clip output
				getClip()->setCropMode((phdClipCropMode) _item);
			}

		} else if(getClip() != NULL && _eventData.eventType == petOnChangeValue && _eventData.control->getID() == IDPOINTS) {

			phdGuiPanelInputPoints * _input = (phdGuiPanelInputPoints*) _eventData.control;

			if(_input->getPointsMode() == gipMouse) {

				getClip()->mouse[0] = _input->getCoord(0, 0);
				getClip()->mouse[1] = _input->getCoord(0, 1);

			} else if(_input->getPointsMode() == gipRect) {

				float ax = _input->getCoord(0, 0);
				float ay = _input->getCoord(0, 1);
				float bx = _input->getCoord(1, 0);
				float by = _input->getCoord(1, 1);

				getClip()->setCropCoords(ax, ay, bx, by);

			} else if(_input->getPointsMode() == gipQuad) {

				float ax = _input->getCoord(0, 0);
				float ay = _input->getCoord(0, 1);
				float bx = _input->getCoord(1, 0);
				float by = _input->getCoord(1, 1);
				float cx = _input->getCoord(2, 0);
				float cy = _input->getCoord(2, 1);
				float dx = _input->getCoord(3, 0);
				float dy = _input->getCoord(3, 1);

				getClip()->setSrcWarpCoords(ax,ay,bx,by,cx,cy,dx,dy);

			}

		} else if(getClip() != NULL && _eventData.eventType == petOnChangeValue && _eventData.control->getID() == IDCLIPW) {

			if(getClip()->getWidth() != _eventData.control->getValue(0) && _eventData.control->getValue(0) > 64) {

				getClip()->setWidth((int)_eventData.control->getValue(0));

				float _h = (float)getClip()->getHeight() * this->getWidth() / (float)getClip()->getWidth();

				this->alignChildren();
			}

		} else if(getClip() != NULL && _eventData.eventType == petOnChangeValue && _eventData.control->getID() == IDCLIPH) {

			if(getClip()->getHeight() != _eventData.control->getValue(0) && _eventData.control->getValue(0) > 64) {

				getClip()->setHeight((int)_eventData.control->getValue(0));

				float _h = (float)getClip()->getHeight() * this->getWidth() / (float)getClip()->getWidth();

				this->alignChildren();
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
phdGuiSmallClip::phdGuiSmallClip(string _caption, phdTreeNodeData * _nodedata) {

	caption = _caption;
	setRectArea(200, 800, defaultW, defaultH + 12);

	resetPointers();

	setClip(NULL);

	// transiction fader-------------------------------------------
	transictionSlider = (phdGuiVerFader*) this->addLastChild(new phdGuiVerFader("T"));
	transictionSlider->setDimension(12, 20);
	transictionSlider->setAlign(gamLeft);
	transictionSlider->setID(IDTRANSICTION);
	if(getClip() != NULL) transictionSlider->setFader(&getClip()->fader);

	// timeline area -------------------------------------------
	timeline = (phdGuiHorTimeline*) this->addLastChild(new phdGuiHorTimeline("timeline"));
	timeline->setID(IDTIMELINE);
	timeline->setDimension(20,12);
	timeline->setAlign(gamBottom);
	if(getClip() != NULL) timeline->setTimeline(&getClip()->timeline);
	ofAddListener(timeline->guiEvent, this, &phdGuiSmallClip::onGuiEvent);

	// clip area -------------------------------------------
	this->clipArea = (phdGuiPanel*) this->addLastChild(new phdGuiPanel("cliparea"));
	this->clipArea->setID(IDPOINTS);
	this->clipArea->setDimension(20, 20);
	this->clipArea->setAlign(gamClient);
	this->clipArea->setMouseToParent(false);
}

phdGuiSmallClip::~phdGuiSmallClip() {

	ofRemoveListener(timeline->guiEvent, this, &phdGuiSmallClip::onGuiEvent);
	caption = "";

	setClip(NULL);

	resetPointers();
}

void phdGuiSmallClip::resetPointers() {
	clipArea = NULL;
	timeline = NULL;
	transictionSlider = NULL;
}

void phdGuiSmallClip::setClip(phdBaseClip * _value) {

	clip = _value;

	if(clip != NULL) {
		transictionSlider->setFader(&getClip()->fader);
		timeline->setTimeline(&getClip()->timeline);
	} else {
		if(transictionSlider != NULL) transictionSlider->setFader(NULL);
		if(timeline != NULL) timeline->setTimeline(NULL);
	}
}
/*
void phdGuiSmallClip::setGenerator(phdBaseGenerator * _value) {

	clip.setGenerator(_value);

	if(_value != NULL) {
		if(clipArea != NULL) setHeight(getBestHeightbyFixedW(_value->getWidth(), _value->getHeight(), defaultW) + 12);
		labelInfo = "W:" + ofToString(_value->getWidth(),  0);
		labelInfo+= "H:" + ofToString(_value->getHeight(), 0);
	} else {
		labelInfo = "W:0 H:0";
		if(clipArea != NULL) setHeight(defaultH + 12);
	}
}
*/
void phdGuiSmallClip::update() {

	// mark to update only once
	if(getClip() != NULL) {
		getClip()->markToUpdate();
		getClip()->update();
	}
	updateChildren();
}

void phdGuiSmallClip::draw(float _x, float _y) {

	ofPushStyle();

	if(timeline != NULL && getClip() != NULL && getClip()->getDuration() != 0) { // draw only timelines with duration != 0
		timeline->setVisible(true);
		timeline->setHeight(12);
	} else if(timeline != NULL) {
		timeline->setVisible(false);
	}

	drawChildren(_x,_y);

	if(getClip() != NULL) {

		float s = getMinScale(clipArea->getWidth(), clipArea->getHeight(), getClip()->getWidth(), getClip()->getHeight());

		float c = getClip()->getWidth() * s;
		float d = getClip()->getHeight() * s;

		float a = clipArea->getScreenX() + (clipArea->getWidth()-c)/2.0;
		float b = clipArea->getScreenY() + (clipArea->getHeight()-d)/2.0;

		getClip()->drawInput(a+1.0, b+1.0, c-2.0, d-2.0, false);  // dont crop => draw all input space

		ofSetColor(getClip()->baseColor);

		float ow = defaultW;
		float oh = getBestHeightbyFixedW(getClip()->getWidth(), getClip()->getHeight(), ow);
		getClip()->draw(this->getScreenX() + this->getWidth(), this->getScreenY() + 20.0, ow, oh);

		ofSetColor(255,192,128,255);
		drawBorderRectangle(this->getScreenX() + this->getWidth(), this->getScreenY() + 20.0, ow, oh);
	}
	//clip.draw(this->getScreenX(), this->getScreenY() + this->getHeight(), clip.getWidth(), clip.getHeight());

	ofPopStyle();
}

void phdGuiSmallClip::onMouseMoved(ofMouseEventArgs & args) {

}

void phdGuiSmallClip::onMousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;
}

void phdGuiSmallClip::onMouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;
	if(!compareMouseEventArgs(argsPress, argsDrag)) {
		argsPress = argsDrag;
	}
}

void phdGuiSmallClip::onMouseReleased(ofMouseEventArgs & args) {
	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click
	}
}

void phdGuiSmallClip::onKeyPressed(ofKeyEventArgs & args) {
}

void phdGuiSmallClip::onGuiEvent(phdGuiEventData &_eventData) {
	if(_eventData.control != NULL) {
		if(_eventData.control->getID() == IDTRANSICTION && _eventData.control == transictionSlider) {

			if(_eventData.tag == "FADEIN") {
			} else if(_eventData.tag == "FADEOUT") {
			} else if(_eventData.tag == "DURATION") {
			} else if(_eventData.tag == "VALUE") {
			}

		} else if(getClip() != NULL && _eventData.eventType == petOnClick && _eventData.control->getID() == IDTIMELINE) {

			if(_eventData.tag == "PLAY") {
				getClip()->play();
			} else if(_eventData.tag == "PAUSE") {
				getClip()->pause();
			}
		}
	}
}
