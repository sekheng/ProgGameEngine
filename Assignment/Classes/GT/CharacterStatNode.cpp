#include "CharacterStatNode.h"

using namespace GinTama;

CharacterStatNode::CharacterStatNode()
	: m_health(3)
{
	setTag(1);
}

CharacterStatNode::~CharacterStatNode()
{

}

CharacterStatNode *CharacterStatNode::create()
{
	CharacterStatNode *zeNode = new CharacterStatNode();
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