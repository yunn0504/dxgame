////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_Texture = 0;
	m_model = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device,char* modelFilename, WCHAR* textureFilename)
{
	bool result;

	// Initialize the vertex and index buffers.
	result = LoadModel1(modelFilename);
	if (!result)
	{
		MessageBox(NULL, L"Could not load the model object.", L"Error", MB_OK);
		return false;
	}
	result = InitializeBuffers(device);
	if(!result)
	{
		MessageBox(NULL, L"Could not buffer the model object.", L"Error", MB_OK);
		return false;
	}
	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		MessageBox(NULL, L"Could not texture the model object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	ReleaseModel();
	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}
	m_indexCount = m_vertexCount;
	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	/*ofstream f;
	f.open("debug.txt");
	f << m_vertexCount << endl;*/

	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3
		(
			position1[i].x,
			position1[i].y,
			position1[i].z
		);
		vertices[i].texture = XMFLOAT2
		(
			texture1[i].u,
			texture1[i].v
		);
		vertices[i].normal = XMFLOAT3
		(
			normal1[i].nx,
			normal1[i].ny,
			normal1[i].nz
		);
		indices[i] = i;
	}
	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	fin.open(filename);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while(input != ':')
	{ 
		fin.get(input);
	}
	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		//fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true; 
}
void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}/*
bool ModelClass::LoadModel1(char* filename)
{
	ifstream inf;
	ofstream of;
	int v = 0;
	int vt = 0;
	int ff = 0;
	char input;
	vertexTemp temp;
	textureTemp temp2;
	indexTemp temp3;

	inf.open(filename);
	of.open("cube2.txt");
	while (!inf.eof())
	{
		inf.get(input);
		if (input == 'v')
		{
			inf.get(input);
			if (input == ' ')
			{
				inf >> temp.x >> temp.y >> temp.z;
				of << v << " " << temp.x << " " << temp.y << " " << temp.z << endl;
				v++;
				position1.push_back(temp);
			}
			else if (input == 't')
			{
				inf >> temp2.u >> temp2.v;
				of << vt << " " << temp2.u << " " << temp2.v << endl;
				vt++;
				texture1.push_back(temp2);
			}
		}
		else if (input == 'f')
		{
			inf.get(input);
			if (input == ' ')
			{
				char tempchar;
				ff++;
				inf >> temp3.v >> tempchar >> temp3.t;
				temp3.v--;
				temp3.t--;
				of << ff << " " << temp3.v << " " << temp3.t << endl;
				triangle1.push_back(temp3);

				inf >> temp3.v >> tempchar >> temp3.t;
				temp3.v--;
				temp3.t--;
				of << ff << " " << temp3.v << " " << temp3.t << endl;
				triangle1.push_back(temp3);

				inf >> temp3.v >> tempchar >> temp3.t;
				temp3.v--;
				temp3.t--;
				of << ff << " " << temp3.v << " " << temp3.t << endl;
				triangle1.push_back(temp3);
				
				m_vertexCount = ff * 3;
			}
		}
	}
	of << v << " " << vt << " " << ff << " " << m_vertexCount << endl;
	of << position1.size() << " " << texture1.size() << " " << triangle1.size() << endl;
	inf.close();
	of.close();
	return 1;
}
*/
bool ModelClass::LoadModel1(char* filename)
{
	ifstream f;
	ofstream of;
	vertexTemp p1;
	textureTemp t1;
	normalTemp n1;
	f.open(filename);
	//of.open("debug1.txt");
	f >> m_vertexCount;
	//of << m_vertexCount<<"\n";
	for (int i = 0; i < m_vertexCount; i++)
	{
		f >> p1.x >> p1.y >> p1.z;
		f >> t1.u >> t1.v;
		f >> n1.nx >> n1.ny >> n1.nz;
		position1.push_back(p1);
		texture1.push_back(t1);
		normal1.push_back(n1);
	}
	/*of 
		<< position1.size() << " "
		<< texture1.size() << " "
		<< normal1.size() << " "
		<< indexlist.size() << " "
		<< m_indexCount << " "
		<< m_vertexCount <<
		endl;*/
	return true;
}