attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 u_textureOffset;
uniform vec2 u_textureScale;
uniform vec2 u_textureRotationCosSin; // The X is the Cos. The Y is the Sin. The values are in radians.

void main()
{
	gl_Position = CC_PMatrix * a_position;
	v_fragmentColor = a_color;
	
	// Texture Coordinates Manipulation
	v_texCoord = a_texCoord;
	
	// Scale
	v_texCoord.x *= u_textureScale.x;
	v_texCoord.y *= u_textureScale.y;
	
	// Rotate
	v_texCoord -= vec2(0.5, 0.5);
	
	vec4 rotationMatrix = vec4(u_textureRotationCosSin.x, u_textureRotationCosSin.y, -u_textureRotationCosSin.y, u_textureRotationCosSin.x);
	vec2 rotationTexCoord = v_texCoord;
	v_texCoord.x = rotationMatrix.x * rotationTexCoord.x + rotationMatrix.z * rotationTexCoord.y;
	v_texCoord.y = rotationMatrix.y * rotationTexCoord.x + rotationMatrix.w * rotationTexCoord.y;
	
	v_texCoord += vec2(0.5, 0.5);
	
	// Translate
	v_texCoord.x += u_textureOffset.x;
	v_texCoord.y += u_textureOffset.y;
}
