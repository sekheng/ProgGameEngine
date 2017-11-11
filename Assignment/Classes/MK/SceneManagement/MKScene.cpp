// Include MK
#include "MKScene.h"

NS_MK_BEGIN

bool MKScene::init()
{
	if (Super::init() == false)
	{
		return false;
	}

	/*
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("MKSceneDerived", "fonts/Marker Felt.ttf", 24);
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);
	*/

	return true;
}

NS_MK_END