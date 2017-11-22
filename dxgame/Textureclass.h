#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11tex.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();
	//The first two functions will load a texture from a given file name and unload that texture when it is no longer needed.

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	//The GetTexture function returns a pointer to the texture resource so that it can be used for rendering by shaders.

	ID3D11ShaderResourceView* GetTexture();

private:
	//This is the private texture resource.

	ID3D11ShaderResourceView* m_texture;
};

#endif