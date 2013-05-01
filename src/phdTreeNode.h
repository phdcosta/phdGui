#pragma once

#include "ofMain.h"

//--------------------------------------------------------------------------------------------------------------
class phdTreeNode;

//--------------------------------------------------------------------------------------------------------------
enum phdNodePositionMode { npmFirstChild, npmLastChild, npmPrevSibling, npmNextSibling };

//--------------------------------------------------------------------------------------------------------------
class phdTreeNodeData {
protected:
public:
};

//--------------------------------------------------------------------------------------------------------------
class phdTreeNode {
protected:
	phdTreeNode * parent;
	phdTreeNode * firstChild;
	phdTreeNode * lastChild;
	phdTreeNode * prevSibling;
	phdTreeNode * nextSibling;

	phdTreeNodeData * nodeData;

	int getChildCount();
	phdTreeNode * getChildByIndex(int _index);
	int getLevel();

	void unlink();

public:
	string caption;

	phdTreeNode(string _caption = "default", phdTreeNodeData * _nodedata = NULL);
	~phdTreeNode();

	int phdTreeNode::size() { return getChildCount(); }
	phdTreeNode * phdTreeNode::operator[](unsigned i) { return getChildByIndex(i); }

	void clear();

	inline virtual string getClassName() { return "PHDTREENODE"; }
	inline virtual phdTreeNode * createInstance() { return new phdTreeNode(); }

	void linkAs(phdTreeNode * _parent, phdNodePositionMode _mode);

	inline virtual phdTreeNode * getParent() { return parent; }

	inline virtual phdTreeNode * getFirstChild() { return firstChild; }
	inline virtual phdTreeNode * getLastChild() { return lastChild; }

	inline virtual phdTreeNode * getPrevSibling() { return prevSibling; }
	inline virtual phdTreeNode * getNextSibling() { return nextSibling; }

	int indexOfChildren(phdTreeNode * _node);

	phdTreeNode * findByCaption(string _caption);

	phdTreeNode * addNode(phdNodePositionMode _mode, phdTreeNode * _node, phdTreeNodeData * _nodedata = NULL);

	phdTreeNode * addFirstChild(phdTreeNode * _node, phdTreeNodeData * _nodedata = NULL);
	phdTreeNode * addLastChild(phdTreeNode * _node, phdTreeNodeData * _nodedata = NULL);

	phdTreeNode * addPrevSibling(phdTreeNode * _node, phdTreeNodeData * _nodedata = NULL);
	phdTreeNode * addNextSibling(phdTreeNode * _node, phdTreeNodeData * _nodedata = NULL);

	void delChild(phdTreeNode * _node);
	void delChild(int _index);

	string traceChildren(phdTreeNode * _node);
};

