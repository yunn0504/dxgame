#ifndef _MESHCLASS_H_
#define _MESHCLASS_H_

#include<D3D11.h>
#include<D3DX10.h>
#include<D3DX10mesh.h>


class MeshClass
{
public:
	MeshClass();
	~MeshClass();

private:
	ID3DX10Mesh *m_mesh;
};

#endif