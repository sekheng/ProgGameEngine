#include "ObstacleNode.h"

NS_GT_BEGIN

GTObstacleNode::GTObstacleNode()
{
	//CAN BE ANYTH, JUST SET IT DIFFERENT FROM PLAYER
	setTag(2);
}

GTObstacleNode::~GTObstacleNode()
{

}

GTObstacleNode *GTObstacleNode::create(
	const std::string& _spriteFileName, 
	const cocos2d::Vec2& _obsPos, 
	const std::function<bool(cocos2d::PhysicsContact& _contact)>& _checkCollision,
	const float& _obsScale)
{
	GTObstacleNode *zeNode = new GTObstacleNode(_spriteFileName, _obsPos, _checkCollision, _obsScale);
	return zeNode;
}

//IDK IF THIS IS A GOOD WAY or CHECKING BY TAG IS A BETTER WAY...
//THIS IS WHAT I UNDERSTOOD FROM THE DOCUMENTATION
bool GTObstacleNode::onContactBegin(cocos2d::PhysicsContact& _contact)
{
	//I TRIED NOT TO USE getShapeA() / getShapeB cuz it made no sense to me... Sry
	//SO INSTEAD, JUS GET NODE OF BODY DIRECTLY
	auto nodeA = _contact.getShapeA()->getBody()->getNode();
	auto nodeB = _contact.getShapeB()->getBody()->getNode();

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

NS_GT_END