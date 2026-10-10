#pragma once




enum class TextureColorSpace
{
	Linear,
	sRGB
};

enum class TextureWrap
{
	Repeat,
	MirroredRepeat,
	ClampToEdge
};

enum class TextureMagFilter
{
	Nearest,
	Linear
};

enum class TextureMinFilter
{
	Nearest,
	Linear,
	NearestMipMapNearest,
	LinearMipMapNearest,
	NearestMipMapLinear,
	LinearMipMapLinear
};


struct TextureSamplerDescription
{
	TextureMagFilter MagFilter = TextureMagFilter::Linear;
	TextureMinFilter MinFilter = TextureMinFilter::Linear;

	TextureWrap WrapU = TextureWrap::Repeat;
	TextureWrap WrapV = TextureWrap::Repeat;
};