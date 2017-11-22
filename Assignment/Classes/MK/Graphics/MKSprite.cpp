// Include MK
#include "MKSprite.h"

NS_MK_BEGIN

void MKSprite::SetTextureParameters(MKSprite* _sprite)
{
	// Set the texture parameters to GL_REPEAT.
	Texture2D::TexParams textureParameters;
	textureParameters.minFilter = GL_NEAREST;
	textureParameters.magFilter = GL_NEAREST;

	if (_sprite->GetUseTextureRepeat())
	{
		textureParameters.wrapS = GL_REPEAT;
		textureParameters.wrapT = GL_REPEAT;
	}
	else
	{
		textureParameters.wrapS = GL_CLAMP_TO_EDGE;
		textureParameters.wrapT = GL_CLAMP_TO_EDGE;
	}

	_sprite->getTexture()->setTexParameters(textureParameters);
}

void MKSprite::SetOriginalTextureSize(MKSprite* _sprite)
{
	_sprite->m_TextureOriginalSize.width = _sprite->getContentSize().width;
	_sprite->m_TextureOriginalSize.height = _sprite->getContentSize().height;
}

MKSprite* MKSprite::create(const std::string& _fileName, bool _useTextureRepeat)
{
	MKSprite *sprite = new (std::nothrow) MKSprite(_useTextureRepeat);
	if (sprite && sprite->initWithFile(_fileName))
	{
		sprite->autorelease();
		SetTextureParameters(sprite);
		SetOriginalTextureSize(sprite);
		return sprite;
	}

	CC_SAFE_DELETE(sprite);
	return nullptr;
}

MKSprite* MKSprite::createWithTexture(Texture2D *texture, bool _useTextureRepeat)
{
	MKSprite *sprite = new (std::nothrow) MKSprite(_useTextureRepeat);
	if (sprite && sprite->initWithTexture(texture))
	{
		sprite->autorelease();
		SetTextureParameters(sprite);
		SetOriginalTextureSize(sprite);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

MKSprite* MKSprite::createWithSize(const std::string& _fileName, const Size& _desiredSize, bool _useTextureRepeat)
{
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// Original image
	auto originalSprite = Sprite::create(_fileName);;
	originalSprite->setFlippedY(true);
	// Scale the sprite to our desired size.
	originalSprite->setScale(
		_desiredSize.width / originalSprite->getContentSize().width,
		_desiredSize.height / originalSprite->getContentSize().height);
	//originalSprite->setPosition(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f);
	originalSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
	originalSprite->setPosition(visibleOrigin.x, visibleOrigin.y);

	// Render the sprite onto a texture.
	auto renderTexture = RenderTexture::create(_desiredSize.width, _desiredSize.height, Texture2D::PixelFormat::RGBA8888);
	// What this does is that originalSprite will be set to the texture.
	renderTexture->begin();
	originalSprite->visit();
	renderTexture->end();

	// Create new Sprite which is the size that we want.
	MKSprite* resultSprite = MKSprite::createWithTexture(renderTexture->getSprite()->getTexture(), _useTextureRepeat);
	SetTextureParameters(resultSprite);

	return resultSprite;
}

NS_MK_END