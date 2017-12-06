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
    , m_countingFloat(0)
    , m_SpeedX(0)
    , m_MovedDistance(0)
{
	setTag(1);
}

CharacterStatNode::~CharacterStatNode()
{

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
                    _parent->getChildByTag<AnimationHandlerNode*>(69)->transitState("BeginJump");
                    break;
                default:
                    break;
                }
                m_countingFloat = 0;
            }
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
}

bool CharacterStatNode::setState(CHARACTER_STATE _whatState)
{
    switch (_whatState)
    {
    case GinTama::RUNNING:
        switch (m_CurrentState)
        {
        case JUMPING:
            m_CurrentState = _whatState;
            break;
        default:
            break;
        }
        break;
    case GinTama::JUMPING:
        switch (m_CurrentState)
        {
        case GinTama::RUNNING:
            // u can only change from running to jumping!
            m_CurrentState = _whatState;
            break;
        default:
            break;
        }
        break;
    case GinTama::DEAD:
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