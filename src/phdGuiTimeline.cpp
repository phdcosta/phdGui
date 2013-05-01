#include "phdGuiTimeline.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiHorTimeline::phdGuiHorTimeline(string _caption, phdTreeNodeData * _nodedata) {
	caption = _caption;
	setBorder(false);
	setTimeline(NULL);
}

phdGuiHorTimeline::~phdGuiHorTimeline() {
}

float phdGuiHorTimeline::getValue(int _index) {
	if(timeline != NULL) {
		if(_index == ptlvStartPos) return timeline->getStartPos();
		if(_index == ptlvEndPos) return timeline->getEndPos();
		if(_index == ptlvCurPos) return timeline->getCurPos();
		if(_index == ptlvVelocity) return timeline->getVelocity();
		if(_index == ptlvDuration) return timeline->getDuration();
		if(_index == ptlvLoopMode) return timeline->getLoopMode();
		if(_index == ptlvDirMode) return timeline->getDirMode();
	}
	return 0.0;
}

void phdGuiHorTimeline::setValue(float _value, int _index) {
	if(timeline != NULL) {
		if(_index == ptlvStartPos)	timeline->setStartPos(_value);
		if(_index == ptlvEndPos)	timeline->setEndPos(_value);
		if(_index == ptlvCurPos)	timeline->setCurPos(_value);
		if(_index == ptlvVelocity)	timeline->setVelocity(_value);
		if(_index == ptlvDuration)	timeline->setDuration(_value);
		if(_index == ptlvLoopMode)	timeline->setLoopMode((ofLoopType) (int)_value);
		if(_index == ptlvDirMode)	timeline->setDirMode((phdDirectionMode) (int)_value);

		phdGuiEventData _data(petOnChangeValue, this);
		ofNotifyEvent(guiEvent, _data, this);
	}
}

void phdGuiHorTimeline::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	if(timeline != NULL) {
		ofPushStyle();
		ofPushMatrix();
		//ofTranslate(getScreenX(), getScreenY(), 0);

		ofColor _fill, _border, _text;
		colorsManager().getColors(_fill, _border, _text, (this == getManager()->getSelected()), (this == getManager()->getFocused()));

		timeline->setCoords(getScreenX(), getScreenY(), getWidth(), getHeight());
		timeline->draw(_fill, _border, _text);

		ofPopMatrix();
		ofPopStyle();
	}
}

phdTimelineHitItemType phdGuiHorTimeline::updateHitTest(float _x, float _y) {

	hitTimeline = tliOutside;

	if(timeline != NULL) hitTimeline = timeline->updateHitTestInfo(_x, _y);

	return hitTimeline;
}

void phdGuiHorTimeline::onMouseMoved(ofMouseEventArgs & args) {

	updateHitTest(args.x, args.y);

}

void phdGuiHorTimeline::onMousePressed(ofMouseEventArgs & args) {

	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;

	updateHitTest(args.x, args.y);
}

void phdGuiHorTimeline::onMouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;
	if(!compareMouseEventArgs(argsPress, argsDrag)) {

		if(timeline != NULL) {
			if(hitTimeline == tliStartMark || hitTimeline == tliEndMark || (hitTimeline == tliCursor && timeline->isPaused())) {

				phdGuiEventData _data(petOnDrag, this);

				double _pos = timeline->getTimeByX(args.x);

				if(hitTimeline == tliStartMark) {

					_data.tag = "STARTPOS";
					timeline->setStartPos(_pos);

				} else if(hitTimeline == tliEndMark) {

					_data.tag = "ENDPOS";
					timeline->setEndPos(_pos);

				} else if(hitTimeline == tliCursor && timeline->isPaused()) {

					_data.tag = "CURPOS";
					timeline->setCurPos(_pos);
				}

				ofNotifyEvent(guiEvent, _data, this);
			}
		}

		argsPress = argsDrag;
	}
}

void phdGuiHorTimeline::onMouseReleased(ofMouseEventArgs & args) {

	if(!compareMouseEventArgs(argsPress, argsDrag)) { // drag != press => has a click

		if(timeline != NULL && hitTimeline == tliButton) {

			phdGuiEventData _data(petOnClick, this);

			if(timeline->isPaused()) {
				_data.tag = "PLAY";
			} else {
				_data.tag = "PAUSE";
			}
			timeline->setPaused(!timeline->isPaused());

			ofNotifyEvent(guiEvent, _data, this);
		}
	}
}

void phdGuiHorTimeline::onKeyPressed(ofKeyEventArgs & args) {
}
