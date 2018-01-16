////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
		XMFLOAT3 normal;
	};
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	struct vertexTemp
	{
		float x, y, z;
	};
	struct textureTemp
	{
		float u, v;
	};
	struct normalTemp
	{
		float nx, ny,nz;
	};
	struct SkinVertex
	{
		XMFLOAT3 p;
		XMFLOAT3 n;
		XMFLOAT2 uv;
		int index[4];
		float weight[3];
	};
	
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*,char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	bool LoadModel1(char*);
	void ReleaseModel();
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
	ModelType* m_model;

	vector<vertexTemp> position1;
	vector<textureTemp> texture1;
	vector<normalTemp> normal1;
	vector<int> indexlist;
};

#endif