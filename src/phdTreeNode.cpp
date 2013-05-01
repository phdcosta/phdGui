#include "phdTreeNode.h"

//--------------------------------------------------------------------------------------------------------------
phdTreeNode::phdTreeNode(string _caption, phdTreeNodeData * _nodedata) {
	parent = NULL;
	firstChild = NULL;
	lastChild = NULL;
	nextSibling = NULL;
	prevSibling = NULL;

	caption = _caption;
	nodeData = _nodedata;
}

phdTreeNode::~phdTreeNode(){
	caption = "";
	clear();
	unlink();
}

//--------------------------------------------------------------------------------------------------------------
int phdTreeNode::getLevel(){
	int nodeCount = -1;
	phdTreeNode * curNode = this;
	while(curNode != NULL) {
		nodeCount += 1;
		curNode = curNode->parent;
	}
	return nodeCount;
}

int phdTreeNode::getChildCount(){
	phdTreeNode * curNode = firstChild;
	int nodeCount = 0;
	while(curNode != NULL) {
		nodeCount += 1;
		curNode = curNode->nextSibling;
	}
	return nodeCount;
}

phdTreeNode * phdTreeNode::getChildByIndex(int _index){
	int _curIndex = 0;
	phdTreeNode * _node = getFirstChild();
	while(_node != NULL) {
		if(_curIndex == _index) return _node;
		_node = _node->getNextSibling();
		_curIndex += 1;
	}
	return _node;
}

phdTreeNode * phdTreeNode::findByCaption(string _caption) {
	phdTreeNode * _node = getFirstChild();
	while(_node != NULL) {
		if(_node->caption == _caption) return _node;
		phdTreeNode * _result = _node->findByCaption(_caption);
		if(_result != NULL) return _result;
		_node = _node->getNextSibling();
	}
	return NULL;
}

void phdTreeNode::unlink(){
	if(parent != NULL) {
		if(parent->firstChild == this) parent->firstChild = nextSibling;
		if(parent->lastChild == this) parent->lastChild = prevSibling;
	}

	if(prevSibling != NULL) prevSibling->nextSibling = nextSibling;
	if(nextSibling != NULL) nextSibling->prevSibling = prevSibling;

	prevSibling = NULL;
	nextSibling = NULL;

	parent = NULL;
}

void phdTreeNode::linkAs(phdTreeNode * _parent, phdNodePositionMode _mode){
	if(_parent != NULL) {
		unlink();
		if(_mode == npmFirstChild) {
			parent = _parent;
			if(parent->firstChild != NULL) {
				nextSibling = parent->firstChild;
				parent->firstChild->prevSibling = this;
			} else {
				parent->lastChild = this;
			}
			parent->firstChild = this;
		} else if(_mode == npmLastChild) {
			parent = _parent;
			if(parent->lastChild != NULL) {
				prevSibling = parent->lastChild;
				parent->lastChild->nextSibling = this;
			} else {
				parent->firstChild = this;
			}
			parent->lastChild = this;
		} else if(_mode == npmPrevSibling) {
			parent = _parent->parent;
			nextSibling = _parent;
			prevSibling = _parent->prevSibling;
			_parent->prevSibling = this;
			if(prevSibling != NULL) prevSibling->nextSibling = this;
			if(parent->firstChild == _parent) parent->firstChild = this;
		} else if(_mode == npmNextSibling) {
			parent = _parent->parent;
			nextSibling = _parent->nextSibling;
			prevSibling = _parent;
			_parent->nextSibling = this;
			if(nextSibling != NULL) nextSibling->prevSibling = this;
			if(_parent->lastChild == _parent) _parent->lastChild = this;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------

void phdTreeNode::clear(){

	phdTreeNode * nodeA = firstChild;

	while(nodeA != NULL) {
		phdTreeNode * nodeB = nodeA->nextSibling;
		delete nodeA;
		nodeA = nodeB;
	}
}

int phdTreeNode::indexOfChildren(phdTreeNode * _node) {

	int curIndex = 0;
	phdTreeNode * curNode = firstChild;

	while(curNode != NULL) {
		if(curNode == _node) return curIndex;
		curNode = curNode->nextSibling;
		curIndex += 1;
	}
	return -1;
}

phdTreeNode * phdTreeNode::addNode(phdNodePositionMode _mode, phdTreeNode * _node, phdTreeNodeData * _nodedata) {
	if(_node != NULL) {
		_node->nodeData = _nodedata;
		_node->linkAs(this, _mode);
		return _node;
	}
	return NULL;
}

phdTreeNode *  phdTreeNode::addFirstChild(phdTreeNode * _node, phdTreeNodeData * _nodedata) {
	return addNode(npmFirstChild, _node, _nodedata);
}

phdTreeNode *  phdTreeNode::addLastChild(phdTreeNode * _node, phdTreeNodeData * _nodedata) {
	return addNode(npmLastChild, _node, _nodedata);
}

phdTreeNode *  phdTreeNode::addPrevSibling(phdTreeNode * _node, phdTreeNodeData * _nodedata) {
	return addNode(npmPrevSibling, _node, _nodedata);
}

phdTreeNode *  phdTreeNode::addNextSibling(phdTreeNode * _node, phdTreeNodeData * _nodedata) {
	return addNode(npmNextSibling, _node, _nodedata);
}

void  phdTreeNode::delChild(phdTreeNode * _node) {
	if(indexOfChildren(_node) != -1) {
		_node->unlink();
		delete _node;
	}
}

void phdTreeNode::delChild(int _index) {
	phdTreeNode * _node = getChildByIndex(_index);
	if(_node != NULL) {
		_node->unlink();
		delete _node;
	}
}

//--------------------------------------------------------------------------------------------------------------

string phdTreeNode::traceChildren(phdTreeNode * _node) {

	string result = "";

	if(_node != NULL) {

		int _l = _node->getLevel(); for(int i = 0; i < _l; i++) result += "+";

		result += _node->caption + "\n";

		phdTreeNode * curNode = _node->firstChild;
		while(curNode != NULL) {
			result += traceChildren(curNode);
			curNode = curNode->nextSibling;
		}
	}
	return result;
}
