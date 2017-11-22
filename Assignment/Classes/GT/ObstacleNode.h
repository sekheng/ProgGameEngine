#pragma once

#include "cocos2d.h"

namespace GinTama
{
	class ObstacleNode : public cocos2d::Node
	{
	public:
		static ObstacleNode* create(const std::string& _spriteFileName, const cocos2d::Vec2& _obsPos, const float& _obsScale = 1);
		void setObstacleRect();
		cocos2d::Rect getObstacleRect();

		bool OnCollisionEnter(const cocos2d::Node& _otherNode);


	protected:
		ObstacleNode();
		ObstacleNode( const std::string& _spriteFileName, const cocos2d::Vec2& _obsPos, const float& _obsScale = 1)
		{
			obsSprite = cocos2d::Sprite::create(_spriteFileName);
			spriteName = _spriteFileName;
			obsPos = _obsPos;
			obsScale = _obsScale;
			setTag(2);
		}
		virtual ~ObstacleNode();

	private:
		cocos2d::Sprite* obsSprite;
		std::string spriteName;
		cocos2d::Vec2 obsPos;
		float obsScale;

		cocos2d::Rect obsRect;
		//int m_health;
	};
};
