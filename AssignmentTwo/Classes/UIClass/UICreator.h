#ifndef UI_CREATOR_H
#define UI_CREATOR_H

//Include cocos2d
#include "cocos2d.h"

//Include UI classes
#include "ui/UIButton.h"
#include "ui/UISlider.h"

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
	ui::Button* createButton(const Vec2& _position, const std::string& _normalSprite, const std::string& _pressedSprite, const std::string& _labelName, const std::function<void(Ref*)>& _funcPtr)
	{
		//GoSceneButton::GoSceneButton("GameScene");
		auto label = Label::createWithTTF(_labelName, "Fonts/Marker_Felt.ttf", 24);
		label->setPosition(_position);
		auto button = ui::Button::create(_normalSprite, _pressedSprite);
		button->setPosition(_position);
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


private:
	MKUICreator() {}
	~MKUICreator() {}

	std::unordered_map<std::string, TTFConfig*> m_NameTTFMap;
};

NS_MK_END

#endif
