#include "Box.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "TestScene.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

Box::Box(GameObject* parent)
	:GameObject(parent), hDrawModel(-1)
{
}

void Box::Initialize()
{
	hDrawModel = Model::Load("TONOCHABox.fbx");
}

void Box::Update()
{
}

void Box::Draw()
{
	Model::SetTransform(hDrawModel, transform_);
	Model::Draw(hDrawModel);
}

void Box::Release()
{
	Model::Release(hDrawModel);
}

void Box::SetBoxPoint(int x, int z)
{
	const float SCALE_SIZE = 3.34f;
	int pX = x - 6;
	int pZ = (z * -1) + 5;
	transform_.position_ = XMFLOAT3((pX * SCALE_SIZE) + SCALE_SIZE / 2, ((pZ * SCALE_SIZE) + SCALE_SIZE / 2) + SCALE_SIZE * 4.5,0);
}
