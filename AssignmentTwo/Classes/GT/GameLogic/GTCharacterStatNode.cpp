#include "GTCharacterStatNode.h"
#include "../../MK/Common/MKAssertions.h"
#include "../../MK/Common/MKMathsHelper.h"
#include "../../GT/Animation/GTAnimationHandlerNode.h"
#include "../../GT/Audio/GTSimperMusicSys.h"
#include "../GameLogic/GTCollisionCategory.h"
#include "Powerup/GTSphereShieldPowerUp.h"

USING_NS_GT
USING_NS_CC;

const static float ACCEPTABLE_VELY = 0.9f;

GTCharacterStatNode::GTCharacterStatNode()
    : m_CurrentState(RUNNING)
    , m_physicsNode(nullptr)
    , m_SlidePhyShape(nullptr)
    , m_OriginPhyShape(nullptr)
    , m_countingFloat(0)
    , m_SpeedX(0)
    , m_MovedDistance(0)
    , m_DurationOfSlide(0)
    , m_SlideCountDown(0)
    , m_AnimHandler(nullptr)
    , m_TotalDist(0)
    , m_ReviveCounter(1)
    , m_ResetDistanceX(300.0f)
    , m_ActingState(ACTING_STATE::ON_GROUND)
{
	setTag(1);
}

GTCharacterStatNode::~GTCharacterStatNode()
{
    if (m_physicsNode)
    {
        m_physicsNode->release();
        m_SlidePhyShape->release();
        m_OriginPhyShape->release();
        DeinitialiseContactListener();
    }
}

GTCharacterStatNode *GTCharacterStatNode::create(MKScene *_scene)
{
	GTCharacterStatNode *zeNode = new GTCharacterStatNode();
    zeNode->autorelease();
    zeNode->m_Scene = _scene;
	return zeNode;
}

GTCharacterStatNode *GTCharacterStatNode::create(MKScene *_scene, PhysicsBody *_physicsBody)
{
    GTCharacterStatNode *zeNode = create(_scene);
    zeNode->setPhysicsNode(_physicsBody);
    return zeNode;
}

void GTCharacterStatNode::update(float delta)
{
    if (!m_AnimHandler)
    {
        m_AnimHandler = _parent->getChildByTag<GTAnimationHandlerNode*>(69);
    }

    // If there is the physics node
    if (m_physicsNode)
    {
        // if this character is sliding, we add the countdown!
        switch (m_CurrentState)
        {
        case SLIDE:
            m_SlideCountDown += delta;
            if (m_SlideCountDown > m_DurationOfSlide)
            {
                // then we can slide back!
                m_physicsNode->removeShape(m_SlidePhyShape, false);
                m_physicsNode->addShape(m_OriginPhyShape, false);
                setPhysicsBitmasks(m_physicsNode);

                setState(RUNNING);
                m_AnimHandler->transitState("Idle");
            }
            break;
        case DEAD:
            // just use the shortcut that Vec2 has provided
            if (m_physicsNode->getVelocity() != Vec2::ANCHOR_BOTTOM_LEFT)
            {
                _parent->setPositionX(m_DeadPositionX);
            }
            break;
        default:
            Vec2 zeVel = m_physicsNode->getVelocity();
            if (zeVel.y < -ACCEPTABLE_VELY)
            {
                m_countingFloat += delta;
                if (m_countingFloat > 0.2f)
                {
                    switch (m_CurrentState)
                    {
                    case RUNNING:
                        // this means that the character is falling!
                        setState(JUMPING);
                        break;
                    default:
                        break;
                    }
                    m_countingFloat = 0;
                }
            }
            break;
        }
        m_physicsNode->setVelocity(Vec2(m_SpeedX, m_physicsNode->getVelocity().y));
        m_TotalDist += (m_SpeedX * delta);
    }

}

void GTCharacterStatNode::setPhysicsBitmasks(cocos2d::PhysicsBody *_physicsBody)
{
    if (_physicsBody)
    {
        _physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_PLAYER);
        _physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_GROUND);
        _physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_GROUND | GT_COLLISION_CATEGORY_OBSTACLE | GT_COLLISION_CATEGORY_POWERUP);
    }
}

void GTCharacterStatNode::setPhysicsNode(cocos2d::PhysicsBody *_physicsBody)
{
    m_physicsNode = _physicsBody;
    m_physicsNode->retain();
    m_OriginPhyShape = _physicsBody->getShape(0);
    m_OriginPhyShape->retain();
    // then have another
    Size zeSlideSize = Size(_physicsBody->getOwner()->getContentSize().width * 0.5f, _physicsBody->getOwner()->getContentSize().height * 0.3f);
    m_SlidePhyShape = PhysicsShapeBox::create(zeSlideSize, _physicsBody->getFirstShape()->getMaterial(), _physicsBody->getFirstShape()->getOffset());
    m_SlidePhyShape->retain();
    m_SlidePhyShape->setCategoryBitmask(GT_COLLISION_CATEGORY_PLAYER);
    m_SlidePhyShape->setCollisionBitmask(GT_COLLISION_CATEGORY_GROUND);
    m_SlidePhyShape->setContactTestBitmask(GT_COLLISION_CATEGORY_GROUND | GT_COLLISION_CATEGORY_OBSTACLE | GT_COLLISION_CATEGORY_POWERUP);

    // Then set the contact listener when it happens
    setPhysicsBitmasks(m_physicsNode);
    InitialiseContactListener();
    // initialize on contact exit!
    m_ContactListener->onContactSeparate = CC_CALLBACK_1(GTCharacterStatNode::OnContactSeparate, this);
}

bool GTCharacterStatNode::setState(CHARACTER_STATE _whatState)
{
    switch (_whatState)
    {
    case RUNNING:
        switch (m_CurrentState)
        {
        case DASH:
        case SLIDE:
        case JUMPING:
        case SLIDE_JUMP:
            m_CurrentState = _whatState;
            break;
        default:
            break;
        }
        break;
    case JUMPING:
        switch (m_CurrentState)
        {
        case RUNNING:
            // u can only change from running to jumping!
            m_CurrentState = _whatState;
            m_AnimHandler->transitState("BeginJump");
            break;
        default:
            break;
        }
        break;
    case SLIDE:
        switch (m_CurrentState)
        {
        case RUNNING:
            // only slide when it is only running!
            m_physicsNode->removeShape(m_OriginPhyShape, false);
            m_physicsNode->addShape(m_SlidePhyShape, false);
            m_physicsNode->resetForces();
            // need to immediately apply that impulse
            m_AnimHandler->transitState("Slide");
            m_SlideCountDown = 0;
            m_CurrentState = _whatState;
            GTSimperMusicSys::GetInstance()->playSound("Slide");
            break;
        case JUMPING:
            if (m_ActingState != ACTING_STATE::ON_GROUND)
            {
                m_physicsNode->setVelocity(Vec2(m_physicsNode->getVelocity().x, 0));
                m_physicsNode->applyImpulse(Vec2(0, -10000.f));
                m_CurrentState = SLIDE_JUMP;
            }
            GTSimperMusicSys::GetInstance()->playSound("Slide");
            break;
        default:
                break;
        }
        break;
    case DEAD:
        switch (m_CurrentState)
        {
        case DEAD:
            // ensure that the current state is that the player is not dead otherwise it does nothing
            break;
        default:
            m_CurrentState = _whatState;
            // if dead then stop the speed and change the transition
            m_SpeedX = 0;
            m_DeadPositionX = _parent->getPositionX();
            m_AnimHandler->transitState("Died");
            break;
        }
        break;
    case REVIVE:
        MK_ASSERTWITHMSG((m_CurrentState == DEAD), "Player shouldn't be revived when it is still not dead!");
        if (m_ReviveCounter > 0)
        {
            --m_ReviveCounter;
            m_SpeedX = m_OriginalSpeedX;
            m_CurrentState = RUNNING;
            // then set the animation to normal
            m_AnimHandler->transitState("Idle");
        }
        break;
    default:
        MK_ASSERTWITHMSG((true == false), "Something is wrong with setState!");
        break;
    }
    return false;
}

CHARACTER_STATE GTCharacterStatNode::getCurrentState()
{
    return m_CurrentState;
}

float GTCharacterStatNode::getSpeedX()
{
    return m_SpeedX;
}

void GTCharacterStatNode::setSpeedX(const float &_speed)
{
    m_SpeedX = _speed;
    m_OriginalSpeedX = _speed;
}

void GTCharacterStatNode::adjustSpeedX(const float &_value)
{
    m_SpeedX += _value;
}

void GTCharacterStatNode::setSlideDuration(const float &_duration)
{
    if (_duration > 0)
        m_DurationOfSlide = _duration;
}

float GTCharacterStatNode::getSlideDuration()
{
    return m_DurationOfSlide;
}

gtBool GTCharacterStatNode::OnContactBegin(cocos2d::PhysicsContact &_contact)
{
    PhysicsShape* physicsShapeA = _contact.getShapeA();
    PhysicsShape* physicsShapeB = _contact.getShapeB();
    // Ignore this collision if we're not involved.
    if (physicsShapeA->getBody() != m_physicsNode &&
        physicsShapeB->getBody() != m_physicsNode)
    {
        return false;
    }

    PhysicsBody* otherPhysicsBody = (physicsShapeA->getBody() != m_physicsNode) ? physicsShapeA->getBody() : physicsShapeB->getBody();
    // Dafuq? How can we collide with ourselves?
    if (otherPhysicsBody == m_physicsNode)
    {
        return false;
    }

    if (MKMathsHelper::ContainsBitmask<mkS32>(GT_COLLISION_CATEGORY_GROUND, otherPhysicsBody->getCategoryBitmask()))
    {
        m_ActingState = ACTING_STATE::ON_GROUND;
        switch (m_CurrentState)
        {
		case RUNNING:
        case JUMPING:
        case SLIDE_JUMP:
            // this means the character touched the ground!
            m_AnimHandler->transitState("Idle");
            setState(RUNNING);
        case DEAD:
            m_physicsNode->setVelocity(Vec2(m_physicsNode->getVelocity().x, 0.f));
            m_physicsNode->resetForces();
            break;
        default:
            break;
        }
        return true;
    }
    else if (MKMathsHelper::ContainsBitmask<mkS32>(GT_COLLISION_CATEGORY_OBSTACLE, otherPhysicsBody->getCategoryBitmask()))
    {
        setState(DEAD);
        return true;
    }
	else if (MKMathsHelper::ContainsBitmask<mkS32>(GT_COLLISION_CATEGORY_POWERUP, otherPhysicsBody->getCategoryBitmask()))
	{
		return true;
	}
    return false;
}


bool GTCharacterStatNode::CharJump()
{
    switch (m_CurrentState)
    {
    case GinTama::RUNNING:
        setState(JUMPING);
        m_physicsNode->applyImpulse(Vec2(0, 30000.f));
        GTSimperMusicSys::GetInstance()->playSound("Jump");
        return true;
        break;
    case SLIDE:
        // if character is attempting to jump when it is still sliding, it means the character is trying to stand up!
        // we can just cheat here and put the countdown pass the duration
        m_SlideCountDown = m_DurationOfSlide + 1.f;
        break;
    default:
        break;
    }
    return false;
}

gtU32 GTCharacterStatNode::getConvertedDistWalk()
{
    // dont know what to convert this to for now
    return m_TotalDist;
}

void GTCharacterStatNode::setReviveCounter(const int &_reviveTimes)
{
    MK_ASSERTWITHMSG((_reviveTimes < 0), "Revive times cannot be less than 0!");
    m_ReviveCounter = _reviveTimes;
}

int GTCharacterStatNode::getReviveCounter()
{
    return m_ReviveCounter;
}

void GTCharacterStatNode::PassInvokeFunctionWhenResetDistance(std::function<void(float)> _functionCall)
{
    m_VectorOfResetDistCalls.push_back(_functionCall);
}

void GTCharacterStatNode::ResetPlayerDistance()
{
    // check whether it has crossed over the certain distance
    if (m_physicsNode->getOwner()->getPositionX() > m_ResetDistanceX)
    {
        m_PlayerPosXAfterReset = m_physicsNode->getOwner()->getPositionX() - m_ResetDistanceX;
        m_physicsNode->getOwner()->setPositionX(m_PlayerPosXAfterReset);
        for (std::vector<std::function<void(float)>>::iterator it = m_VectorOfResetDistCalls.begin(), end = m_VectorOfResetDistCalls.end(); it != end; ++it)
        {
            (*it)(-m_ResetDistanceX);
        }
    }
}

gtBool GTCharacterStatNode::OnContactSeparate(cocos2d::PhysicsContact &_contact)
{
    PhysicsShape* physicsShapeA = _contact.getShapeA();
    PhysicsShape* physicsShapeB = _contact.getShapeB();
    // Ignore this collision if we're not involved.
    if (physicsShapeA->getBody() != m_physicsNode &&
        physicsShapeB->getBody() != m_physicsNode)
    {
        return false;
    }

    PhysicsBody* otherPhysicsBody = (physicsShapeA->getBody() != m_physicsNode) ? physicsShapeA->getBody() : physicsShapeB->getBody();
    // Dafuq? How can we collide with ourselves?
    if (otherPhysicsBody == m_physicsNode)
    {
        return false;
    }
    // check to make sure that it is the ground that is being separated
    if (MKMathsHelper::ContainsBitmask<mkS32>(GT_COLLISION_CATEGORY_GROUND, otherPhysicsBody->getCategoryBitmask()))
    {
        m_ActingState = ACTING_STATE::FLOATING;
        return true;
    }
    return false;
}