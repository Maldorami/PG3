#include "Importador.h"
#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"
#include "bsp_plane.h"
#include <stack>
#include <typeinfo>

using namespace std;

#pragma comment (lib, "assimp.lib")

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")
void getChild(aiNode& node, const aiScene& scene, Node& orkSceneRoot,Renderer& rendi);
int TotalTriangles = 0;

Importador::Importador(Renderer& rkRenderer)
{}

bool Importador::importScene(std::string fileName, Node& orkSceneRoot, Renderer& rendi){
	Assimp::Importer Importer;
	fileName = "Assets\\" + fileName;
	const aiScene* pScene = Importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	getChild(*pScene->mRootNode, *pScene, orkSceneRoot, rendi);
	totalTrianglesOnScene = TotalTriangles;
	return true;
}

void getChild(aiNode& node, const aiScene& scene, Node& orkSceneRoot, Renderer& rendi){
	for (unsigned int i = 0; i < node.mNumMeshes; i++){
		Mesh* _mesh = new Mesh(rendi);
		_mesh->setName(node.mName.C_Str());

		const aiMesh* mesh = scene.mMeshes[node.mMeshes[i]];

		unsigned short* indices = new unsigned short[mesh->mNumFaces * 3];
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}		

		CustomVertexZ* verticesT = new CustomVertexZ[mesh->mNumVertices];
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			verticesT[i].x = mesh->mVertices[i].x;
			verticesT[i].y = mesh->mVertices[i].y;
			verticesT[i].z = mesh->mVertices[i].z;
			verticesT[i].u = mesh->mTextureCoords[0][i].x;
			verticesT[i].v = mesh->mTextureCoords[0][i].y;
		}

		_mesh->_triang = mesh->mNumFaces;
		TotalTriangles += _mesh->_triang;
		_mesh->setMeshData(verticesT, Primitive::TriangleList, mesh->mNumVertices, indices, mesh->mNumFaces * 3);

		_mesh->buildBV();
		aiVector3t<float> scale;
		aiQuaterniont<float> rotation;
		aiVector3t<float> position;
		node.mTransformation.Decompose(scale, rotation, position);
		_mesh->setPos(position.x, position.y, position.z);
		_mesh->setScale(scale.x, scale.y, scale.z);
		//_mesh->setRotation(rotation.x, rotation.y, rotation.z);

		double ysqr = rotation.y * rotation.y;
		double t0 = +2.0 * (rotation.w * rotation.x + rotation.y * rotation.z);
		double t1 = +1.0 - 2.0 * (rotation.x * rotation.x + ysqr);
		double t2 = +2.0 * (rotation.w * rotation.y - rotation.z * rotation.x);
		t2 = t2 > 1.0 ? 1.0 : t2;
		t2 = t2 < -1.0 ? -1.0 : t2;
		double t3 = +2.0 * (rotation.w * rotation.z + rotation.x * rotation.y);
		double t4 = +1.0 - 2.0 * (ysqr + rotation.z * rotation.z);
		_mesh->setRotation(std::atan2(t0, t1), std::asin(t2), std::atan2(t3, t4));

		_mesh->updateBV();


		if (!_mesh->getName().find("bsp_")) {

			bsp_plane* plane = new bsp_plane();

			/*D3DXVECTOR3 v1;
			v1.x = verticesT[0].x + _mesh->posX();
			v1.y = verticesT[0].y + _mesh->posY();
			v1.z = verticesT[0].z + _mesh->posZ();
			D3DXVECTOR3 v2;
			v2.x = verticesT[1].x + _mesh->posX();
			v2.y = verticesT[1].y + _mesh->posY();
			v2.z = verticesT[1].z + _mesh->posZ();
			D3DXVECTOR3 v3;
			v3.x = verticesT[2].x + _mesh->posX();
			v3.y = verticesT[2].y + _mesh->posY();
			v3.z = verticesT[2].z + _mesh->posZ();*/

			D3DXVECTOR3* point = new D3DXVECTOR3();
			D3DXVECTOR3* direction = new D3DXVECTOR3();

			point->x = _mesh->posX();
			point->y = _mesh->posY();
			point->z = _mesh->posZ();


			if (_mesh->BV.xMin == _mesh->BV.xMax) {
				direction->x = 1;
				direction->y = 0;
				direction->z = 0;
			}
			else 
				if (_mesh->BV.zMin == _mesh->BV.zMax) {
				direction->x = 0;
				direction->y = 0;
				direction->z = 1;
			}
			else
				if (_mesh->BV.yMin == _mesh->BV.yMax) {
					direction->x = 0;
					direction->y = 1;
					direction->z = 0;
				}


			plane->bsp_ConstructPlane(point, direction);
			plane->_dir = direction;
			plane->mesh = _mesh;
			plane->Update();
			rendi.bsp_Planes.push_back(plane);
		}

		aiString path;
		if (scene.mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS){

			string pathName = path.data;
			char tmp = ' ';
			bool ready = false;
			stack<char> stackFinal;

			for (unsigned int i = pathName.size(); i != -1; i--){
				tmp = pathName[i];

				if (tmp == '/') ready = true;
				
				if (!ready){
					stackFinal.push(tmp);
				}
			}

			string pathFinal = "Assets\\";
			int sizeStackFinal = stackFinal.size();
			for (int x = 0; x < sizeStackFinal; x++){
				pathFinal += stackFinal.top();
				stackFinal.pop();
			}

			_mesh->setTextureId(rendi.loadTexture(pathFinal, D3DCOLOR_XRGB(255, 0, 255)));
		}

		orkSceneRoot.AddChild(_mesh);
		}

		if (node.mNumChildren > 0)
		{
			for (unsigned int i = 0; i < node.mNumChildren; i++)
				{
					Node* _node = new Node();
					aiVector3t<float> scale;
					aiQuaterniont<float> rotation;
					aiVector3t<float> position;
					node.mTransformation.Decompose(scale, rotation, position);
					_node->setPos(position.x, position.y, position.z);
					_node->setScale(scale.x, scale.y, scale.z);

					_node->setName(node.mName.C_Str());
					orkSceneRoot.AddChild(_node);
					getChild(*node.mChildren[i], scene, *_node, rendi);
				}
		}
}
