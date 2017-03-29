#include "Node.h"
#include <d3dx9.h>

Node::Node(){}

using namespace std;

void Node::getChild(string name, Entity3D& child){
	std::vector<Entity3D*>::iterator it;
	for (it = _childs.begin(); it != _childs.end(); it++){
		if (name == (*it)->getName())
		{
		child = *(*it);
		}
		else
		{
		(*it)->getChild(name, child);
		}
	}
}
void Node::updateBV(){
	std::vector<Entity3D*>::iterator it;

	for (it = _childs.begin(); it != _childs.end(); it++){

		(*it)->updateBV();

		if (it == _childs.begin())
		{
			BV = (*it)->BV;
		}

		else{

			if (BV.ActualxMax < (*it)->BV.ActualxMax) BV.ActualxMax = (*it)->BV.ActualxMax;
			if (BV.ActualxMin > (*it)->BV.ActualxMin) BV.ActualxMin = (*it)->BV.ActualxMin;

			if (BV.ActualyMax < (*it)->BV.ActualyMax) BV.ActualyMax = (*it)->BV.ActualyMax;
			if (BV.ActualyMin > (*it)->BV.ActualyMin) BV.ActualyMin = (*it)->BV.ActualyMin;

			if (BV.ActualzMax < (*it)->BV.ActualzMax) BV.ActualzMax = (*it)->BV.ActualzMax;
			if (BV.ActualzMin > (*it)->BV.ActualzMin) BV.ActualzMin = (*it)->BV.ActualzMin;
		}
	}

	BV.pivot.x = BV.ActualxMax - ((BV.ActualxMax - BV.ActualxMin) / 2);
	BV.pivot.y = BV.ActualyMax - ((BV.ActualyMax - BV.ActualyMin) / 2);
	BV.pivot.z = BV.ActualzMax - ((BV.ActualzMax - BV.ActualzMin) / 2);

	BV.width = BV.ActualxMax - BV.ActualxMin;
	BV.height = BV.ActualyMax - BV.ActualyMin;
	BV.depth = BV.ActualzMax - BV.ActualzMin;
}

void Node::draw(Renderer& rkRenderer, CollisionResult eParentResult, Frustum& rkFrustum){

	updateWordTransformation();
	std::vector<Entity3D*>::iterator it;
	if (eParentResult != AllOutside)
	{
		if (eParentResult == AllInside)
		{
			for (it = _childs.begin(); it != _childs.end(); it++){
				(*it)->draw(rkRenderer, eParentResult, rkFrustum);
			}
		}
		else
		{
			if (rkFrustum.CheckCollision(BV) != AllOutside)
			{
				for (it = _childs.begin(); it != _childs.end(); it++){
					(*it)->draw(rkRenderer, rkFrustum.CheckCollision(BV), rkFrustum);
				}
			}
		}
	}
}

void Node::draw(Renderer& rkRenderer, CollisionResult eParentResult, Frustum& rkFrustum, Text& _text){

	updateWordTransformation();

	std::vector<Entity3D*>::iterator it;
	if (eParentResult != AllOutside)
	{
		if (eParentResult == AllInside)
		{
			_text.setText(_text._text + "\n" + getName() + " (AllInside) ");
			for (it = _childs.begin(); it != _childs.end(); it++){
				(*it)->draw(rkRenderer, eParentResult, rkFrustum, _text);
			}
		}
		else
		{
			if (rkFrustum.CheckCollision(BV) != AllOutside)
			{
				_text.setText(_text._text + "\n" + getName() + " (Partially) ");
				for (it = _childs.begin(); it != _childs.end(); it++){
					(*it)->draw(rkRenderer, rkFrustum.CheckCollision(BV), rkFrustum, _text);
				}
			}
		}
	}
}

void Node::AddChild(Entity3D* child){
	child->setParent(*this);
	_childs.push_back(child);
}

void Node::updateWordTransformation(){
	Entity3D::updateWordTransformation();

	std::vector<Entity3D*>::iterator it;
	for (it = _childs.begin(); it != _childs.end(); it++){
		(*it)->updateWordTransformation();
	}
}

void Node::removeChild(Entity3D* child){
	std::vector<Entity3D*>::iterator it;
	it = std::find(_childs.begin(), _childs.end(), child);

	if (it != _childs.end()){
		child->_parent = NULL;
		_childs.erase(it);
	}
}