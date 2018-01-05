#include "GTCharacterStatNode.h"
#include "../../MK/Common/MKAssertions.h"
#include "../../MK/Common/MKMathsHelper.h"
#include "../../GT/Animation/GTAnimationHandlerNode.h"
#include "../../GT/Audio/GTSimperMusicSys.h"
#include "../GameLogic/GTCollisionCategory.h"

using namespace GinTama;
USING_NS_CC;
USING_NS_MK

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

                SetPhysicsBitmasks(m_physicsNode);

                setState(RUNNING);
                m_AnimHandler->transitState("Idle");
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
        switch (m_CurrentState)
        {
        case DEAD:
            // just use the shortcut that Vec2 has provided

            if (m_physicsNode->getVelocity() != Vec2::ANCHOR_BOTTOM_LEFT)
            {
                _parent->setPositionX(m_DeadPositionX);
            }
            break;
        default:
            break;
        }
        m_physicsNode->setVelocity(Vec2(m_SpeedX, m_physicsNode->getVelocity().y));
    }
}

void GTCharacterStatNode::SetPhysicsBitmasks(cocos2d::PhysicsBody *_physicsBody)
{
    if (_physicsBody)
    {
        _physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_PLAYER);
        _physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_GROUND);
        _physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_GROUND | GT_COLLISION_CATEGORY_OBSTACLE);
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
    m_SlidePhyShape->setContactTestBitmask(GT_COLLISION_CATEGORY_GROUND | GT_COLLISION_CATEGORY_OBSTACLE);

    // Then set the contact listener when it happens
    SetPhysicsBitmasks(m_physicsNode);
    InitialiseContactListener();
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
            // If character is attempting to slide while jump, it will attempt to do an impulse to straight away go down
            m_physicsNode->setVelocity(Vec2(m_physicsNode->getVelocity().x, 0));
            m_physicsNode->applyImpulse(Vec2(0, -20000.f));
            m_CurrentState = SLIDE_JUMP; 
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
    default:
        MK_ASSERTWITHMSG(true == false, "Something is wrong with setState!");
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
        switch (m_CurrentState)
        {
        case DEAD:
            m_physicsNode->setVelocity(Vec2(m_physicsNode->getVelocity().x, 0.f));
            m_physicsNode->resetForces();
            break;
        case JUMPING:
        case SLIDE_JUMP:
            m_physicsNode->setVelocity(Vec2(m_physicsNode->getVelocity().x, 0.f));
            m_physicsNode->resetForces();
            // this means the character touched the ground!
            m_AnimHandler->transitState("Idle");
            setState(RUNNING);
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
    return m_TotalDist * 100.f;
}