#pragma once

#include "cocos2d.h"

namespace GinTama
{
    enum CHARACTER_STATE {
        RUNNING = 0x0001,
        JUMPING = 0x0002,
        DEAD = 0x0004,
        TOTAL_STATE = RUNNING | JUMPING | DEAD,
    };

	class CharacterStatNode : public cocos2d::Node
	{
	public:
		static CharacterStatNode* create();
        static CharacterStatNode* create(cocos2d::PhysicsBody *_physicsBody);

        /** This will set the health to be the exact health!
        *
        * @param _health will be the exact value of the health
        */
		bool setHealth(const int &_health);
		/** Adjust the value of the health
        
        */
        bool adjustHealth(const int &_value);
        /** Set the speed to the exact
        *
        */
        void setSpeedX(const float &_speed);
        /** Adjust the value of the speed
        *
        */
        void adjustSpeedX(const float &_value);

        float getSpeedX();
        int getHealth();
        virtual void update(float delta);

        void setPhysicsNode(cocos2d::PhysicsBody *_physicsBody);

        bool setState(CHARACTER_STATE _whatState);
        CHARACTER_STATE getCurrentState();

	protected:
		CharacterStatNode();
		virtual ~CharacterStatNode();

		int m_health;
        cocos2d::PhysicsBody* m_physicsNode;
        CHARACTER_STATE m_CurrentState;
        float m_countingFloat;
        float m_SpeedX;
        float m_MovedDistance;
	};
};