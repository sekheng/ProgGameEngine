// Include MK
#include "MKBackgrounds.h"
#include "../SceneManagement/MKScene.h"

NS_MK_BEGIN

MKBackgrounds::MKBackgrounds(MKScene* _scene, const char* _imageRear, const char* _imageMiddle, const char* _imageFront)
{
	// Initalise the backgrounds as nullptr by default.
	m_Backgrounds = new cocos2d::Sprite*[NUM_BACKGROUNDLAYER];
	for (mkU32 i = 0; i < NUM_BACKGROUNDLAYER; ++i)
	{
		m_Backgrounds[i] = nullptr;
	}

	// Create the backgrounds.
	if (_imageRear != nullptr)
	{
		m_Backgrounds[REAR] = Sprite::create(_imageRear);
	}

	if (_imageMiddle != nullptr)
	{
		m_Backgrounds[MIDDLE] = Sprite::create(_imageMiddle);
	}

	if (_imageFront != nullptr)
	{
		m_Backgrounds[FRONT] = Sprite::create(_imageFront);
	}

	// Add the backgrounds to the scene.
	for (mkU32 i = 0; i < NUM_BACKGROUNDLAYER; ++i)
	{
		if (m_Backgrounds[i] != nullptr)
		{
			SetBackgroundTextureParameters(m_Backgrounds[i]);
			_scene->addChild(m_Backgrounds[i]);
		}
	}
}

MKBackgrounds::~MKBackgrounds()
{
}

void MKBackgrounds::SetBackgroundTextureParameters(Sprite* _background)
{
	Texture2D::TexParams textureParameters;
	textureParameters.minFilter = GL_NEAREST;
	textureParameters.magFilter = GL_NEAREST;
	textureParameters.wrapS = GL_REPEAT;
	textureParameters.wrapT = GL_REPEAT;
	_background->getTexture()->setTexParameters(textureParameters);
}

void MKBackgrounds::ScrollBackground(BackgroundLayer _layer, float _offset)
{
	if (m_Backgrounds[_layer] == nullptr)
	{
		return;
	}

	Rect scrollRect = m_Backgrounds[_layer]->getTextureRect();
	scrollRect.origin.x += _offset;
	m_Backgrounds[_layer]->setTextureRect(scrollRect);
}

NS_MK_END