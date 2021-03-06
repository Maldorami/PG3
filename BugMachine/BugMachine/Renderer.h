#ifndef RENDERER_H
#define RENDERER_H
#include <string>
#include <vector>
#include "Mat.h"
#include <Windows.h>
#include "RenderTypes.h"


struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DVertexBuffer9;
typedef interface ID3DXFont ID3DXFont;
typedef interface ID3DXFont *LPD3DXFONT;
typedef ID3DXFont* Font;

class IndexBuffer;
class VertexBuffer;
class bsp_plane;
class Mesh;

#define DLLexport __declspec(dllexport)
class Renderer{
public:
	DLLexport void draw(CustomVertexZ* apkVertices, _D3DPRIMITIVETYPE ePrimitive, unsigned int uiVertexCount);
	DLLexport void draw(CustomVertex* apkVertices, _D3DPRIMITIVETYPE ePrimitive, unsigned int uiVertexCount);
	DLLexport const Texture loadTexture(const std::string& fileName, int colorKey);

	DLLexport Renderer(){};
	DLLexport ~Renderer();

	bool init(HWND hwnd);
	void beginFrame();
	void endFrame();
	void drawText(ID3DXFont* _Font, std::string& _text, RECT& _rect);

	void setMatrix(MatrixType matris, const Matrix& Matrix);

	void setCurrentTexture(const Texture& texture);

	IDirect3DDevice9* d3ddev;
	VertexBuffer* createVertexBuffer(size_t uiVertexSize, unsigned int uiFVF);
	IndexBuffer* createIndexBuffer();
	void setCurrentIndexBuffer(IndexBuffer* pkIndexBuffer);
	void setCurrentVertexBuffer(VertexBuffer* pkVertexBuffer);
	void drawCurrentBuffers(Primitive ePrimitive);
	float screenDepth;
	Matrix projectionMatrix;

	int totalTriangles = 0;
	int currentTrianglesRenderer = 0;

	DLLexport void AddPlane();
	DLLexport void UpdateBSPMeshes(float scaleX, float scaleY, float scaleZ);
	DLLexport void UpdatePlanes();
	std::vector<bsp_plane*> bsp_Planes;
	std::vector<Mesh*> bsp_mesh;

private:
	VertexBuffer* v_buffer;
	VertexBuffer* v_bufferS;
	IDirect3D9* d3d;
	std::vector<Texture> textures;

	VertexBuffer* _vertexBuffer;
	IndexBuffer* _indexBuffer;

};
#endif