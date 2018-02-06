#ifndef UI_CREATOR_H
#define UI_CREATOR_H

//Include cocos2d
#include "cocos2d.h"

//Include UI classes
#include "ui/UIButton.h"
#include "ui/UISlider.h"
#include "ui/UIScrollView.h"

//Include MKMacros
#include "../MK/Common/MKAssertions.h"
#include "../MK/Common/MKMacros.h"
#include "../MK/Common/MKSingletonTemplate.h"

#include <string>

USING_NS_CC;
NS_MK_BEGIN

class MKUICreator : public MKSingletonTemplate<MKUICreator>
{
	friend MKSingletonTemplate<MKUICreator>;
public:
	//Create Button
	ui::Button* createButton(const Vec2& _position, const std::string& _normalSprite, const std::string& _pressedSprite, const std::string& _labelName, const std::function<void(Ref*)>& _funcPtr, const float& _scale = 1.75f)
	{
		//GoSceneButton::GoSceneButton("GameScene");
		auto label = Label::createWithTTF(_labelName, "fonts/Marker_Felt.ttf", 20 / Director::getInstance()->getContentScaleFactor());
		label->setPosition(_position);
		auto button = ui::Button::create(_normalSprite, _pressedSprite);
		button->setPosition(_position);
		button->setScale(_scale);
		//button->addTouchEventListener(_funcPtr);
        button->addClickEventListener(_funcPtr);
		button->setTitleLabel(label);

		return button;
	}
	//Create Slider
	ui::Slider* createSlider(const Vec2& _position, const std::string& _barSprite, const std::string& _progressSprite, const std::string& _ballSprite, const std::function<void(Ref*, ui::Slider::EventType)>& _funcPtr)
	{
		auto slider = ui::Slider::create();
		slider->loadBarTexture(_barSprite);
		slider->loadSlidBallTextureNormal(_ballSprite);
		slider->loadProgressBarTexture(_progressSprite);
		slider->setPosition(_position);
		slider->addEventListener(_funcPtr);

		return slider;
	}
	//Create ScrollView
	ui::ScrollView* createScroller(ui::ScrollView::Direction _direction, const Size& _contentSize, const Size& _innerContainerSize, const bool& _bounceEnable, const Vec2& _position)
	{
		auto scroller = ui::ScrollView::create();
		scroller->setDirection(_direction);
		scroller->setContentSize(_contentSize);
		scroller->setInnerContainerSize(_innerContainerSize);
		scroller->setBounceEnabled(_bounceEnable);
		scroller->setAnchorPoint(Vec2(0.5f, 0.5f));
		scroller->setPosition(_position);

		return scroller;
	}

private:
	MKUICreator() {}
	~MKUICreator() {}

	std::unordered_map<std::string, TTFConfig*> m_NameTTFMap;
};

NS_MK_END

#endif
