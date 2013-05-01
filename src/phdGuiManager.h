#pragma once

#include "ofMain.h"

#include "phdTreeNode.h"
#include "phdPopupMenu.h"

//--------------------------------------------------------------------------------------------------------------
class phdGuiEventData;
class phdGuiControl;
class phdGuiManager;
class phdGuiInputField;

//--------------------------------------------------------------------------------------------------------------
bool compareMouseEventArgs(ofMouseEventArgs & argsA, ofMouseEventArgs & argsB);

//--------------------------------------------------------------------------------------------------------------
enum phdGuiControlAlignmentMode { gamSize, gamFitWidth, gamFitHeight, gamRight, gamLeft, gamTop, gamBottom, gamClient };

//--------------------------------------------------------------------------------------------------------------
enum phdGuiEventType { petOnClick, petOnDrag, petOnChangeValue, petOnChangeString };

//--------------------------------------------------------------------------------------------------------------
enum phdColorModeType { pcmtNormal, pcmtSelected, pcmtFocused };

//----------------------------------------------------------------------------------------------------------------------
enum phdEditFieldType { pftString, pftFloat, pftInteger };

//----------------------------------------------------------------------------------------------------------------------
enum phdEditFieldDisableMode { pefmEnterKey, pefmEscapeKey, pefmClickOut };

//----------------------------------------------------------------------------------------------------------------------
class phdGuiInputField {
protected:
	float x, y, w, h;
	bool editing;

	phdEditFieldType fieldType;

	int holdIntData;
	float holdFloatData;
	string holdStringData;

	int cursorPos, maxLength;

	int * resultIntData;
	float * resultFloatData;
	string * resultStringData;

	phdGuiControl * ownerControl;
	int ownerIndex;

	bool usingEvents;

	ofColor colors[3]; // fill, border e text

public:
	ofEvent<phdGuiEventData> guiEvent;

	phdGuiInputField();
	~phdGuiInputField();

	void setupColors(ofColor _fill, ofColor _border, ofColor _text) {
		colors[0] = _fill;		colors[0].a = 255;
		colors[1] = _border;	colors[1].a = 255;
		colors[2] = _text;		colors[2].a = 255;
	}

	bool isEditing() { return editing; }

	bool testInside(float _x, float _y);
	void disable(phdEditFieldDisableMode _mode);
	inline void setCursorPos(int _pos);
	void processKey(int _key);

	void editInteger(float _x, float _y, float _w, float _h, int & _result, int _length = 8);
	void editFloat(float _x, float _y, float _w, float _h, float & _result, int _length = 8, int _precision = 4);
	void editString(float _x, float _y, float _w, float _h, string & _result, int _length = 20);

	void editInteger(float _x, float _y, float _w, float _h, phdGuiControl * _gc, int _index = 0, int _length = 8);
	void editFloat(float _x, float _y, float _w, float _h, phdGuiControl * _gc, int _index = 0, int _length = 8, int _precision = 4);
	void editString(float _x, float _y, float _w, float _h, phdGuiControl * _gc, int _index = 0, int _length = 20);

	void draw() { draw(x, y, w, h); }
	void draw(float _x, float _y, float _w, float _h);

	void keyPressed(ofKeyEventArgs & args);

	void enableEvents();
	void disableEvents();
};

//--------------------------------------------------------------------------------------------------------------
class phdGuiEventData {
public:
	phdGuiControl * control;
	phdGuiEventType eventType;
	string tag;
	phdGuiEventData();
	phdGuiEventData(phdGuiEventType _eventType, phdGuiControl * _control, string _tag = "");
};

//--------------------------------------------------------------------------------------------------------------
class phdGuiControl : public phdTreeNode {
protected:
	phdGuiManager * manager;
	phdPopupMenu * popupMenu;

	ofMouseEventArgs argsPress;
	ofMouseEventArgs argsDrag;

	float x, y, w, h;
	int id, group;
	phdGuiControlAlignmentMode align;
	bool visible;
	bool border;

public:
	phdHitTestInfo hitInfo;

	ofEvent<phdGuiEventData> guiEvent;

	phdGuiControl(string _caption = "control", phdTreeNodeData * _nodedata = NULL);
	~phdGuiControl();

	inline virtual string getClassName() { return "PHDGUICONTROL"; }

	phdGuiControl * getHigherParent();

	phdGuiManager * getManager();
	void setManager(phdGuiManager * _manager);

	virtual void setPopupMenu(phdPopupMenu * _menu);
	virtual phdPopupMenu * getPopupMenu();

	inline virtual phdGuiControl * getParent() { return (phdGuiControl*) parent; }

	inline virtual phdGuiControl * getFirstChild() { return (phdGuiControl*) firstChild; }
	inline virtual phdGuiControl * getLastChild() { return (phdGuiControl*) lastChild; }

	inline virtual phdGuiControl * getPrevSibling() { return (phdGuiControl*) prevSibling; }
	inline virtual phdGuiControl * getNextSibling() { return (phdGuiControl*) nextSibling; }

	inline virtual phdGuiControl * createInstance() { return new phdGuiControl(); }

	inline virtual bool isContainer() { return false; }

	// use these methods to control internal values
	inline virtual float getValue(int _index = 0) { return 0.0; }
	inline virtual void setValue(float _value = 0.0, int _index = 0) { }
	inline virtual int getCount() { return 0; }

	// use these methods to control internal strings
	inline virtual string getString(int _index = 0) { return ""; }
	inline virtual void setString(string _value = "", int _index = 0) { }
	inline virtual int getStringCount() { return 0; }

	inline virtual void setVisible(bool _value) { visible = _value; if(getParent() != NULL) getParent()->alignChildren(); }
	inline virtual bool isVisible() { return visible; }

	inline virtual void setID(int _value) { id = _value; }
	inline virtual int getID() { return id; }

	inline virtual void setGroup(int _value) { group = _value; }
	inline virtual int getGroup() { return group; }

	inline virtual void setAlign(phdGuiControlAlignmentMode _value) { align = _value; if(getParent() != NULL) getParent()->alignChildren(); }
	inline virtual phdGuiControlAlignmentMode getAlign() { return align; }

	inline float getScreenX();
	inline float getScreenY();

	inline float getX() { return x; }
	inline void setX(float _value) { x = _value; }

	inline float getY() { return y; }
	inline void sety(float _value) { y = _value; }

	inline virtual float getWidth() { return w; }
	inline virtual void setWidth(float _value);

	inline virtual float getHeight() { return h; }
	inline virtual void setHeight(float _value);

	inline void setBorder(bool _value) { border = _value; }
	inline bool hasBorder() { return border; }

	void setPosition(float _x, float _y);
	void setDimension(float _w, float _h, bool _normalized = false);
	void setRectArea(float _x, float _y, float _w, float _h, bool _check = true);

	void alignChildren();

	virtual bool isInside(float _x, float _y);
	virtual bool testBounds(float _x, float _y);
	virtual phdGuiControl * hitTest(float _x, float _y);
	virtual phdGuiControl * findById(int _id);

	virtual void update();
	virtual void updateChildren();

	virtual void draw(float _x, float _y);
	virtual void drawChildren(float _x, float _y, bool _firstToLast = true);

	virtual bool updateHitTestInfo(float _x, float _y);

	virtual void onMouseMoved(ofMouseEventArgs & args);
	virtual void onMousePressed(ofMouseEventArgs & args);
	virtual void onMouseDragged(ofMouseEventArgs & args);
	virtual void onMouseReleased(ofMouseEventArgs & args);
	virtual void onKeyPressed(ofKeyEventArgs & args);

	virtual void onGuiEvent(phdGuiEventData &_eventData);
};

//--------------------------------------------------------------------------------------------------------------
class phdGuiManager {
protected:
	bool usingEvents;

	phdGuiControl root;

	phdGuiControl * currentControl;
	phdGuiControl * focusedControl;
	phdGuiControl * selectedControl;

	ofMouseEventArgs argsPress;
	ofMouseEventArgs argsDrag;

	float controlGap;

	ofColor textColors[3];
	ofColor borderColors[3];
	ofColor fillColors[3];

	phdGuiInputField inputField;

public:
	phdGuiManager();
	~phdGuiManager();

	inline virtual string getClassName() { return "PHDGUIMANAGER"; }

	phdGuiControl * getRoot() { return &root; }

	phdPopupMenu * getPopupMenu() { return root.getPopupMenu(); }
	void setPopupMenu(phdPopupMenu * _menu) { root.setPopupMenu(_menu); }

	phdGuiControl & phdGuiManager::operator[](string _caption) { return findByCaption(_caption); }
	phdGuiControl & findByCaption(string _caption);

	phdGuiControl & phdGuiManager::operator[](int _id) { return findById(_id); }
	phdGuiControl & findById(int _id);

	inline phdGuiControl * getFocused() { return focusedControl; }
	inline void setFocused(phdGuiControl * _control) { focusedControl = _control; }

	inline phdGuiControl * getSelected() { return selectedControl; }
	inline void setSelected(phdGuiControl * _control);

	inline phdGuiControl * getcurrent() { return currentControl; }
	inline void setCurrent(phdGuiControl * _control) { currentControl = _control; }

	inline phdGuiInputField & getInputField() { return inputField; }

	ofColor & getTextColor(phdColorModeType _colorMode) { return textColors[_colorMode]; }
	ofColor & getBorderColor(phdColorModeType _colorMode) { return borderColors[_colorMode]; }
	ofColor & getFillColor(phdColorModeType _colorMode) { return fillColors[_colorMode]; }

	ofColor & getFillColor(phdGuiControl * _control) {
		if(getFocused() == _control) return fillColors[pcmtFocused];
		if(getSelected() == _control) return fillColors[pcmtSelected];
		return fillColors[pcmtNormal];
	}

	ofColor & getTextColor(phdGuiControl * _control) {
		if(getFocused() == _control) return textColors[pcmtFocused];
		if(getSelected() == _control) return textColors[pcmtSelected];
		return textColors[pcmtNormal];
	}

	ofColor & getBorderColor(phdGuiControl * _control) {
		if(getFocused() == _control) return borderColors[pcmtFocused];
		if(getSelected() == _control) return borderColors[pcmtSelected];
		return borderColors[pcmtNormal];
	}

	phdGuiControl * addControl(phdGuiControl * _newControl = NULL);
	void goUp() { if(currentControl != NULL) { setCurrent(currentControl->getParent()); } }

	void delNode(string _caption);
	void delNode(int _id);

	phdGuiControl & control(string _caption = "");

	void setSize(float _w, float _h) { root.setRectArea(0,0,_w,_h); }

	void setGap(float _value) { controlGap = _value; }
	float getGap() { return controlGap; }

	void update();
	void draw();

	void mouseMoved(ofMouseEventArgs & args);
	void mousePressed(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	void mouseReleased(ofMouseEventArgs & args);

	void keyPressed(ofKeyEventArgs & args);

	void enableEvents();
	void disableEvents();
};