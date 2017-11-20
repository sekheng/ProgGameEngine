#ifndef MK_BACKGROUND_H
#define MK_BACKGROUND_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../Common/MKPasskey.h"
#include "../Common/MKAssertions.h"
#include "../Common/MKMacros.h"

USING_NS_CC;

NS_MK_BEGIN

class MKScene;

class MKBackgrounds : public cocos2d::Node
{
public:
	enum BackgroundLayer
	{
		REAR = 0,
		MIDDLE,
		FRONT,

		NUM_BACKGROUNDLAYER,
	};

private:
	cocos2d::Sprite** m_Backgrounds = nullptr;

	void SetBackgroundTextureParameters(Sprite* _background);

public:
	MKBackgrounds(MKScene* _scene, const char* _imageRear = nullptr, const char* _imageMiddle = nullptr, const char* _imageFront = nullptr);
	virtual ~MKBackgrounds();

	void ScrollBackground(BackgroundLayer _layer, float _offset);

};

NS_MK_END

#endif