// Include MK
#include "MKSprite.h"

NS_MK_BEGIN

void MKSprite::InitGLProgram(MKSprite* _sprite)
{
    // Create GLProgram
    _sprite->m_GLProgram = new GLProgram();
    _sprite->m_GLProgram->initWithFilenames("Shaders/MKShader_Default.vsh", "Shaders/MKShader_Default.fsh");
    _sprite->m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    _sprite->m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    _sprite->m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
    //_sprite->m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD1, GLProgram::VERTEX_ATTRIB_TEX_COORD1);
    //_sprite->m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD2, GLProgram::VERTEX_ATTRIB_TEX_COORD2);
    //_sprite->m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD3, GLProgram::VERTEX_ATTRIB_TEX_COORD3);
    //_sprite->m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_NORMAL, GLProgram::VERTEX_ATTRIB_NORMAL);
    //_sprite->m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_BLEND_WEIGHT, GLProgram::VERTEX_ATTRIB_BLEND_WEIGHT);
    //_sprite->m_GLProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_BLEND_INDEX, GLProgram::VERTEX_ATTRIB_BLEND_INDEX);
    _sprite->m_GLProgram->link();
    _sprite->m_GLProgram->updateUniforms();

    // Create GLProgramState
    _sprite->m_GLProgramState = GLProgramState::getOrCreateWithGLProgram(_sprite->m_GLProgram);
    _sprite->setGLProgram(_sprite->m_GLProgram);
    _sprite->setGLProgramState(_sprite->m_GLProgramState);

    // Update Uniforms
    _sprite->SetTextureOffset(_sprite->m_TextureOffset);
    _sprite->SetTextureScale(_sprite->m_TextureScale);
    _sprite->SetTextureRotation(_sprite->m_TextureRotation);
    _sprite->m_GLProgram->updateUniforms();
}

void MKSprite::InitTextureParameters(MKSprite* _sprite)
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

MKSprite* MKSprite::Create(const mkString& _fileName, mkBool _useTextureRepeat)
{
    MKSprite *sprite = new (std::nothrow) MKSprite(_useTextureRepeat);
    if (sprite && sprite->initWithFile(_fileName))
    {
        sprite->autorelease();
        InitGLProgram(sprite);
        InitTextureParameters(sprite);
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return nullptr;
}

MKSprite* MKSprite::CreateWithTexture(Texture2D *texture, mkBool _useTextureRepeat)
{
	MKSprite *sprite = new (std::nothrow) MKSprite(_useTextureRepeat);
	if (sprite && sprite->initWithTexture(texture))
	{
		sprite->autorelease();
        InitGLProgram(sprite);
		InitTextureParameters(sprite);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

NS_MK_END