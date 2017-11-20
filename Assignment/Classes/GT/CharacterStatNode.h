#pragma once

#include "cocos2d.h"

namespace GinTama
{
	class CharacterStatNode : public cocos2d::Node
	{
	public:
		static CharacterStatNode* create();

		bool setHealth(const int &_health);
		bool adjustHealth(const int &_value);

		int getHealth();

	protected:
		CharacterStatNode();
		virtual ~CharacterStatNode();

		int m_health;
	};
};