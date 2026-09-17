#include "esa.h"
#include "Engine/Model.h"
#include "Engine/Collider.h"
#include "TestScene.h"

Esa::Esa(GameObject* parent)
	:GameObject(parent), hDrawModel(-1),myType_(ESA_TYPE_NORMAL),counter_(-1), setPos_(0,0,0)
{
}

void Esa::Initialize()
{
	hDrawModel = Model::Load("esa.fbx");
	transform_.scale_ = XMFLOAT3(0.5f, 0.5f, 0.5f);
	counter_ = (float)(rand() % 100) / 100;
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
	AddCollider(collision);
}

void Esa::Update()
{
	counter_+= 0.1f;
	transform_.position_.y = setPos_.y + (sinf(counter_) / 2) + 1;
	if (myType_ == ESA_TYPE_POWER)
	{
		transform_.rotate_.y += setPos_.y + 1;
	}
}

void Esa::Draw()
{
	Model::SetTransform(hDrawModel, transform_);
	Model::Draw(hDrawModel);
}

void Esa::Release()
{
	Model::Release(hDrawModel);
	TestScene* tsesce = (TestScene*)this->GetParent()->GetParent();
	if (tsesce == nullptr)
	{
		return;
	}
	tsesce->DownEsaNoKazu();
	if (myType_ == ESA_TYPE_POWER)
	{
		tsesce->UpScore(10);
	}
	else
	{
		tsesce->UpScore(5);
	}
}

void Esa::SetEsaPoint(int x, int z)
{
	const float SCALE_SIZE = 3.34f;
	int pX = x - 6;
	int pZ = (z * -1) + 5;
	setPos_ = XMFLOAT3((pX * SCALE_SIZE) + SCALE_SIZE / 2, ((pZ * SCALE_SIZE) + SCALE_SIZE / 2) + SCALE_SIZE*4.5,0);
	transform_.position_ = setPos_;
}

void Esa::SetEsaType(ESA_TYPE by)
{
	myType_ = by;
	if (myType_ == ESA_TYPE_POWER)
	{
		transform_.scale_ = XMFLOAT3(1, 1, 1);
	}
}

void Esa::OnCollision(GameObject* terget)
{
	if (terget->GetObjectName() == "Player")
	{
		KillMe();
	}
}
