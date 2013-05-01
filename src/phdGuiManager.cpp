#include "phdGuiManager.h"

//--------------------------------------------------------------------------------------------------------------

bool compareMouseEventArgs(ofMouseEventArgs & argsA, ofMouseEventArgs & argsB) {
	return (argsA.x == argsB.x && argsA.y == argsB.y && argsA.button == argsB.button);
}

//----------------------------------------------------------------------------------------------------------------------
phdGuiInputField::phdGuiInputField() {
	editing = false;
	resultStringData = NULL;
	resultFloatData = NULL;
	resultIntData = NULL;
	maxLength = 10;
	fieldType = pftString;

	ownerControl = NULL;
	ownerIndex = 0;

	usingEvents = false;
	enableEvents();
}

phdGuiInputField::~phdGuiInputField() {
	resultStringData = NULL;
	resultFloatData = NULL;
	resultIntData = NULL;
	
	disableEvents();
}

bool phdGuiInputField::testInside(float _x, float _y) {
	return (_x >= x && _x <= x + w && _y >= y && _y <= y + h);
}

void phdGuiInputField::disable(phdEditFieldDisableMode _mode) {

	if(editing && fieldType == pftString && _mode != pefmEscapeKey) { // string

		// is editing a variable
		if(resultStringData != NULL) (*resultStringData) = holdStringData;

		// is editing a control
		if(ownerControl != NULL) {
			ownerControl->setString(holdStringData, ownerIndex);
			phdGuiEventData _data(petOnChangeString, ownerControl);
			ofNotifyEvent(guiEvent, _data, ownerControl);
		}
	} else if(editing && fieldType == pftFloat && _mode != pefmEscapeKey) { // float

		// is editing a variable
		if(resultFloatData != NULL) (*resultFloatData) = atof(holdStringData.c_str()); //ofToFloat(holdStringData);

		// is editing a control
		if(ownerControl != NULL) {
			ownerControl->setValue((*resultFloatData), ownerIndex);
			phdGuiEventData _data(petOnChangeValue, ownerControl);
			ofNotifyEvent(guiEvent, _data, ownerControl);
		}
	} else if(editing && fieldType == pftInteger && _mode != pefmEscapeKey) { // integer

		// is editing a variable
		if(resultIntData != NULL) (*resultIntData) = ofToInt(holdStringData);

		// is editing a control
		if(ownerControl != NULL) {
			ownerControl->setValue((*resultIntData), ownerIndex);
			phdGuiEventData _data(petOnChangeValue, ownerControl);
			ofNotifyEvent(guiEvent, _data, ownerControl);
		}
	}

	editing = false;
	resultStringData = NULL;
	resultFloatData = NULL;
	resultIntData = NULL;

	ownerControl = NULL;
	ownerIndex = 0;
}

inline void phdGuiInputField::setCursorPos(int _pos) {
	if(_pos >= 0 && _pos <= maxLength) cursorPos = _pos;
}

void phdGuiInputField::processKey(int _key) {

	if(!editing) return;

	switch(_key) {
	case OF_KEY_RETURN :
		disable(pefmEnterKey);
		break;
	case OF_KEY_ESC :
		disable(pefmEscapeKey);
		break;
	case OF_KEY_BACKSPACE :
		if (cursorPos > 0) {
			setCursorPos(cursorPos - 1);
			 holdStringData.erase(holdStringData.begin() + cursorPos);
		} break;
	case OF_KEY_DEL :
		if (holdStringData.size() > cursorPos) {
			holdStringData.erase(holdStringData.begin() + cursorPos);
		} break;
	case OF_KEY_LEFT :
		if (cursorPos > 0){
			setCursorPos(cursorPos - 1);
		} break;
	case OF_KEY_RIGHT :
		if (cursorPos < holdStringData.size()){
			setCursorPos(cursorPos + 1);
		} break;
	case OF_KEY_HOME :
		setCursorPos(0);
		break;
	case OF_KEY_END :
		setCursorPos(holdStringData.size());
		break;
	default:
		if(_key >= 32 && _key <= 126 && holdStringData.size() < maxLength) {
			if(fieldType == pftString) {
				holdStringData.insert(holdStringData.begin() + cursorPos, _key);
				setCursorPos(cursorPos + 1);
			} else {
				bool isNumber = (_key >= 48 && _key <= 57);
				bool dotFound = (std::string::npos != holdStringData.find('.'));
				bool insMinus = (_key == '-' && cursorPos == 0);
				if(fieldType == pftFloat && (isNumber || (_key == '.' && !dotFound) || insMinus)) {
					holdStringData.insert(holdStringData.begin() + cursorPos, _key);
					setCursorPos(cursorPos + 1);
				} else if(fieldType == pftInteger && (isNumber || insMinus)) {
					holdStringData.insert(holdStringData.begin() + cursorPos, _key);
					setCursorPos(cursorPos + 1);
				}
			}
		}
	}
}

void phdGuiInputField::editInteger(float _x, float _y, float _w, float _h, int & _result, int _length) {
	fieldType = pftInteger;
	x = _x; y = _y; w = MAX(_w, _length*8+4); h = _h;
	holdStringData = "";
	maxLength = _length;
	resultIntData = &_result;
	if(resultIntData != NULL) {
		holdStringData = ofToString((*resultIntData));
	}
	editing = true;
	cursorPos = MAX(0, MIN(maxLength, holdStringData.size()));
}


void phdGuiInputField::editInteger(float _x, float _y, float _w, float _h, phdGuiControl * _gc, int _index, int _length) {
	ownerControl = _gc;
	ownerIndex = _index;
	if(_gc != NULL) holdIntData = (int) _gc->getValue(_index);
	editInteger(_x,_y,_w,_h,holdIntData,_length);
}

void phdGuiInputField::editFloat(float _x, float _y, float _w, float _h, float & _result, int _length, int _precision) {
	fieldType = pftFloat;
	x = _x; y = _y; w = MAX(_w, _length*8+4); h = _h;
	holdStringData = "";
	maxLength = _length;
	resultFloatData = &_result;
	if(resultFloatData != NULL) {
		holdStringData = ofToString((*resultFloatData), _precision);
	}
	editing = true;
	cursorPos = MAX(0, MIN(maxLength, holdStringData.size()));
}


void phdGuiInputField::editFloat(float _x, float _y, float _w, float _h, phdGuiControl * _gc, int _index, int _length, int _precision) {
	ownerControl = _gc;
	ownerIndex = _index;
	if(_gc != NULL) holdFloatData = _gc->getValue(_index);
	editFloat(_x,_y,_w,_h,holdFloatData,_length,_precision);
}

void phdGuiInputField::editString(float _x, float _y, float _w, float _h, string & _result, int _length) {
	fieldType = pftString;
	x = _x; y = _y; w = MAX(_w, _length*8+4); h = _h;
	holdStringData = "";
	maxLength = _length;
	resultStringData = &_result;
	if(resultStringData != NULL) {
		holdStringData = (*resultStringData);
	}
	editing = true;
	cursorPos = MAX(0, MIN(maxLength, holdStringData.size()));
}

void phdGuiInputField::editString(float _x, float _y, float _w, float _h, phdGuiControl * _gc, int _index, int _length) {
	fieldType = pftString;
	x = _x; y = _y; w = MAX(_w, _length*8+4); h = _h;

	ownerControl = _gc;
	ownerIndex = _index;
	if(_gc != NULL) holdStringData = _gc->getString(_index);

	maxLength = _length;
	resultStringData = NULL;
	if(resultStringData != NULL) {
		holdStringData = (*resultStringData);
	}
	editing = true;
	cursorPos = MAX(0, MIN(maxLength, holdStringData.size()));
}

void phdGuiInputField::draw(float _x, float _y, float _w, float _h) {

	if(!editing) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(x, y);

	drawFilledBorderRectangle(0, 0, w, h, colors[0], colors[1]);

	ofSetColor(colors[2]);	ofDrawBitmapString(holdStringData, 4, 11);
	
	float timeFrac = ofGetElapsedTimef();
	timeFrac -= (int)timeFrac;

	ofSetColor(colors[2] * MIN(1.0, timeFrac + 0.5));

	ofSetLineWidth(1.5f);
	ofLine(cursorPos*8.0 + 4.0, 2.0, cursorPos*8.0 + 4.0, 12.0);

	ofPopMatrix();
	ofPopStyle();
}

void phdGuiInputField::keyPressed(ofKeyEventArgs & args) {

	processKey(args.key);

}

void phdGuiInputField::enableEvents() {
	if(!usingEvents) {
        ofAddListener(ofEvents().keyPressed, this, &phdGuiInputField::keyPressed);
		usingEvents = true;
	}
}

void phdGuiInputField::disableEvents(){
	if(usingEvents) {
		ofRemoveListener(ofEvents().keyPressed, this, &phdGuiInputField::keyPressed);
		usingEvents = false;
	}
}

//--------------------------------------------------------------------------------------------------------------
phdGuiEventData::phdGuiEventData() {
	control = NULL;
	eventType = petOnChangeValue;
}

phdGuiEventData::phdGuiEventData(phdGuiEventType _eventType, phdGuiControl * _control, string _tag) {
	eventType = _eventType;
	control = _control;
	tag = _tag;
}

//--------------------------------------------------------------------------------------------------------------
phdGuiControl::phdGuiControl(string _caption, phdTreeNodeData * _nodedata) {
	popupMenu = NULL;
	caption = _caption;
	id = group = -1;
	align = gamSize;
	x = y = 0.0;
	w = h = 10;
	manager = NULL;
	popupMenu = NULL;
	visible = true;
	border = true;
}

phdGuiControl::~phdGuiControl() {
	phdTreeNode::~phdTreeNode();
}

phdGuiControl * phdGuiControl::getHigherParent() {

	phdGuiControl * _cursor = this;
	while(_cursor != NULL && _cursor->getParent() != getManager()->getRoot()) {
		_cursor = _cursor->getParent();
	}
	if(_cursor != NULL && _cursor->getParent() == getManager()->getRoot()) return _cursor;

	return NULL;
}

phdGuiManager * phdGuiControl::getManager() {

	if(manager != NULL) return manager;

	phdGuiControl * _cursor = getParent();
	while(_cursor != NULL) {

		phdGuiManager * _manager = _cursor->getManager();

		if(_manager != NULL) {
			manager = _manager;
			return manager;
		}

		_cursor = _cursor->getParent();
	}

	return NULL;
}

void phdGuiControl::setManager(phdGuiManager * _manager) {
	manager = _manager;
	phdGuiControl * curControl = getFirstChild();
	while(curControl != NULL) {
		curControl->setManager(_manager);
		curControl = curControl->getNextSibling();
	}
}

void phdGuiControl::setPopupMenu(phdPopupMenu * _menu) {
	if(popupMenu != NULL && _menu == NULL) {
		popupMenu->setCaller(NULL);
		popupMenu->setOwner(NULL);
		popupMenu->setParent(NULL);
	}
	popupMenu = _menu;
}

phdPopupMenu * phdGuiControl::getPopupMenu() {

	if(popupMenu != NULL) return popupMenu;

	phdGuiControl * _cursor = getParent();
	while(_cursor != NULL && _cursor != getManager()->getRoot()) {
		if(_cursor->getPopupMenu() != NULL) return _cursor->getPopupMenu();
		_cursor = _cursor->getParent();
	}

	return getManager()->getPopupMenu();
}

float phdGuiControl::getScreenX() { 
	float _result = this->getX();
	phdGuiControl * _control = getParent();
	while(_control != NULL) {
		_result += _control->getX();
		_control = _control->getParent();
	}
	return _result;
}

float phdGuiControl::getScreenY() { 
	float _result = this->getY();
	phdGuiControl * _control = getParent();
	while(_control != NULL) {
		_result += _control->getY();
		_control = _control->getParent();
	}
	return _result;
}

void phdGuiControl::setWidth(float _value) {
	if(this->w != _value) {
		this->w = _value;
		if(this->align == gamFitWidth || this->align == gamTop || this->align == gamBottom) {
			if(getParent() != NULL) getParent()->alignChildren();
		}
	}
}

void phdGuiControl::setHeight(float _value) {
	if(this->h != _value) {
		this->h = _value;
		if(this->align == gamFitHeight || this->align == gamLeft || this->align == gamRight) {
			if(getParent() != NULL) getParent()->alignChildren();
		}
	}
}

void phdGuiControl::setPosition(float _x, float _y) {
	if(getParent() != NULL) {
		if(true || testBounds(_x,_y)) {
			x = MAX(0, MIN(_x, getParent()->getWidth()-w));
			y = MAX(0, MIN(_y, getParent()->getHeight()-h));
		}
	} else {
		x = MAX(0, MIN(_x, ofGetWidth()-w));
		y = MAX(0, MIN(_y, ofGetHeight()-h));
	}
}

void phdGuiControl::setDimension(float _w, float _h, bool _normalized) {
	if(_normalized) {
		if(getParent() == NULL) return;
		_w = getParent()->getWidth() * _w - 3.0;
		_h = getParent()->getHeight() * _h - 3.0;
	}
	if(this->w != _w || this->h != _h) {
		this->w = _w;
		this->h = _h;
		if(this->align != gamSize) {
			if(getParent() != NULL) getParent()->alignChildren();
		}
	}
}

void phdGuiControl::setRectArea(float _x, float _y, float _w, float _h, bool _check) {
	this->x = _x;
	this->y = _y;
	if(_check) {
		setWidth(_w);
		setHeight(_h);
	} else {
		this->w = _w;
		this->h = _h;
	}
}

void phdGuiControl::alignChildren() {

	float _m = 2.0;

	if(getManager() != NULL) _m = getManager()->getGap();

	float _gapL = _m, _gapT = _m, _gapR = _m, _gapB = _m;

	phdGuiControl * _cursor = getFirstChild();
	while(_cursor != NULL) {

		if(_cursor->isVisible()) {
			if(_cursor->getAlign() == gamFitWidth || _cursor->getAlign() == gamTop || _cursor->getAlign() == gamBottom) {

				float _w = getWidth() - _gapL - _gapR;
				float _h = _cursor->getHeight();

				if(_cursor->getAlign() == gamTop) {
					_cursor->setRectArea( _gapL, _gapT, _w, _h, false);
					_gapT += _h + _m;
				} else if(_cursor->getAlign() == gamBottom) {
					_cursor->setRectArea( _gapL, getHeight() - _gapB - _h, _w, _h, false);
					_gapB += _h + _m;
				} else {
					_cursor->setRectArea( _gapL, _cursor->getY(), _w, _h, false);
				}
			} else if(_cursor->getAlign() == gamFitHeight || _cursor->getAlign() == gamLeft || _cursor->getAlign() == gamRight) {

				float _w = _cursor->getWidth();
				float _h = getHeight() - _gapT - _gapB;

				if(_cursor->getAlign() == gamLeft) {
					_cursor->setRectArea( _gapL, _gapT, _w, _h, false);
					_gapL += _w + _m;
				} else if(_cursor->getAlign() == gamRight) {
					_cursor->setRectArea( getWidth() - _gapR - _w, _gapT, _w, _h, false);
					_gapR += _w + _m;
				} else {
					_cursor->setRectArea( _cursor->getX(), _gapT, _w, _h, false);
				}
			} else if(_cursor->getAlign() == gamClient) {
				_cursor->setRectArea( _gapL, _gapT, getWidth() - _gapL - _gapR, getHeight() - _gapT - _gapB);
			}

			_cursor->alignChildren(); // propagate to children
		}

		_cursor = _cursor->getNextSibling();
	}
}

bool phdGuiControl::isInside(float _x, float _y) {
	return visible && !(_x < x || _x > x + w || _y < y || _y > y + h);
}

bool phdGuiControl::testBounds(float _x, float _y) {

	float _oldX = this->x;
	float _oldY = this->y;

	this->x = _x;
	this->y = _y;

	bool isOver = false;

	if(getParent() != NULL) {
		phdGuiControl * _cursor = getParent()->getFirstChild();
		while(_cursor != NULL) {
			if(_cursor != this) {
				if(_cursor->isInside(_x, _y)) { isOver = true; break; }
				if(_cursor->isInside(_x+w, _y)) { isOver = true; break; }
				if(_cursor->isInside(_x+w, _y+h)) { isOver = true; break; }
				if(_cursor->isInside(_x, _y+h)) { isOver = true; break; }
				if(isInside(_cursor->x, _cursor->y)) { isOver = true; break; }
				if(isInside(_cursor->x+_cursor->w, _cursor->y)) { isOver = true; break; }
				if(isInside(_cursor->x+_cursor->w, _cursor->y+_cursor->h)) { isOver = true; break; }
				if(isInside(_cursor->x, _cursor->y+_cursor->h)) { isOver = true; break; }
			}
			_cursor = _cursor->getNextSibling();
		}
	}

	this->x = _oldX;
	this->y = _oldY;

	return !isOver;
}

phdGuiControl * phdGuiControl::hitTest(float _x, float _y) {
	if(this->isInside(_x + this->x,_y + this->y)) {
		phdGuiControl * curControl = getFirstChild();
		while(curControl != NULL) {
			phdGuiControl * insideControl = curControl->hitTest(_x - curControl->x, _y - curControl->y);
			if(insideControl != NULL) return insideControl;
			curControl = curControl->getNextSibling();
		}
		return this;
	}
	return NULL;
}

phdGuiControl * phdGuiControl::findById(int _id) {
	phdGuiControl * _control = getFirstChild();
	while(_control != NULL) {
		if(_control->getID() == _id) return _control;
		phdGuiControl * _result = _control->findById(_id);
		if(_result != NULL) return _result;
		_control = _control->getNextSibling();
	}
	return NULL;
}

void phdGuiControl::update() {

	// updates childrens of this control
	updateChildren();

}

void phdGuiControl::updateChildren() {
	phdGuiControl * curControl = getFirstChild();
	while(curControl != NULL) {
		curControl->update();
		curControl = curControl->getNextSibling();
	}
}

void phdGuiControl::draw(float _x, float _y) {

	if(!isVisible() || getManager() == NULL) return;

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(getScreenX(), getScreenY(), 0);

	drawFilledBorderRectangle(0, 0, getWidth(), getHeight(), getManager()->getFillColor(pcmtNormal), getManager()->getBorderColor(pcmtNormal));

	ofSetColor(getManager()->getTextColor(pcmtNormal));
	ofDrawBitmapString(caption, (getWidth()-(caption.size()*8.0))/2.0, getHeight() / 2.0 + 4.0);

	ofPopMatrix();
	ofPopStyle();

	drawChildren(_x, _y);
}

void phdGuiControl::drawChildren(float _x, float _y, bool _firstToLast) {

	ofPushStyle();

	phdGuiControl * curControl = getFirstChild();
	if(!_firstToLast) curControl = getLastChild();

	while(curControl != NULL) {

		curControl->draw(_x + getX(), _y + getY());

		if(_firstToLast) curControl = curControl->getNextSibling();
		else curControl = curControl->getPrevSibling();
	}

	ofPopStyle();
}

bool phdGuiControl::updateHitTestInfo(float _x, float _y) {

	hitInfo.clear();

	hitInfo.infoType = phtOutside;

	return (hitInfo.infoType != phtOutside);
}

void phdGuiControl::onMouseMoved(ofMouseEventArgs & args) {

	updateHitTestInfo(args.x, args.y);

}

void phdGuiControl::onMousePressed(ofMouseEventArgs & args) {
}

void phdGuiControl::onMouseDragged(ofMouseEventArgs & args) {
}

void phdGuiControl::onMouseReleased(ofMouseEventArgs & args) {
}

void phdGuiControl::onKeyPressed(ofKeyEventArgs & args) {
}

void phdGuiControl::onGuiEvent(phdGuiEventData &_eventData) {
}

//--------------------------------------------------------------------------------------------------------------
phdGuiManager::phdGuiManager() {

	usingEvents = false;

	root.caption = "phd";
	root.setRectArea(0, 0, 1024, 768);
	root.setManager(this);

	currentControl = &root;
	focusedControl = selectedControl = NULL;

	controlGap = 2.0;

	genColorsFromBase(ofColor(128, 160, 220, 128), fillColors[0], fillColors[1], fillColors[2], 1.0, 0.75, 1.25, true);	
	genColorsFromBase(ofColor(220, 192, 160, 255), textColors[0], textColors[1], textColors[2], 1.0, 0.75, 1.25, true);	
	genColorsFromBase(ofColor(160, 192, 220, 220), borderColors[0], borderColors[1], borderColors[2], 1.0, 0.75, 1.25, true);

	inputField.setupColors(getFillColor(pcmtSelected), getBorderColor(pcmtSelected), getTextColor(pcmtSelected));

	enableEvents();
}

phdGuiManager::~phdGuiManager() {
	disableEvents();
	currentControl = focusedControl = selectedControl = NULL;
}

phdGuiControl & phdGuiManager::findByCaption(string _caption) {
	phdGuiControl * _result = (phdGuiControl*) root.findByCaption(_caption);
	if(_result != NULL) return (*_result);
	return root;
}

phdGuiControl & phdGuiManager::findById(int _id) {
	phdGuiControl * _result = (phdGuiControl*) root.findById(_id);
	if(_result != NULL) return (*_result);
	return root;
}

void phdGuiManager::setSelected(phdGuiControl * _control) { 

	if(selectedControl != _control) {

		selectedControl = _control;

		if(selectedControl != NULL) {
			phdGuiControl * _higher = selectedControl->getHigherParent();
			if(_higher != NULL) {
				_higher->linkAs(&root, npmFirstChild);
			}
		}
	}
}

phdGuiControl * phdGuiManager::addControl(phdGuiControl * _newControl) {
	if(_newControl != NULL) {
		control().addLastChild(_newControl);
		_newControl->setManager(this);
		currentControl = _newControl;
	} else {
		if(currentControl == NULL) currentControl = &root;
	}
	return currentControl;
}

void phdGuiManager::delNode(string _caption) {
	phdGuiControl * _node = (phdGuiControl*) root.findByCaption(_caption);
	if(_node != &root) {
		if(_node == currentControl) currentControl = &root;
		delete _node;
	}
}

void phdGuiManager::delNode(int _id) {
	phdGuiControl * _node = (phdGuiControl*) root.findById(_id);
	if(_node != &root) {
		if(_node == currentControl) currentControl = &root;
		delete _node;
	}
}

phdGuiControl & phdGuiManager::control(string _caption) {
	if(_caption != "") {
		phdGuiControl * _result = (phdGuiControl*) root.findByCaption(_caption);
		if(_result != NULL) return (*_result);
	}

	if(currentControl == NULL) {
		currentControl = &root;
	}

	return (*currentControl);
}

void phdGuiManager::update() {

	root.update();

}

void phdGuiManager::draw() {

	// draw controls
	root.drawChildren(root.getX(), root.getY(), false);

	if(getSelected() != NULL) {
		drawBorderRectangle(getSelected()->getScreenX(), getSelected()->getScreenY(), getSelected()->getWidth(), getSelected()->getHeight(), ofColor(255,192,128,255));
		ofSetColor(255,255,255,255);
	}

	// draw input field over controls
	inputField.draw();

	// draw popups over everything
	popupsManager().draw();
}

void phdGuiManager::mouseMoved(ofMouseEventArgs & args) {

	if(popupsManager().indexOfPopupUnderMouse(args.x,args.y) != -1) {
		popupsManager().onMouseMoved(args);
		return;
	}

	phdGuiControl * _ctrl = root.hitTest(args.x, args.y);

	if(getSelected() == _ctrl && _ctrl != NULL) {// && getSelected()->updateHitTestInfo(args.x, args.y)) {
		getSelected()->onMouseMoved(args);
	}

	setFocused(_ctrl);
}

void phdGuiManager::mousePressed(ofMouseEventArgs & args) {
	argsPress = args;
	argsDrag.x = -1; argsDrag.y = -1; argsDrag.button = -1;

	if(popupsManager().countVisibles() > 0) {
		if(popupsManager().indexOfPopupUnderMouse(args.x,args.y) == -1) { // dont over any popup
			popupsManager().hidesAll(); // hides every popup and continue
			setSelected(NULL);
		} else {
			popupsManager().onMousePressed(args);
			return;
		}
	}

	// is editing field with keyboard
	if(inputField.isEditing() && !inputField.testInside(args.x,args.y)) {
		inputField.disable(pefmClickOut);
	}

	setSelected(getFocused());

	if(getSelected() != NULL) getSelected()->onMousePressed(args);
}

void phdGuiManager::mouseDragged(ofMouseEventArgs & args) {
	argsDrag = args;

	if(popupsManager().countVisibles() > 0) {
		if(popupsManager().indexOfPopupUnderMouse(args.x,args.y) == -1) { // isnt over any popup
			popupsManager().hidesAll(); // hides every popup and continue
			setSelected(getFocused());
		} else {
			popupsManager().onMouseDragged(args);
			return;
		}
	}

	if(!compareMouseEventArgs(argsPress, argsDrag)) {
		if(getSelected() != NULL) getSelected()->onMouseDragged(args);
		argsPress = argsDrag;
	}
}

void phdGuiManager::mouseReleased(ofMouseEventArgs & args) {

	if(popupsManager().countVisibles() > 0) {
		popupsManager().onMouseReleased(args);
		return;
	}

	if(!compareMouseEventArgs(argsPress, argsDrag)) { // its a click

		setSelected(getFocused());

		if(argsPress.button == 0) {

		} else if(args.button == 2) { // right click
			if(getSelected() == &root) { // getSelected == &root when click inside manager
				phdPopupMenu * _popup = getPopupMenu();
				if(_popup != NULL) { 
					getPopupMenu()->setOwner((void*)this);
					getPopupMenu()->popup(args.x, args.y, (void*)this, "GUIMANAGER");
					return;
				}
			} else if(getSelected() != NULL) {
				phdPopupMenu * _popup = getSelected()->getPopupMenu();
				if(_popup != NULL) {
					_popup->setOwner((void*)selectedControl);
					_popup->popup(args.x, args.y, (void*)selectedControl, "GUICONTROL");
					return;
				}
			} // getSelected == NULL when click outside manager
		}
	}

	if(getSelected() != NULL) getSelected()->onMouseReleased(args);
}

void phdGuiManager::keyPressed(ofKeyEventArgs & args) {

	//masterInputField().processKey(args.key);

}

void phdGuiManager::enableEvents() {
	if(!usingEvents) {
		ofAddListener(ofEvents().mouseMoved, this, &phdGuiManager::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &phdGuiManager::mousePressed);
		ofAddListener(ofEvents().mouseDragged, this, &phdGuiManager::mouseDragged);
		ofAddListener(ofEvents().mouseReleased, this, &phdGuiManager::mouseReleased);
        ofAddListener(ofEvents().keyPressed, this, &phdGuiManager::keyPressed);
		usingEvents = true;
	}
}

void phdGuiManager::disableEvents(){
	if(usingEvents) {
		ofRemoveListener(ofEvents().mouseMoved, this, &phdGuiManager::mouseMoved);
		ofRemoveListener(ofEvents().mousePressed, this, &phdGuiManager::mousePressed);
		ofRemoveListener(ofEvents().mouseDragged, this, &phdGuiManager::mouseDragged);
		ofRemoveListener(ofEvents().mouseReleased, this, &phdGuiManager::mouseReleased);
		ofRemoveListener(ofEvents().keyPressed, this, &phdGuiManager::keyPressed);
		usingEvents = false;
	}
}

