#pragma once

#include "ofMain.h"

#include "phdDefinitions.h"
#include "phdUtils.h"

//--------------------------------------------------------------------------------------------------------------
class phdMenuItem;
class phdPopupMenu;

//--------------------------------------------------------------------------------------------------------------
enum phdPopupEventType { petClickOnItem, petClickOutside, petPopupSubMenu, petSetItemsBeforePopup };

//--------------------------------------------------------------------------------------------------------------
class phdPopupMenuEventData {
public:
	phdMenuItem * item;
	phdPopupMenu * menu;
	phdPopupEventType eventType;
};

static const int menuItemHeight = 18;
static const int menuItemCharWidth = 8;
static const int menuMaxItemsPerCol = 25;

//--------------------------------------------------------------------------------------------------------------
class phdMenuItem {
public:
	string name;
	int id;
	bool selected;
	int group;
	phdPopupMenu * subMenu;
	phdPopupMenu * parent;
	phdMenuItem();
	phdMenuItem(string _name, int _id, phdPopupMenu * _parent, phdPopupMenu * _subMenu, int _group);
	~phdMenuItem();
	string getFullPath();
	int getLevel();
	string getFullCallerTag();
};

//--------------------------------------------------------------------------------------------------------------
class phdPopupMenu {
protected:
	int nRows, nCols;
	bool visible;
	float x, y;
	float colWidth, menuHeight;
	vector<phdMenuItem*> items;

	void * owner;
	void * caller;
	string callerTag;
	phdPopupMenu * parent;
	string name;

	void updateMenuDimensions();
	void setClickedItem(phdMenuItem * _item);

public:
	phdHitTestInfo hitInfo;

	ofEvent<phdPopupMenuEventData> popEvent;

	phdPopupMenu(phdPopupMenu * _parent = NULL);
	~phdPopupMenu();

	phdMenuItem * phdPopupMenu::operator[](unsigned i) { return items[i]; }
	inline int size() { return items.size(); }
	inline phdMenuItem * getItem(int _index) { return items[_index]; }

	void freeItems();

	void setOwner(void * _owner) { owner = _owner; }
	void * getOwner() { return owner; }

	void setCaller(void * _caller) { caller = _caller; }
	void * getCaller() { return caller; }

	void setCallerTag(string _callerTag) { callerTag = _callerTag; }
	string getCallerTag() { return callerTag; }

	void setParent(phdPopupMenu * _menu) { parent = _menu; }
	phdPopupMenu * getParent() { return parent; }

	void setName(string _name) { name = _name; }
	string getName() { return name; }

	int indexOf(phdMenuItem * _value);
	int indexByName(string _value);

	phdPopupMenu * getRootMenu();

	void setVisible(bool _value);
	bool getVisible();

	float getX() { return x; }
	float getY() { return y; }

	float getWidth();
	float getHeight();

	void setPosition(float _x, float _y);

	bool updateHitTestInfo(float _x, float _y);

	void update();
	void draw();

	void onMouseMoved(ofMouseEventArgs& args);
	void onMousePressed(ofMouseEventArgs& args);
	void onMouseDragged(ofMouseEventArgs& args);
	void onMouseReleased(ofMouseEventArgs& args);

	phdMenuItem * addItem(phdMenuItem * _item);
	phdMenuItem * addItem(string _name, int _id, phdPopupMenu * _subMenu = NULL, int _group = -1);

	phdPopupMenu * testInsideMeAndMySubMenus(float _x, float _y);

	bool testInsideThis(float _x, float _y);

	void popup(float _x, float _y, void * _caller, string _callerTag);
};

//--------------------------------------------------------------------------------------------------------------
class phdPopupsManager {
protected:
	vector<phdPopupMenu*> items;

public:
	phdPopupsManager();
	~phdPopupsManager();

	phdPopupMenu * phdPopupsManager::operator[](unsigned i) { return items[i]; }
	int phdPopupsManager::size() { return items.size(); }

	void freeItems();
	void clear();

	int countVisibles();
	int indexOfPopupUnderMouse(float _x, float _y);
	void hidesAll();

	int indexOf(phdPopupMenu * _menu);
	int indexOf(string _name);
	phdPopupMenu * findByName(string _name);

	phdPopupMenu * addMenu(string _name, phdPopupMenu * _parent);
	void delMenu(string _name);
	void delMenu(phdPopupMenu * _menu);

	void update();
	void draw();

	void onMouseMoved(ofMouseEventArgs& args);
	void onMousePressed(ofMouseEventArgs& args);
	void onMouseDragged(ofMouseEventArgs& args);
	void onMouseReleased(ofMouseEventArgs& args);
};

//--------------------------------------------------------------------------------------------------------------

phdPopupsManager & popupsManager();

//--------------------------------------------------------------------------------------------------------------
