#include "GTCharacterStatNode.h"
#include "..\..\MK\Common\MKAssertions.h"
#include "..\..\GT\Animation\GTAnimationHandlerNode.h"
#include "..\..\GT\Audio\GTSimperMusicSys.h"
#include "..\GameLogic\GTCollisionCategory.h"

using namespace GinTama;
USING_NS_CC;

const static float ACCEPTABLE_VELY = 0.9f;

GTCharacterStatNode::GTCharacterStatNode()
    : m_health(3)
    , m_CurrentState(RUNNING)
    , m_physicsNode(nullptr)
    , m_SlidePhyShape(nullptr)
    , m_OriginPhyShape(nullptr)
    , m_countingFloat(0)
    , m_SpeedX(0)
    , m_MovedDistance(0)
    , m_DurationOfSlide(0)
    , m_SlideCountDown(0)
    , m_DurationOfDash(0)
    , m_DashCountDown(0)
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

bool GTCharacterStatNode::setHealth(const int &_health)
{
	m_health = _health;
	if (m_health < 0)
		return false;
	return true;
}

bool GTCharacterStatNode::adjustHealth(const int &_value)
{
	m_health += _value;
	if (m_health < 0)
		return false;
	return true;
}

int GTCharacterStatNode::getHealth()
{
    return m_health;
}

void GTCharacterStatNode::update(float delta)
{
    if (!m_AnimHandler)
    {
        m_AnimHandler = _parent->getChildByTag<GTAnimationHandlerNode*>(69);
    }
    // if there is the physics node
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
                setState(RUNNING);
                m_AnimHandler->transitState("Idle");
            }
            break;
        case DASH:
            m_DashCountDown += delta;
            if (m_DashCountDown > m_DurationOfDash)
            {
                m_SpeedX *= 0.5f;
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
    }

    // since the moving along of the x-axis is different from the physics system, we will be using m_SpeedX
    float zeDistMoved = m_SpeedX * delta;
    m_MovedDistance += zeDistMoved;
    m_TotalDist += zeDistMoved;
    // we will need to move the parent transform position
    _parent->setPositionX(_parent->getPositionX() + m_MovedDistance);
    // then we will need to reset the position X after every 1000 distance
    if (m_MovedDistance > 1000.0f)
    {
        m_MovedDistance -= 1000.0f;
        _parent->setPositionX(_parent->getPositionX() + m_MovedDistance);
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
    // Then set the contact listener when it happens
    m_physicsNode->setCategoryBitmask(GT_COLLISION_CATEGORY_PLAYER);
    m_physicsNode->setCollisionBitmask(GT_COLLISION_CATEGORY_GROUND | GT_COLLISION_CATEGORY_PLAYER);
    m_physicsNode->setContactTestBitmask(GT_COLLISION_CATEGORY_GROUND | GT_COLLISION_CATEGORY_OBSTACLE);

    m_SlidePhyShape->setCategoryBitmask(GT_COLLISION_CATEGORY_PLAYER);
    m_SlidePhyShape->setCollisionBitmask(GT_COLLISION_CATEGORY_GROUND | GT_COLLISION_CATEGORY_PLAYER);
    m_SlidePhyShape->setContactTestBitmask(GT_COLLISION_CATEGORY_OBSTACLE);
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
            // need to immediately apply that impulse
            m_physicsNode->applyImpulse(Vec2(0, -100.f));
            m_AnimHandler->transitState("Slide");
            m_SlideCountDown = 0;
            m_CurrentState = _whatState;
            GTSimperMusicSys::GetInstance()->playSound("Slide");
        default:
                break;
        }
        break;
    case DASH:
        // then the speed of the character will be multiplied by 2!
        switch (m_CurrentState)
        {
        case RUNNING:
            m_DashCountDown = 0;
            // we will need to ensure that the character is sliding then we multiply the speed by 2!
            m_SpeedX *= 2.0f;
            m_AnimHandler->transitState("Dash");
            m_CurrentState = _whatState;
            GTSimperMusicSys::GetInstance()->playSound("Dash");
            break;
        default:
            break;
        }
        break;
    case DEAD:
        m_CurrentState = _whatState;
        // if dead then stop the speed and change the transition
        m_SpeedX = 0;
        m_AnimHandler->transitState("Died");
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

void GTCharacterStatNode::setDashDuration(const float &_duration)
{
    if (_duration > 0)
        m_DurationOfDash = _duration;
}

float GTCharacterStatNode::getDashDuration()
{
    return m_DurationOfDash;
}

gtBool GTCharacterStatNode::OnContactBegin(cocos2d::PhysicsContact &_contact)
{
    if (_contact.getShapeA()->getCategoryBitmask() == GT_COLLISION_CATEGORY_GROUND || _contact.getShapeB()->getCategoryBitmask() == GT_COLLISION_CATEGORY_GROUND)
    {
        switch (m_CurrentState)
        {
        case RUNNING:
            break;
        default:
            m_physicsNode->setVelocity(Vec2(m_physicsNode->getVelocity().x, 0.f));
            m_physicsNode->resetForces();
            // this means the character touched the ground!
            m_AnimHandler->transitState("Idle");
            setState(RUNNING);
            break;
        }
    }
    return true;
}

gtBool GTCharacterStatNode::CompareBitMask(gtU32 _lhs, gtU32 _rhs)
{
    gtU32 largerNum = _lhs;
    if (largerNum < _rhs)
        largerNum = _rhs;
    gtU32 zeComparedMask = (_lhs | _rhs);
    // if the bits still remains the same after that, the values are the same
    if (zeComparedMask == largerNum)
        return true;
    return false;
}

bool GTCharacterStatNode::CharJump()
{
    switch (m_CurrentState)
    {
    case GinTama::RUNNING:
        setState(JUMPING);
        m_physicsNode->applyImpulse(Vec2(0, 7500.f));
        GTSimperMusicSys::GetInstance()->playSound("Jump");
        return true;
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