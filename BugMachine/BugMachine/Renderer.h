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

private:
	VertexBuffer* v_buffer;
	VertexBuffer* v_bufferS;
	IDirect3D9* d3d;
	std::vector<Texture> textures;

	VertexBuffer* _vertexBuffer;
	IndexBuffer* _indexBuffer;

};
#endif