#include "phdGuiPushGroup.h"

//--------------------------------------------------------------------------------------------------------------
phdGuiPushGroup::phdGuiPushGroup(string _caption, phdTreeNodeData * _nodedata) {

	caption = _caption;
	setDimension(60,40);
	setAlignment(pbgaHorizontal);

}

phdGuiPushGroup::~phdGuiPushGroup() {
}

// override this to setID for all sliders
void phdGuiPushGroup::setID(int _id) {
	if(id != _id) {
		id = _id;
		int _item = id;
		phdGuiControl * _control = this->getFirstChild();
		while(_control != NULL){
			_control->setID(++_item);
			_control = _control->getNextSibling();
		}
	}
}

void phdGuiPushGroup::setGroup(int _group) {
	if(group != _group) {
		group = _group;
		phdGuiControl * _control = this->getFirstChild();
		while(_control != NULL){
			_control->setGroup(group);
			_control = _control->getNextSibling();
		}
	}
}

// use these methods to control internal values
float phdGuiPushGroup::getValue(int _index) {

	int _result = 0;

	phdGuiPushButton * _control = (phdGuiPushButton*) this->getFirstChild();
	while(_control != NULL){
		if(_control->isPushed()) return _result;
		_result += 1;
		_control = (phdGuiPushButton*) _control->getNextSibling();
	}

	return -1;
}

void phdGuiPushGroup::setValue(float _value, int _index) {

	phdGuiPushButton * _button = (phdGuiPushButton*) this->getChildByIndex((int)_value);

	if(_button != NULL)	_button->setPushed(true);
}

phdGuiPushButton * phdGuiPushGroup::addButton(phdGuiPushButton * _button) {

	if(_button != NULL) {
		this->addLastChild(_button);
		_button->setGroup(this->getGroup());
		_button->setID(this->getID());
		setAlignment(alignment, true); // force recalculations
		ofAddListener(_button->guiEvent, this, &phdGuiPushGroup::onGuiEvent);
	}

	return _button;
}

void phdGuiPushGroup::setAlignment(phdGuiPushGroupAlignmentType _alignment, bool _force) {

	if(_force || alignment != _alignment) {

		alignment = _alignment;

		float _gap = 2.0; if(getManager() != NULL) _gap = getManager()->getGap();

		phdGuiPushButton * _button = (phdGuiPushButton*) this->getFirstChild();

		while(_button != NULL){
			if(alignment == pbgaVertical) {
				float _size = this->getHeight() / this->getChildCount() - _gap;
				_button->setDimension(this->getWidth(), _size);
				_button->setAlign(gamTop);

			} else {
				float _size = this->getWidth() / this->getChildCount() - _gap;
				_button->setDimension(_size, this->getHeight());
				_button->setAlign(gamLeft);
			}

			_button = (phdGuiPushButton*) _button->getNextSibling();
		}
	}
}

void phdGuiPushGroup::draw(float _x, float _y) {
	drawChildren(_x,_y);
}

void phdGuiPushGroup::onMouseMoved(ofMouseEventArgs & args) {
}

void phdGuiPushGroup::onMousePressed(ofMouseEventArgs & args) {
}

void phdGuiPushGroup::onMouseDragged(ofMouseEventArgs & args) {
}

void phdGuiPushGroup::onMouseReleased(ofMouseEventArgs & args) {
}

void phdGuiPushGroup::onKeyPressed(ofKeyEventArgs & args) {
}

void phdGuiPushGroup::onGuiEvent(phdGuiEventData &_eventData) {
	if(_eventData.control != NULL) {
		if(_eventData.eventType == petOnClick) {
			phdGuiEventData _data(petOnChangeValue, this);
			ofNotifyEvent(guiEvent, _data, this);
		}
	}
}
