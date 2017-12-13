#pragma once

#include "cocos2d.h"
#include "..\GameLogic\Obstacle\GTObstacleMacros.h"
#include "..\..\MK\SceneManagement\MKScene.h"
#include "..\Animation\GTAnimationHandlerNode.h"

USING_NS_MK

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
        GT_INITIALISECONTACTLISTENER(GTCharacterStatNode);
        GT_DEINITIALISECONTACTLISTENER(GTCharacterStatNode);

		static GTCharacterStatNode* create(MKScene *_scene);
        static GTCharacterStatNode* create(MKScene *_scene, cocos2d::PhysicsBody *_physicsBody);

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
        float getTotalDistanceWalk() const { return m_TotalDist; }
        /** Converts m_totalDist which is pixel initially
        *
        * @return converted distance
        */
        gtU32 getConvertedDistWalk();

        virtual void update(float delta);

        void setPhysicsNode(cocos2d::PhysicsBody *_physicsBody);

        /** Makes the character do the jumping if possible
        *
        * @return true if it is able to jump
        */
        bool CharJump();

        bool setState(CHARACTER_STATE _whatState);
        CHARACTER_STATE getCurrentState();
        MKScene* GetScene() { return m_Scene; }
        const MKScene* GetScene() const { return m_Scene; }

	protected:
		GTCharacterStatNode();
		virtual ~GTCharacterStatNode();
        /** For contact with the ground
        *
        * @param _contact will be pass by the physics
        * @return true
        */
        gtBool OnContactBegin(cocos2d::PhysicsContact &_contact);
        gtBool CompareBitMask(gtU32 _lhs, gtU32 _rhs);

		int m_health;
        cocos2d::PhysicsBody* m_physicsNode;
        cocos2d::PhysicsShape *m_SlidePhyShape, *m_OriginPhyShape;
        CHARACTER_STATE m_CurrentState;
        float m_countingFloat;
        float m_SpeedX;
        float m_MovedDistance, m_TotalDist;
        float m_DurationOfSlide, m_SlideCountDown;
        float m_DurationOfDash, m_DashCountDown;
        MKScene* m_Scene = nullptr;
        GTAnimationHandlerNode *m_AnimHandler;

        cocos2d::EventListenerPhysicsContact* m_ContactListener = NULL;
    };
}