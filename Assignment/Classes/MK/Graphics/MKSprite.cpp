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

MKSprite* MKSprite::Create(const std::string& _fileName, bool _useTextureRepeat)
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

MKSprite* MKSprite::CreateWithTexture(Texture2D *texture, bool _useTextureRepeat)
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

MKSprite* MKSprite::CreateWithSize(const std::string& _fileName, const Size& _desiredSize, bool _useTextureRepeat)
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
	MKSprite* resultSprite = MKSprite::CreateWithTexture(renderTexture->getSprite()->getTexture(), _useTextureRepeat);

	return resultSprite;
}

NS_MK_END