#ifndef MESH_H
#define MESH_H

#include "Entity3D.h"

class Mesh : public Entity3D{

	// constructor
public:
	DLLexport Mesh(Renderer& rkRenderer);
	DLLexport ~Mesh();

	// rendering
	DLLexport void setMeshData(const CustomVertex* pakVertices, Primitive ePrimitive,
					size_t uiVertexCount, const unsigned short* pusIndices,
					size_t uiIndexCount);

	DLLexport void setMeshData(const CustomVertexZ* pakVertices, Primitive ePrimitive,
		size_t uiVertexCount, const unsigned short* pusIndices,
		size_t uiIndexCount);

	DLLexport void draw(Renderer& rkRenderer, CollisionResult eParentResult, Frustum& rkFrustum);
	DLLexport void draw(Renderer& rkRenderer, CollisionResult eParentResult, Frustum& rkFrustum, Text& _text);
	DLLexport void setTextureId(Texture _texture);

	DLLexport void updateBV();
	DLLexport void buildBV();

	DLLexport void getChild(std::string name, Entity3D& child);

	IndexBuffer* indexB;
	VertexBuffer* vertexB;


private:
	Renderer rendi;
	Primitive primitive;
	Texture _texture;

	bool drawBV = true;
	const CustomVertexZ* _vertices;
};
#endif