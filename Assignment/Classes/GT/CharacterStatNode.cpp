#include "CharacterStatNode.h"
#include "MK/Common/MKAssertions.h"
#include "AnimationHandlerNode.h"
#include "SimperMusicSys.h"

using namespace GinTama;
USING_NS_CC;

const static float ACCEPTABLE_VELY = 0.9f;

CharacterStatNode::CharacterStatNode()
	: m_health(3)
    , m_CurrentState(RUNNING)
    , m_physicsNode(nullptr)
    , m_countingFloat(0)
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
        //if (zeVel.y < EPSILON && zeVel.y > -EPSILON)
        //{
        //    switch (m_CurrentState)
        //    {
        //    case GinTama::JUMPING:
        //        // only when it was actually jumping then start landing
        //        setState(RUNNING);
        //        getParent()->getChildByTag<AnimationHandlerNode*>(69)->transitState("Idle");
        //        break;
        //    default:
        //        break;
        //    }
        //}
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
                    getParent()->getChildByTag<AnimationHandlerNode*>(69)->transitState("BeginJump");
                    break;
                default:
                    break;
                }
                m_countingFloat = 0;
            }
        }
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