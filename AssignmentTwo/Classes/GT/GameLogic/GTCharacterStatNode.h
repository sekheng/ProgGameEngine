#pragma once

#include "cocos2d.h"
#include "../../GT/GameLogic/GTGameplayMacros.h"
#include "../../MK/SceneManagement/MKScene.h"
#include "../Animation/GTAnimationHandlerNode.h"

USING_NS_MK

namespace GinTama
{
    enum CHARACTER_STATE {
        RUNNING = 0x0001,
        JUMPING = 0x0002,
        DEAD = 0x0004,
        SLIDE = 0x0008,
        DASH = 0x0010,
        SLIDE_JUMP = 0x0020,
        REVIVE = 0x0080,
        TOTAL_STATE = RUNNING | JUMPING | DEAD | SLIDE | DASH | SLIDE_JUMP | REVIVE,
    };

	class GTCharacterStatNode : public cocos2d::Node
	{
	public:
        GT_INITIALISECONTACTLISTENER(GTCharacterStatNode);
        GT_DEINITIALISECONTACTLISTENER(GTCharacterStatNode);

        void ResetPlayerDistance();

		static GTCharacterStatNode* create(MKScene *_scene);
        static GTCharacterStatNode* create(MKScene *_scene, cocos2d::PhysicsBody *_physicsBody);

        /** Set the speed to the exact
        *
        */
        void setSpeedX(const float &_speed);
        /** Adjust the value of the speed
        *
        */
        void adjustSpeedX(const float &_value);

        void setSlideDuration(const float &_duration);

        float getSpeedX();
        float getSlideDuration();
        float getTotalDistanceWalk() const { return m_TotalDist; }
        /** Converts m_totalDist which is pixel initially
        *
        * @return converted distance
        */
        gtU32 getConvertedDistWalk();

        virtual void update(float delta);

        void setPhysicsBitmasks(cocos2d::PhysicsBody *_physicsBody);
        void setPhysicsNode(cocos2d::PhysicsBody *_physicsBody);

        void setReviveCounter(const int &_reviveTimes);
        int getReviveCounter();

        /** Makes the character do the jumping if possible
        *
        * @return true if it is able to jump
        */
        bool CharJump();

        bool setState(CHARACTER_STATE _whatState);
        CHARACTER_STATE getCurrentState();
        MKScene* GetScene() { return m_Scene; }
        const MKScene* GetScene() const { return m_Scene; }

        void setResetDistance(float _dist) { m_ResetDistanceX = _dist; }
        float getResetDistance() { return m_ResetDistanceX; }

        void PassInvokeFunctionWhenResetDistance(std::function<void(float)> _functionCall);

	protected:
		GTCharacterStatNode();
		virtual ~GTCharacterStatNode();
        /** For contact with the ground
        *
        * @param _contact will be pass by the physics
        * @return true
        */
        gtBool OnContactBegin(cocos2d::PhysicsContact &_contact);

        gtBool OnContactSeparate(cocos2d::PhysicsContact &_contact);

        cocos2d::PhysicsBody* m_physicsNode;
        cocos2d::PhysicsShape *m_SlidePhyShape, *m_OriginPhyShape;
        CHARACTER_STATE m_CurrentState;
        float m_countingFloat;
        float m_SpeedX, m_OriginalSpeedX;
        float m_ResetDistanceX, m_PlayerPosXAfterReset;
        float m_MovedDistance, m_TotalDist;
        float m_DurationOfSlide, m_SlideCountDown;
        float m_DeadPositionX;
        int m_ReviveCounter;
        MKScene* m_Scene = nullptr;
        GTAnimationHandlerNode *m_AnimHandler;

        std::vector<std::function<void(float)>> m_VectorOfResetDistCalls;

        cocos2d::EventListenerPhysicsContact* m_ContactListener = NULL;

        enum ACTING_STATE
        {
            ON_GROUND = 0x0001,
            FLOATING = 0x0002,
            TOTAL_CURRENT_STATE,
        };
        ACTING_STATE m_ActingState;
    };
}