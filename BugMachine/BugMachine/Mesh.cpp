#include "Mesh.h"
#include "pg2_indexbuffer.h"
#include "pg2_vertexbuffer.h"

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

Mesh::Mesh(Renderer& rkRenderer)
:
indexB(NULL),
vertexB(NULL),
_vertices(NULL),
rendi(rkRenderer)
{}
using namespace std;

void Mesh::getChild(std::string name, Entity3D& child){
	if (name == getName())
	{
		child = *this;
	}
}

void Mesh::updateBV(){

	D3DXVECTOR3* wordScale = new D3DXVECTOR3();
	D3DXQUATERNION* wordRotation = new D3DXQUATERNION();
	D3DXVECTOR3* wordTranslation = new D3DXVECTOR3();
	D3DXMatrixDecompose(wordScale, wordRotation, wordTranslation, _WordtransformationMatrix);

	BV.pivot.x = wordTranslation->x;
	BV.pivot.y = wordTranslation->y;
	BV.pivot.z = wordTranslation->z;

	BV.ActualxMax = (BV.xMax * wordScale->x) + wordTranslation->x;
	BV.ActualxMin = (BV.xMin * wordScale->x) + wordTranslation->x;

	BV.ActualyMax = (BV.yMax * wordScale->y) + wordTranslation->y;
	BV.ActualyMin = (BV.yMin * wordScale->y) + wordTranslation->y;

	BV.ActualzMax = (BV.zMax * wordScale->z) + wordTranslation->z;
	BV.ActualzMin = (BV.zMin * wordScale->z) + wordTranslation->z;

	BV.width = BV.ActualxMax - BV.ActualxMin;
	BV.height = BV.ActualyMax - BV.ActualyMin;
	BV.depth = BV.ActualzMax - BV.ActualzMin;

	delete wordRotation;
	delete wordScale;
	delete wordTranslation;
}

void Mesh::buildBV(){

	////AABB Values
	float xMin = 0, xMax = 0;
	float yMin = 0, yMax = 0;
	float zMin = 0, zMax = 0;
	xMin = xMax = _vertices[0].x;
	yMin = yMax = _vertices[0].y;
	zMin = zMax = _vertices[0].z;

	for (unsigned int i = 0; i < vertexB->vertexCount(); i++)
	{
		if (_vertices[i].x < xMin)
			xMin = _vertices[i].x;
		if (_vertices[i].x > xMax)
			xMax = _vertices[i].x;

		if (_vertices[i].y < yMin)
			yMin = _vertices[i].y;
		if (_vertices[i].y > yMax)
			yMax = _vertices[i].y;

		if (_vertices[i].z < zMin)
			zMin = _vertices[i].z;
		if (_vertices[i].z > zMax)
			zMax = _vertices[i].z;
	}

	BV.pivot.x = xMax - ((xMax - xMin) / 2);
	BV.pivot.y = yMax - ((yMax - yMin) / 2);
	BV.pivot.z = zMax - ((zMax - zMin) / 2);
	BV.width = xMax - xMin;
	BV.height = yMax - yMin;
	BV.depth = zMax - zMin;
	BV.xMax = xMax;
	BV.xMin = xMin;
	BV.yMax = yMax;
	BV.yMin = yMin;
	BV.zMax = zMax;
	BV.zMin = zMin;
}


void Mesh::setMeshData(const CustomVertex* pakVertices, Primitive ePrimitive,
	size_t uiVertexCount, const unsigned short* pusIndices,
	size_t uiIndexCount)
{
	vertexB = rendi.createVertexBuffer(sizeof(CustomVertex), CUSTOMFVF);
	indexB = rendi.createIndexBuffer();

	vertexB->setVertexData(pakVertices, uiVertexCount);
	indexB->setIndexData(pusIndices, uiIndexCount);

	rendi.setCurrentVertexBuffer(vertexB);
	rendi.setCurrentIndexBuffer(indexB);

	primitive = ePrimitive;
}

void Mesh::setMeshData(const CustomVertexZ* pakVertices, Primitive ePrimitive,
	size_t uiVertexCount, const unsigned short* pusIndices,
	size_t uiIndexCount)
{
	_vertices = pakVertices;

	vertexB = rendi.createVertexBuffer(sizeof(CustomVertexZ), CUSTOMFVFT);
	indexB = rendi.createIndexBuffer();

	vertexB->setVertexData(pakVertices, uiVertexCount);
	indexB->setIndexData(pusIndices, uiIndexCount);

	rendi.setCurrentVertexBuffer(vertexB);
	rendi.setCurrentIndexBuffer(indexB);

	primitive = ePrimitive;
}

void Mesh::draw(Renderer& rkRenderer, CollisionResult eParentResult, Frustum& rkFrustum){
	if (eParentResult != AllOutside)
	{
		rendi.setCurrentTexture(_texture);
		rendi.setMatrix(MatrixType::World, _WordtransformationMatrix);
		rendi.drawCurrentBuffers(primitive);
	}
}

void Mesh::draw(Renderer& rkRenderer, CollisionResult eParentResult, Frustum& rkFrustum, Text& _text){

	if (eParentResult != AllOutside)
	{
		rendi.setCurrentTexture(_texture);
		rendi.setMatrix(MatrixType::World, _WordtransformationMatrix);
		rendi.drawCurrentBuffers(primitive);

		_text.setText(_text._text + "\n   +" + getName());
	}
}

void Mesh::setTextureId(Texture texturrah){
	_texture = texturrah;
}

Mesh::~Mesh(){
	delete indexB;
	delete vertexB;

	indexB = NULL;
	vertexB = NULL;
}