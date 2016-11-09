#pragma once
#include "Mesh.h"
#include "Material.h"

using namespace DirectX; 

class Entity
{
public:
	~Entity();
	Entity(Mesh* inputMeshm, Material* inputMaterial); 
	//attributes 
	Mesh* mesh;
	Material* material; 
	
	//getters and setters
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation;  }
	XMFLOAT3 GetScale() { return this->scale; }
	XMFLOAT4X4* GetWorldMatrix() { return &worldMatrix;  }


	void setWorldMatrix(XMFLOAT4X4 newWorldMatrix) { worldMatrix = newWorldMatrix; }

	//Class Specific functions 
	void updateScene(); 
	void drawScene(ID3D11DeviceContext* deviceContext);
	void Move(float x, float y, float z) { position.x += x;	position.y += y;	position.z += z; }
	void Rotate(float x, float y, float z) { rotation.x += x;	rotation.y += y;	rotation.z += z; }
	void Scale(float x, float y, float z) { scale.x += x;	scale.y += y;	scale.z += z; }
	void prepareMaterial(XMFLOAT4X4 view, XMFLOAT4X4 proj); 
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	XMFLOAT4X4 worldMatrix;
	

};

