#ifndef MK_SPRITE_H
#define MK_SPRITE_H

// Include Cocos
#include "cocos2d.h"

// Include STL
#include <string>

// Include MK
#include "../Common/MKMacros.h"

USING_NS_CC;
using namespace std;

NS_MK_BEGIN

class MKSprite : public cocos2d::Sprite
{
private:
	Vec2 m_TextureOffset;
	Size m_TextureOriginalSize;
	const bool m_UseTextureRepeat;

	static void SetTextureParameters(MKSprite* _sprite);
	static void SetOriginalTextureSize(MKSprite* _sprite);

public:
	MKSprite(bool _useTextureRepeat)
		: m_UseTextureRepeat(_useTextureRepeat)
	{
		// Default Texture Offset
		m_TextureOffset.x = 0.0f;
		m_TextureOffset.y = 0.0f;
	}
	virtual ~MKSprite() {}

	// If _textureRepeat is true, the size MUST be a power of 2. I don't set the rules, it's OpenGL and Cocos' dumb systems.
	// Cocos stupidly making the texture size the object size is the most infuriating thing.
	static MKSprite* create(const std::string& _fileName, bool _useTextureRepeat = false);
	static MKSprite* MKSprite::createWithTexture(Texture2D *texture, bool _useTextureRepeat = false);
	static MKSprite* MKSprite::createWithSize(const std::string& _fileName, const Size& _desiredSize, bool _useTextureRepeat = false);

	bool GetUseTextureRepeat() const
	{
		return m_UseTextureRepeat;
	}

	// The below functions only work if m_TextureRepeat is true.
	Vec2 GetTextureOffset() const
	{
		return m_TextureOffset;
	}

	void SetTextureOffset(mkF32 _x, mkF32 _y)
	{
		m_TextureOffset.x = _x;
		m_TextureOffset.y = _y;

		Rect textureRect = getTextureRect();
		textureRect.origin.x = m_TextureOriginalSize.width * _x;
		textureRect.origin.y = m_TextureOriginalSize.height * _y;
		setTextureRect(textureRect);
	}

	void OffsetTexture(mkF32 _x, mkF32 _y)
	{
		SetTextureOffset(m_TextureOffset.x + _x, m_TextureOffset.y + _y);
	}

	// This scales the sprite, causing the texture to be repeated.
	Vec2 GetRepeat() const
	{
		Size currentSize = getTextureRect().size;
		return Vec2(currentSize.width / m_TextureOriginalSize.width, currentSize.height / m_TextureOriginalSize.height);
	}

	void SetRepeat(mkF32 _x, mkF32 _y)
	{
		Rect textureRect = getTextureRect();
		textureRect.size.width = _x * m_TextureOriginalSize.width;
		textureRect.size.height = _y * m_TextureOriginalSize.height;
		setTextureRect(textureRect);
	}

	void Repeat(mkF32 _x, mkF32 _y)
	{
		Rect textureRect = getTextureRect();
		textureRect.size.width *= _x;
		textureRect.size.height *= _y;
		setTextureRect(textureRect);
	}

};

NS_MK_END

#endif // MK_SPRITE_H