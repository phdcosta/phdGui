#include "phdPopupMenu.h"

//--------------------------------------------------------------------------------------------------
phdPopupsManager globalPopusManager;

//--------------------------------------------------------------------------------------------------

phdPopupsManager & popupsManager() {
	return globalPopusManager;
}

//--------------------------------------------------------------------------------------------------------------
phdMenuItem::phdMenuItem() {
	name = "menuItem";
	id = -1;
	parent = NULL;
	subMenu = NULL;
	group = -1;
	selected = false;
}

phdMenuItem::phdMenuItem(string _name, int _id, phdPopupMenu * _parent, phdPopupMenu * _subMenu, int _group) {
	name = _name;
	id = _id;
	parent = _parent;
	subMenu = _subMenu;
	group = _group;
	selected = false;
	if(subMenu != NULL) subMenu->setParent(_parent);
}

phdMenuItem::~phdMenuItem() {
	parent = NULL;
}

string phdMenuItem::getFullPath() {
	phdPopupMenu * _parent = parent;
	string _path = name + ":"; // item name
	while(_parent != NULL) { // while has parent
		if(_parent != NULL) _path = _parent->getName() + ":" + _path; // insert menuName on path
		_parent = _parent->getParent();
	}
	return _path;
}

int phdMenuItem::getLevel() {
	phdPopupMenu * _parent = parent;
	int _result = -1; // item level
	while(_parent != NULL) { // while has parent
		if(_parent != NULL) _result += 1; // sum level
		_parent = _parent->getParent();
	}
	return _result;
}

string phdMenuItem::getFullCallerTag() {
	phdPopupMenu * _parent = parent;
	string _result = "";
	while(_parent != NULL) { // while has parent
		if(_parent != NULL) _result = _parent->getCallerTag();
		_parent = _parent->getParent();
	}
	return _result;
}

//--------------------------------------------------------------------------------------------------------------
phdPopupMenu::phdPopupMenu(phdPopupMenu * _parent) {
	owner = NULL;
	parent = _parent;
	visible = false;
	hitInfo.clear();
}

phdPopupMenu::~phdPopupMenu() {
	freeItems();
	items.clear();
}

void phdPopupMenu::freeItems() {
	for(int i = 0; i <items.size(); i++) {
		//if(items[i]->subMenu != NULL) items[i]->subMenu->freeItems(); // free items dont work with menu testApp.h
		delete items[i];
	}
	items.clear();
}

int phdPopupMenu::indexOf(phdMenuItem * _value) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i] == _value) return i;
	}
	return -1;
}

int phdPopupMenu::indexByName(string _value) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->name == _value) return i;
	}
	return -1;
}

phdPopupMenu * phdPopupMenu::getRootMenu() {
	phdPopupMenu * _result = this;
	while(_result->parent != NULL) { // while has parent
		_result = _result->parent;
	}
	return _result;
}

void phdPopupMenu::setVisible(bool _value) {
	if(_value != visible) {
		visible = _value;
		if(!visible) {
			for(int i = 0; i < items.size(); i++) {
				if(items[i]->subMenu != NULL) items[i]->subMenu->setVisible(false); // ocults every subMenu
			}
		} else {
			if(items.size() > 0) {
			}
		}
	}
}

bool phdPopupMenu::getVisible() {
	return visible;
}

float phdPopupMenu::getWidth() { // get max item width
	float _w = 0;
	for(int i = 0; i < items.size(); i++) {
		float _iw = (items[i]->name.size() + 1) * menuItemCharWidth;
		if(items[i]->subMenu != NULL) _iw += menuItemCharWidth * 2;
		if(_iw > _w) _w = _iw;
	}
	return _w;
}

float phdPopupMenu::getHeight() { // get a col height ==== menu height
	//return items.size() * menuItemHeight;
	return nRows * menuItemHeight;
}

void phdPopupMenu::setPosition(float _x, float _y) {
	x = _x; y = _y;
}

void phdPopupMenu::update() {
	if(getVisible()) updateHitTestInfo(ofGetMouseX(), ofGetMouseY());
}

void phdPopupMenu::draw() {
	ofPushStyle();
	if(visible && items.size() > 0) {
		for(int i = 0; i < nCols; i++) {
			drawFilledBorderRectangle(x + (i*colWidth), y, colWidth, menuHeight, ofColor(128,64,255,128), ofColor(230,198,255,220));
		}

		for(int i = 0; i < items.size(); i++) {

			int _c = (int)i / nRows;
			int _r = (int)i % nRows;

			float _itemX = x + ((int)(i / nRows) * colWidth);
			float _itemY = y + ((i % nRows) * menuItemHeight);
			phdMenuItem * _item = (phdMenuItem*) hitInfo.itemPtr;

			// group division
			if(i < items.size()-1 && items[i]->group != items[i+1]->group) {
				ofSetColor(230,198,255,220);
				ofLine(_itemX, _itemY + menuItemHeight, _itemX + colWidth, _itemY + menuItemHeight);
			}

			if(_item == items[i]) {
				if(items[i]->selected) {
					drawFilledBorderRectangle(_itemX, _itemY, colWidth, menuItemHeight, ofColor(128,192,255,128), ofColor(230,198,255,220));
				} else {
					drawFilledBorderRectangle(_itemX, _itemY, colWidth, menuItemHeight, ofColor(128,64,255,128), ofColor(230,198,255,220));
				}
				ofSetColor(255,255,64,255);
			} else {
				if(items[i]->selected) {
					drawFilledBorderRectangle(_itemX, _itemY, colWidth, menuItemHeight, ofColor(128,192,255,128), ofColor(230,198,255,220));
				}
				ofSetColor(255,255,255,255);
			}

			string _str = items[i]->name; if(items[i]->subMenu != NULL) _str += " >";
			ofDrawBitmapString(_str, _itemX+2, _itemY+12);

			if(items[i]->subMenu != NULL) {
				items[i]->subMenu->setPosition(x + colWidth*nCols + 2, _itemY);
				items[i]->subMenu->draw();
			}
		}
	}
	ofPopStyle();
}

bool phdPopupMenu::updateHitTestInfo(float _x, float _y) {

	hitInfo.clear();

	if(!visible) return false;

	// inside this menu?
	if(_x >= x && _x <= x+colWidth*nCols && _y >= y && _y <= y+menuHeight) {

		hitInfo.infoType = phtInside;

		if(items.size() == 0) {

		} else {

			int _r = (int)((_y-y) / menuItemHeight);
			int _c = (int)((_x-x) / colWidth);

			hitInfo.itemIndex =  _c * nRows + _r;

			if(hitInfo.itemIndex > -1 && hitInfo.itemIndex < items.size()) {
				hitInfo.itemPtr = (void*) items[hitInfo.itemIndex];
			} else {
				hitInfo.itemIndex = -1;
				hitInfo.itemPtr = NULL;
			}
		}

	} else {

		hitInfo.infoType = phtOutside;

		// inside submenus?
		for(int i = 0; i < items.size(); i++) {
			if(items[i] != NULL && items[i]->subMenu != NULL) { // has a submenu
				if(items[i]->subMenu->getVisible() && items[i]->subMenu->updateHitTestInfo(_x,_y)) { // if inside submenu

					// pass parent caller info - setted on popup method
					items[i]->subMenu->caller = caller;
					items[i]->subMenu->callerTag = callerTag;
					items[i]->subMenu->owner = owner;

					hitInfo = items[i]->subMenu->hitInfo; // get hitInfo from submenu
				}
			}
		}
	}

	return (hitInfo.infoType != phtOutside);
}

void phdPopupMenu::onMouseMoved(ofMouseEventArgs& args) {

	if(!visible) return;

	updateHitTestInfo(args.x, args.y);

	if(hitInfo.infoType == phtOutside) {
		phdPopupMenu * _popup = NULL;
		_popup = testInsideMeAndMySubMenus(args.x,args.y);
		bool _found = (_popup != NULL);
		if(_found) { // click inside - a sub menu
			if(_popup != NULL) { _popup->onMouseMoved(args); }
		}
	}
}

void phdPopupMenu::onMousePressed(ofMouseEventArgs& args) {

	if(!visible) return;

	updateHitTestInfo(args.x, args.y);

	if(hitInfo.infoType == phtOutside && getVisible()) {
		phdPopupMenu * _popup = NULL;
		_popup = testInsideMeAndMySubMenus(args.x,args.y);
		bool _found = (_popup != NULL);
		setVisible(_found);
		if(!_found) { // click outside - pass menu
			phdPopupMenuEventData _pmed;
			_pmed.item = NULL;
			_pmed.menu = (phdPopupMenu*) this;
			_pmed.eventType = petClickOutside;
			ofNotifyEvent(popEvent, _pmed, this);
		}
	}
}

void phdPopupMenu::onMouseDragged(ofMouseEventArgs& args) {

	if(!visible) return;

	updateHitTestInfo(args.x, args.y);

	if(hitInfo.infoType == phtOutside) {
		phdPopupMenu * _popup = NULL;
		_popup = testInsideMeAndMySubMenus(args.x,args.y);
		bool _found = (_popup != NULL);
		if(_found) { // click inside - a sub menu
			if(_popup != NULL) { _popup->onMouseDragged(args); }
		}
	}
}

void phdPopupMenu::onMouseReleased(ofMouseEventArgs& args) {

	if(!visible) return;

	updateHitTestInfo(args.x, args.y);

	if(hitInfo.infoType == phtInside && hitInfo.itemIndex != -1) {
		if(hitInfo.itemIndex != -1) {
			phdMenuItem * _item = (phdMenuItem*) hitInfo.itemPtr;
			_item->parent->setClickedItem(_item);
		}
	} else if(hitInfo.infoType == phtOutside) {
		phdPopupMenu * _popup = NULL;
		_popup = testInsideMeAndMySubMenus(args.x,args.y);
		bool _found = (_popup != NULL);
		setVisible(_found);
		if(!_found) { // click outside - pass menu
			phdPopupMenuEventData _pmed;
			_pmed.item = NULL;
			_pmed.menu = (phdPopupMenu*) this;
			_pmed.eventType = petClickOutside;
			ofNotifyEvent(popEvent, _pmed, this);
		} else {
			if(_popup != NULL) { _popup->onMouseReleased(args); }
		}
	}
}

phdMenuItem * phdPopupMenu::addItem(phdMenuItem * _item) {
	if(_item != NULL) {
		items.push_back(_item);
		_item->parent = (phdPopupMenu*)this;
		updateMenuDimensions();
	}
	return _item;
}

phdMenuItem * phdPopupMenu::addItem(string _name, int _id, phdPopupMenu * _subMenu, int _group) {
	phdMenuItem * _result = addItem(new phdMenuItem(_name, _id, (phdPopupMenu*)this, _subMenu, _group));
	return _result;
}

void phdPopupMenu::setClickedItem(phdMenuItem * _item) {

	// hides all submenus from items of this menu
	for(int i = 0; i < items.size(); i++) {
		if(items[1]->group == -1) items[i]->selected = false;
		if(items[i]->subMenu != NULL) items[i]->subMenu->setVisible(false);
	}

	if(_item != NULL) {
		if(_item->subMenu != NULL) { // if has a submenu

			phdPopupMenuEventData _pmed;
			_pmed.menu = (phdPopupMenu*) this;
			_pmed.item = _item;
			_pmed.eventType = petPopupSubMenu;
			ofNotifyEvent(popEvent, _pmed, this);

			_item->subMenu->setParent(_item->parent);
			_item->subMenu->setVisible(!_item->subMenu->visible); // shows or hides it

		} else {
			
			// if belongs to a group -> sinalize it
			if(_item->group != -1 && _item->parent != NULL) {
				for(int i = 0; i < _item->parent->size(); i++) {
					if((*_item->parent)[i]->group == _item->group) {
						(*_item->parent)[i]->selected = false; // unselect every item of this group
					}
				}
				_item->selected = true;
			}

			phdPopupMenu * _parent = _item->parent;
			while(_parent != NULL && _parent->parent != NULL) { // get root
				_parent = _parent->parent;
			}

			if(_parent != NULL) {
				_parent->setVisible(false); // hides all
			}

			phdPopupMenuEventData _pmed;
			_pmed.menu = (phdPopupMenu*) this;
			_pmed.item = _item;
			_pmed.eventType = petClickOnItem;
			ofNotifyEvent(popEvent, _pmed, this);
		}
	}
}

phdPopupMenu * phdPopupMenu::testInsideMeAndMySubMenus(float _x, float _y) {

	bool _result = (_x >= x && _x <= x+colWidth*nCols && _y >= y && _y <= y+menuHeight);

	if(_result) {
		return this;
	}

	for(int i = 0; i < items.size(); i++) {
		if(items[i]->subMenu != NULL && items[i]->subMenu->getVisible()) {
			return items[i]->subMenu->testInsideMeAndMySubMenus(_x,_y);
		}
	}

	return NULL;
}

bool phdPopupMenu::testInsideThis(float _x, float _y) {
	return !(_x < x || _x > x + getWidth() || _y < y || _y > y + getHeight());
}

void phdPopupMenu::updateMenuDimensions() {

	nCols = 1 + (int)(items.size() / menuMaxItemsPerCol); // 15 max in a col

	nRows = (int)(items.size() / nCols);
	if(items.size() % nCols != 0) nRows += 1;

	colWidth = getWidth();
	menuHeight = getHeight();  // actualize width / height
}

void phdPopupMenu::popup(float _x, float _y, void * _caller, string _callerTag) {
	if(!getVisible()) {
		caller = (void*) _caller;
		callerTag = _callerTag;
		setPosition(_x, _y);
		setVisible(true);
	}
}

//--------------------------------------------------------------------------------------------------------------
phdPopupsManager::phdPopupsManager() {

}

phdPopupsManager::~phdPopupsManager() {
	freeItems();
}

void phdPopupsManager::freeItems() {
	for (vector<phdPopupMenu*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	items.clear();
}

void phdPopupsManager::clear() {
	items.clear();
}

int phdPopupsManager::countVisibles() {
	int _result = 0;
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->getVisible()) _result += 1;
	}
	return _result;
}

int phdPopupsManager::indexOfPopupUnderMouse(float _x, float _y) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->getVisible() && items[i]->testInsideThis(_x,_y)) return i;
	}
	return -1;
}

void phdPopupsManager::hidesAll() {
	for(int i = 0; i < items.size(); i++) {	items[i]->setVisible(false); }
}

int phdPopupsManager::indexOf(phdPopupMenu * _item) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i] == _item) return i;
	}
	return -1;
}

int phdPopupsManager::indexOf(string _name) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->getName() == _name) return i;
	}
	return -1;
}

phdPopupMenu * phdPopupsManager::findByName(string _name) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->getName() == _name) return items[i];
	}
	return NULL;
}

phdPopupMenu * phdPopupsManager::addMenu(string _name, phdPopupMenu * _parent) {
	int _index = indexOf(_name);
	if(_index == -1) {
		items.push_back(new phdPopupMenu(_parent));
		items[items.size()-1]->setName(_name);
		return items[items.size()-1];
	}
	return items[_index];
}

void phdPopupsManager::delMenu(string _name) {
	int _index = indexOf(_name);
	if(_index > -1 && _index < items.size()) {
		delete items[_index];
		items.erase(items.begin() + _index);
	}
}

void phdPopupsManager::delMenu(phdPopupMenu * _menu) {
	int _index = indexOf(_menu);
	if(_index > -1 && _index < items.size()) {
		delete items[_index];
		items.erase(items.begin() + _index);
	}
}

void phdPopupsManager::update() {
	for(int i = 0; i < items.size(); i++) items[i]->update();
}

void phdPopupsManager::draw() {
	for(int i = 0; i < items.size(); i++) items[i]->draw();
}

void phdPopupsManager::onMouseMoved(ofMouseEventArgs& args) {
	for(int i = 0; i < items.size(); i++) items[i]->onMouseMoved(args);
}

void phdPopupsManager::onMousePressed(ofMouseEventArgs& args) {
	for(int i = 0; i < items.size(); i++) items[i]->onMousePressed(args);
}

void phdPopupsManager::onMouseDragged(ofMouseEventArgs& args) {
	for(int i = 0; i < items.size(); i++) items[i]->onMouseDragged(args);
}

void phdPopupsManager::onMouseReleased(ofMouseEventArgs& args) {
	for(int i = 0; i < items.size(); i++) items[i]->onMouseReleased(args);
}