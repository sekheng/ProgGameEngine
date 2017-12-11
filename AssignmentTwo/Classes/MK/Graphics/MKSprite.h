#ifndef MK_SPRITE_H
#define MK_SPRITE_H

// Include Cocos
#include "cocos2d.h"

// Include STL
#include <string>
#include <cmath>

// Include MK
#include "..\Common\MKMacros.h"
#include "..\Common\MKMathsHelper.h"

USING_NS_CC;
using namespace std;

NS_MK_BEGIN

class MKSprite : public cocos2d::Sprite
{
private:
    GLProgram* m_GLProgram = nullptr;
    GLProgramState* m_GLProgramState = nullptr;

	Vec2 m_TextureOffset;
    Vec2 m_TextureScale;
    mkF32 m_TextureRotation;
	const mkBool m_UseTextureRepeat;

protected:
    static void InitGLProgram(MKSprite* _sprite);
	static void InitTextureParameters(MKSprite* _sprite);

public:
	// If _textureRepeat is true, the size MUST be a power of 2. I don't set the rules, it's OpenGL and Cocos' dumb systems.
	// Cocos stupidly making the texture size the object size is the most infuriating thing.
	static MKSprite* Create(const mkString& _fileName, mkBool _useTextureRepeat = false);
	static MKSprite* CreateWithTexture(Texture2D *texture, mkBool _useTextureRepeat = false);
	static MKSprite* CreateWithSize(const mkString& _fileName, const Size& _desiredSize, mkBool _useTextureRepeat = false);

    mkBool GetUseTextureRepeat() const
	{
		return m_UseTextureRepeat;
	}

	// The below functions only work if m_TextureRepeat is true.
	Vec2 GetTextureOffset() const
	{
		return m_TextureOffset;
	}

    void SetTextureOffset(const cocos2d::Vec2& _textureOffset)
    {
        SetTextureOffset(_textureOffset.x, _textureOffset.y);
    }

    void SetTextureOffset(mkF32 _x, mkF32 _y)
    {
        m_TextureOffset.x = _x;
        m_TextureOffset.y = _y;

        m_GLProgramState->setUniformVec2("u_textureOffset", m_TextureOffset);
        m_GLProgram->updateUniforms();
	}

	void OffsetTexture(mkF32 _x, mkF32 _y)
	{
		SetTextureOffset(m_TextureOffset.x + _x, m_TextureOffset.y + _y);
	}

    Vec2 GetTextureScale() const
    {
        return m_TextureScale;
    }

    void SetTextureScale(const cocos2d::Vec2& _textureScale)
    {
        SetTextureScale(_textureScale.x, _textureScale.y);
    }

    void SetTextureScale(mkF32 _u, mkF32 _v)
    {
        m_TextureScale.x = _u;
        m_TextureScale.y = _v;

        m_GLProgramState->setUniformVec2("u_textureScale", m_TextureScale);
        m_GLProgram->updateUniforms();
    }

    void ScaleTexture(mkF32 _u, mkF32 _v)
    {
        SetTextureScale(m_TextureScale.x + _u, m_TextureScale.y + _v);
    }

    mkF32 GetTextureRotation() const
    {
        return m_TextureRotation;
    }

    void SetTextureRotation(mkF32 _degrees)
    {
        m_TextureRotation = _degrees;

        mkF32 rotationRadians = m_TextureRotation * MKMathsHelper::Deg2Rad;
        m_GLProgramState->setUniformVec2("u_textureRotationCosSin", Vec2(cos(rotationRadians), sin(rotationRadians)));
        m_GLProgram->updateUniforms();
    }

    void RotateTexture(mkF32 _degrees)
    {
        SetTextureRotation(m_TextureRotation + _degrees);
    }

CC_CONSTRUCTOR_ACCESS:
    MKSprite(mkBool _useTextureRepeat)
        : m_UseTextureRepeat(_useTextureRepeat)
    {
        // Default Texture Offset
        m_TextureOffset.x = 0.0f;
        m_TextureOffset.y = 0.0f;

        m_TextureScale.x = 1.0f;
        m_TextureScale.y = 1.0f;

        m_TextureRotation = 0.0f;
    }
    virtual ~MKSprite() {}

};

NS_MK_END

#endif // MK_SPRITE_H