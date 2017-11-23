#include "ObstacleNode.h"

using namespace GinTama;

ObstacleNode::ObstacleNode()
{
	//CAN BE ANYTH, JUST SET IT DIFFERENT FROM PLAYER
	setTag(2);
}

ObstacleNode::~ObstacleNode()
{

}

ObstacleNode *ObstacleNode::create(
	const std::string& _spriteFileName, 
	const cocos2d::Vec2& _obsPos, 
	const std::function<bool(cocos2d::PhysicsContact& _contact)>& _checkCollision,
	const float& _obsScale)
{
	ObstacleNode *zeNode = new ObstacleNode(_spriteFileName, _obsPos, _checkCollision, _obsScale);
	return zeNode;
}

//IDK IF THIS IS A GOOD WAY or CHECKING BY TAG IS A BETTER WAY...
//THIS IS WHAT I UNDERSTOOD FROM THE DOCUMENTATION
bool ObstacleNode::onContactBegin(const cocos2d::PhysicsBody& _body1, const cocos2d::PhysicsBody& _body2)
{
	//I TRIED NOT TO USE getShapeA() / getShapeB cuz it made no sense to me... Sry
	//SO INSTEAD, JUS GET NODE OF BODY DIRECTLY
	auto nodeA = _body1.getNode();
	auto nodeB = _body2.getNode();

	//CHECK IF THIS NODE IS TAGGED PLAYER
	if (nodeA->getTag() == 1)
	{
		cocos2d::log("nodeA is Player Physics Object");
	}
	else if (nodeB->getTag() == 1)
	{
		cocos2d::log("nodeB is Player Physics Object");
	}

	return true;
}
