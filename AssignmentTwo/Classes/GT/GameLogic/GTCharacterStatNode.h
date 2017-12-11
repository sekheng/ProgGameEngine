#pragma once

#include "cocos2d.h"

namespace GinTama
{
    enum CHARACTER_STATE {
        RUNNING = 0x0001,
        JUMPING = 0x0002,
        DEAD = 0x0004,
        SLIDE = 0x0008,
        DASH = 0x0010,
        TOTAL_STATE = RUNNING | JUMPING | DEAD | SLIDE | DASH,
    };

	class GTCharacterStatNode : public cocos2d::Node
	{
	public:
		static GTCharacterStatNode* create();
        static GTCharacterStatNode* create(cocos2d::PhysicsBody *_physicsBody);

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

        void setSlideDuration(const float &_duration);
        void setDashDuration(const float &_duration);

        float getSpeedX();
        int getHealth();
        float getSlideDuration();
        float getDashDuration();

        virtual void update(float delta);

        void setPhysicsNode(cocos2d::PhysicsBody *_physicsBody);

        bool setState(CHARACTER_STATE _whatState);
        CHARACTER_STATE getCurrentState();

	protected:
		GTCharacterStatNode();
		virtual ~GTCharacterStatNode();

		int m_health;
        cocos2d::PhysicsBody* m_physicsNode;
        cocos2d::PhysicsShape *m_SlidePhyShape, *m_OriginPhyShape;
        CHARACTER_STATE m_CurrentState;
        float m_countingFloat;
        float m_SpeedX;
        float m_MovedDistance;
        float m_DurationOfSlide, m_SlideCountDown;
        float m_DurationOfDash, m_DashCountDown;
	};
}