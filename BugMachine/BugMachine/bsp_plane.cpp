#include "bsp_plane.h"
#include "RenderTypes.h"
//---------------------------------------------------------------------------
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

bsp_plane::bsp_plane()
{
	m_plane = new D3DXPLANE();
	_dir = new D3DXVECTOR3();
}

bsp_plane::~bsp_plane() {
	delete m_plane;
	m_plane = NULL;
}

void bsp_plane::bsp_ConstructPlane() {
	D3DXVECTOR3* point = new D3DXVECTOR3();
	D3DXVECTOR3* direction = new D3DXVECTOR3();

	point->x = 0;
	point->y = 0;
	point->z = 0;

	direction->x = 1;
	direction->y = 0;
	direction->z = 0;

	D3DXPlaneFromPointNormal(m_plane, point, direction);

	//D3DXPlaneFromPoints(m_plane,
	//D3DXPlaneNormalize(*m_plane, *m_plane);
}

void bsp_plane::bsp_ConstructPlane(D3DXVECTOR3* poss, D3DXVECTOR3* dir) {
	D3DXPlaneFromPointNormal(m_plane, poss, dir);
}

void bsp_plane::Update() {
	D3DXVECTOR3* point = new D3DXVECTOR3();	
	point->x = mesh->BV.pivot.x;
	point->y = mesh->BV.pivot.y;
	point->z = mesh->BV.pivot.z;

	D3DXPlaneFromPointNormal(m_plane, point, _dir);
}

void bsp_plane::bsp_DoBSP(Entity3D& entity, Camera* cam) {
	int resultCont = 0;
	int totalResult = 0;

	// buscar poss de la camara y revizar poss con respecto al plano
	bool camSide;
	D3DXVECTOR3* camPos = new D3DXVECTOR3(cam->posX, cam->posY, cam->posZ);
	if (D3DXPlaneDotCoord(m_plane, camPos) <= 0.0f) {
		camSide = true;
	}
	else {
		camSide = false;
	}

	// buscar poss de la entidad y comparar la poss con la de la camara con respecto al plano
	D3DXVECTOR3 aabbVertices[8] =
	{
		D3DXVECTOR3(entity.BV.ActualxMin, entity.BV.ActualyMin, entity.BV.ActualzMin),
		D3DXVECTOR3(entity.BV.ActualxMax, entity.BV.ActualyMax, entity.BV.ActualzMax),
		D3DXVECTOR3(entity.BV.ActualxMax, entity.BV.ActualyMin, entity.BV.ActualzMax),
		D3DXVECTOR3(entity.BV.ActualxMax, entity.BV.ActualyMax, entity.BV.ActualzMin),
		D3DXVECTOR3(entity.BV.ActualxMax, entity.BV.ActualyMin, entity.BV.ActualzMin),
		D3DXVECTOR3(entity.BV.ActualxMin, entity.BV.ActualyMin, entity.BV.ActualzMax),
		D3DXVECTOR3(entity.BV.ActualxMin, entity.BV.ActualyMax, entity.BV.ActualzMax),
		D3DXVECTOR3(entity.BV.ActualxMin, entity.BV.ActualyMax, entity.BV.ActualzMin)
	};
	bool side;
	int tmp = 0;
	for (int i = 0; i < 8; i++)
	{
			if (D3DXPlaneDotCoord(m_plane, &aabbVertices[i]) <= 0.0f)
			{
				side = true;
			}
			else
			{
				side = false;
			}

			if (side == camSide) {
				tmp++;
			}
	}

	if (tmp == 0) {
		// la entidad no esta del lado de la camara -> no se debería dibujar
		entity.canDraw = false;
	}
}