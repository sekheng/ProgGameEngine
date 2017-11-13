// Include MK
#include "MKSceneDerived.h"
#include "MK/SceneManagement/MKSceneManager.h"

NS_MK_BEGIN

bool MKSceneDerived::init()
{
	if (!Super::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("MKSceneDerived", "fonts/Marker Felt.ttf", 24);
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));
	
	// add the label as a child to this layer
	this->addChild(label, 1);

	scheduleUpdate();

	return true;
}

void MKSceneDerived::update(float _deltaTime)
{
}

NS_MK_END