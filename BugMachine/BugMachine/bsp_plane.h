#ifndef BSP_PLANE_H
#define BSP_PLANE_H

#define DLLexport __declspec(dllexport)

#include "Renderer.h"
#include "Camera.h"
#include "Entity3D.h"

class Entity3D;
class D3DXVECTOR3;

class bsp_plane {

public:
	DLLexport bsp_plane();
	DLLexport ~bsp_plane();
	DLLexport void bsp_ConstructPlane();
	DLLexport void bsp_ConstructPlane(D3DXVECTOR3* poss, D3DXVECTOR3* dir);
	DLLexport void bsp_ConstructPlane(D3DXVECTOR3* v1, D3DXVECTOR3* v2, D3DXVECTOR3* v3);
	DLLexport void bsp_DoBSP(Entity3D& aabb, Camera* cam);

	Entity3D* mesh;
	D3DXVECTOR3* _dir;


	Plane m_plane;

};
#endif