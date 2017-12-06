#include "CharacterStatNode.h"
#include "MK/Common/MKAssertions.h"
#include "GT/AnimationHandlerNode.h"
#include "GT/SimperMusicSys.h"

using namespace GinTama;
USING_NS_CC;

const static float ACCEPTABLE_VELY = 0.9f;

CharacterStatNode::CharacterStatNode()
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
{
	setTag(1);
}

CharacterStatNode::~CharacterStatNode()
{
    if (m_physicsNode)
    {
        m_physicsNode->release();
        m_SlidePhyShape->release();
        m_OriginPhyShape->release();
    }
}

CharacterStatNode *CharacterStatNode::create()
{
	CharacterStatNode *zeNode = new CharacterStatNode();
    zeNode->autorelease();
	return zeNode;
}

CharacterStatNode *CharacterStatNode::create(PhysicsBody *_physicsBody)
{
    CharacterStatNode *zeNode = create();
    zeNode->setPhysicsNode(_physicsBody);
    return zeNode;
}

bool CharacterStatNode::setHealth(const int &_health)
{
	m_health = _health;
	if (m_health < 0)
		return false;
	return true;
}

bool CharacterStatNode::adjustHealth(const int &_value)
{
	m_health += _value;
	if (m_health < 0)
		return false;
	return true;
}

int CharacterStatNode::getHealth()
{
    return m_health;
}

void CharacterStatNode::update(float delta)
{
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
                _parent->getChildByTag<AnimationHandlerNode*>(69)->transitState("Idle");
            }
            break;
        case DASH:
            m_DashCountDown += delta;
            if (m_DashCountDown > m_DurationOfDash)
            {
                m_SpeedX *= 0.5f;
                setState(RUNNING);
                _parent->getChildByTag<AnimationHandlerNode*>(69)->transitState("Idle");
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
    m_MovedDistance += m_SpeedX * delta;
    // we will need to move the parent transform position
    _parent->setPositionX(_parent->getPositionX() + m_MovedDistance);
    // then we will need to reset the position X after every 1000 distance
    if (m_MovedDistance > 1000.0f)
    {
        m_MovedDistance -= 1000.0f;
        _parent->setPositionX(_parent->getPositionX() + m_MovedDistance);
    }
}

void CharacterStatNode::setPhysicsNode(cocos2d::PhysicsBody *_physicsBody)
{
    m_physicsNode = _physicsBody;
    m_physicsNode->retain();
    m_OriginPhyShape = _physicsBody->getShape(0);
    m_OriginPhyShape->retain();
    // then have another
    Size zeSlideSize = Size(_physicsBody->getOwner()->getContentSize().width * 0.5f, _physicsBody->getOwner()->getContentSize().height * 0.3f);
    m_SlidePhyShape = PhysicsShapeBox::create(zeSlideSize, _physicsBody->getFirstShape()->getMaterial(), _physicsBody->getFirstShape()->getOffset());
    m_SlidePhyShape->retain();
}

bool CharacterStatNode::setState(CHARACTER_STATE _whatState)
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
            _parent->getChildByTag<AnimationHandlerNode*>(69)->transitState("BeginJump");
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
            _parent->getChildByTag<AnimationHandlerNode*>(69)->transitState("Slide");
            m_SlideCountDown = 0;
            m_CurrentState = _whatState;
            SimperMusicSys::GetInstance()->playSound("Slide");
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
            _parent->getChildByTag<AnimationHandlerNode*>(69)->transitState("Dash");
            m_CurrentState = _whatState;
            SimperMusicSys::GetInstance()->playSound("Dash");
            break;
        default:
            break;
        }
        break;
    case DEAD:
        m_CurrentState = _whatState;
        // if dead then stop the speed and change the transition
        m_SpeedX = 0;
        _parent->getChildByTag<AnimationHandlerNode*>(69)->transitState("Died");
        break;
    default:
        MK_ASSERTWITHMSG(true == false, "Something is wrong with setState!");
        break;
    }
    return false;
}

CHARACTER_STATE CharacterStatNode::getCurrentState()
{
    return m_CurrentState;
}

float CharacterStatNode::getSpeedX()
{
    return m_SpeedX;
}

void CharacterStatNode::setSpeedX(const float &_speed)
{
    m_SpeedX = _speed;
}

void CharacterStatNode::adjustSpeedX(const float &_value)
{
    m_SpeedX += _value;
}

void CharacterStatNode::setSlideDuration(const float &_duration)
{
    if (_duration > 0)
        m_DurationOfSlide = _duration;
}

float CharacterStatNode::getSlideDuration()
{
    return m_DurationOfSlide;
}

void CharacterStatNode::setDashDuration(const float &_duration)
{
    if (_duration > 0)
        m_DurationOfDash = _duration;
}

float CharacterStatNode::getDashDuration()
{
    return m_DurationOfDash;
}