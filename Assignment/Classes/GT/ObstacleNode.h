#pragma once

#include "cocos2d.h"

namespace GinTama
{
	class ObstacleNode : public cocos2d::Node
	{
	public:
		//TO BE CALLED IN GAMESCENE::INIT or WHEREVER IT IS BEING CREATED
		static ObstacleNode* create(const std::string& _spriteFileName, const cocos2d::Vec2& _obsPos, const std::function<bool(cocos2d::PhysicsContact& _contact)>& _checkCollision, const float& _obsScale = 1);

		bool onContactBegin(const cocos2d::PhysicsBody& _body1, const cocos2d::PhysicsBody& _body2);

	protected:
		ObstacleNode();
		// TO BE CALLED IN CREATE, PREVENTS RETYPING
		// JUST CREATE THIS OBJECT AND ADDCHILD
		ObstacleNode( const std::string& _spriteFileName, const cocos2d::Vec2& _obsPos, const std::function<bool(cocos2d::PhysicsContact& _contact)>& _checkCollision, const float& _obsScale = 1)
		{
			//CREATE OBSTACLE SPRITE
			obsSprite = cocos2d::Sprite::create(_spriteFileName);
			
			//PHYSICS BODY FOR THIS OBSTACLE + EVENTLISTENER
			//TRY NOT TO RETYPE THIS IN INIT
			obsBody = cocos2d::PhysicsBody::createBox(cocos2d::Size(obsSprite->getContentSize().width, obsSprite->getContentSize().height));

			auto contactListener = cocos2d::EventListenerPhysicsContact::create();
			contactListener->onContactBegin = _checkCollision;
			_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

			obsSprite->setPhysicsBody(obsBody);

			//TO BE APPLIED TO THE SPRITE
			obsPos = _obsPos;
			obsSprite->setPosition(obsPos);

			obsScale = _obsScale;
			obsSprite->setScale(_obsScale);

			setTag(2);
		}
		virtual ~ObstacleNode();

	private:
		cocos2d::Vec2 obsPos;
		float obsScale;

		cocos2d::Sprite* obsSprite;
		cocos2d::PhysicsBody* obsBody;
	};
};
