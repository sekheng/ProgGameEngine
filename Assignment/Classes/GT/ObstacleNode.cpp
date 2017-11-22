#include "ObstacleNode.h"

using namespace GinTama;

ObstacleNode::ObstacleNode()
{
	setTag(2);
}

ObstacleNode::~ObstacleNode()
{

}

ObstacleNode *ObstacleNode::create(const std::string& _spriteFileName, const cocos2d::Vec2& _obsPos, const float& _obsScale)
{
	ObstacleNode *zeNode = new ObstacleNode(_spriteFileName, _obsPos, _obsScale);
	return zeNode;
}

void ObstacleNode::setObstacleRect()
{
	obsRect = this->getBoundingBox();
}

cocos2d::Rect ObstacleNode::getObstacleRect()
{
	return obsRect;
}

bool ObstacleNode::OnCollisionEnter(const cocos2d::Node& _otherNode)
{
	if (this->getObstacleRect().intersectsRect(_otherNode.getBoundingBox()))
	{
		cocos2d::log("COLLIDED");
		return true;
	}
	else
	{
		cocos2d::log("NOT COLLIDED");
		return false;
	}
}