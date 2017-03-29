#include"Renderer.h"
#include "pg2_vertexbuffer.h"
#include "pg2_indexbuffer.h"
//---------------------------------------------------------------------------
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

D3DPRIMITIVETYPE primitiveTypes[Primitive::PrimitiveCount] = {
	D3DPT_TRIANGLELIST,
	D3DPT_TRIANGLESTRIP,
	D3DPT_POINTLIST,
	D3DPT_LINELIST,
	D3DPT_LINESTRIP,
	D3DPT_TRIANGLEFAN };
//---------------------------------------------------------------------------
bool Renderer::init(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	if (hWnd == NULL)
		return false;
	
	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DVIEWPORT9 viewPort;
	d3ddev->GetViewport(&viewPort);

	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);


	float viewPortWidth = static_cast<float> (viewPort.Width);
	float viewPortHeight = static_cast<float> (viewPort.Height);

	screenDepth = 100000.0f;
	projectionMatrix = new D3DXMATRIX();
	D3DXMatrixPerspectiveFovLH(projectionMatrix, (float)(M_PI * 0.25), viewPortWidth / viewPortHeight, 1.0f, screenDepth);
	d3ddev->SetTransform(D3DTS_PROJECTION, projectionMatrix);

	return true;	

	delete projectionMatrix;
	projectionMatrix = NULL;
}
//---------------------------------------------------------------------------
void Renderer::draw(CustomVertexZ* apkVertices, _D3DPRIMITIVETYPE ePrimitive, unsigned int uiVertexCount)
{
	//v_bufferS->bind();
	//v_bufferS->draw(apkVertices, ePrimitive, uiVertexCount);
}
//---------------------------------------------------------------------------
void Renderer::draw(CustomVertex* apkVertices, _D3DPRIMITIVETYPE ePrimitive, unsigned int uiVertexCount)
{
	//_vertexBuffer->bind();
	//_vertexBuffer->draw(apkVertices, ePrimitive, uiVertexCount);
}
//---------------------------------------------------------------------------
VertexBuffer* Renderer::createVertexBuffer(size_t uiVertexSize, unsigned int uiFVF){
	VertexBuffer* vertexB = new VertexBuffer(*this, d3ddev, uiVertexSize, uiFVF);
	return vertexB;
}
//---------------------------------------------------------------------------
IndexBuffer* Renderer::createIndexBuffer(){
	IndexBuffer* indexB = new IndexBuffer(*this, d3ddev);
	return indexB;
}
//---------------------------------------------------------------------------
void Renderer::setCurrentIndexBuffer(IndexBuffer* pkIndexBuffer){
	_indexBuffer = pkIndexBuffer;
}
//---------------------------------------------------------------------------
void Renderer::setCurrentVertexBuffer(VertexBuffer* pkVertexBuffer){
	_vertexBuffer = pkVertexBuffer;
}
//---------------------------------------------------------------------------
void Renderer::drawCurrentBuffers(Primitive ePrimitive){
	_indexBuffer->bind();
	_vertexBuffer->bind();

	d3ddev->DrawIndexedPrimitive(primitiveTypes[ePrimitive], 0, 0, _vertexBuffer->vertexCount(), 0, _indexBuffer->indexCount() / 3);
}
//---------------------------------------------------------------------------
void Renderer::beginFrame(){
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene();
}
//---------------------------------------------------------------------------
void Renderer::endFrame(){
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}
//---------------------------------------------------------------------------
Renderer::~Renderer()
{
	d3ddev->Release();
	d3d->Release();

	delete v_buffer;
	delete v_bufferS;

	v_buffer = NULL;
	v_bufferS = NULL;

	for (std::vector<Texture>::iterator it = textures.begin(); it != textures.end(); it++){
		(*it)->Release();
		(*it) = NULL;
	}

	textures.clear();
}
//---------------------------------------------------------------------------
void Renderer::setMatrix(MatrixType matris, const Matrix& Matrix){
	d3ddev->SetTransform(D3DTS_WORLD, Matrix);
}
//---------------------------------------------------------------------------
const Texture Renderer::loadTexture(const std::string& fileName, int colorKey){
	Texture texture = NULL;

	HRESULT hr = D3DXCreateTextureFromFileEx(d3ddev,
		fileName.c_str(),
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
		0, 0,	D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, D3DX_FILTER_NONE,
		colorKey,
		NULL,
		NULL,
		&texture);

	if (hr != D3D_OK){
		return NoTexture;
	}
	else{		
		textures.push_back(texture);
		return texture;

	}
}
//---------------------------------------------------------------------------
void Renderer::setCurrentTexture(const Texture& texture){
	d3ddev->SetTexture(0, texture);
}
//---------------------------------------------------------------------------
void Renderer::drawText(ID3DXFont* _Font, std::string& _text, RECT& _rect){
	_Font->DrawTextA(NULL, _text.c_str(), -1, &_rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
}
//---------------------------------------------------------------------------