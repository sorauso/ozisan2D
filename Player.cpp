#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Ground.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Collider.h"

namespace
{
	enum PLAYER_DIRECTION
	{
		P_LEFT,
		P_RIGHT,
		P_UP,
		P_DOWN,
		P_DIRECTION_MAX
	};
	enum PLAYER_STATE
	{
		P_STAND,
		P_WALKING,
		P_TURN,
		P_STATE_MAX
	};
	enum CAMERA_ANGLE
	{
		C_LEFT,
		C_RIGHT,
		C_UP,
		C_DOWN,
		C_DIRECTION_MAX
	};
	XMVECTOR PLAYER_MOVE[PLAYER_DIRECTION::P_DIRECTION_MAX] = {
		XMVectorSet(-1,0,0,0),
		XMVectorSet(1,0,0,0),
		XMVectorSet(0,0,1,0),
		XMVectorSet(0,0,-1,0)
	};
	float PLAYER_ANGLE[PLAYER_DIRECTION::P_DIRECTION_MAX] = { 90,270,180,0 };
	XMFLOAT3 CAMERA_POSITION_BACE = XMFLOAT3(25, 20, 0);
	XMFLOAT3 CAMERA_POSITION[CAMERA_ANGLE::C_DIRECTION_MAX] = {
		XMFLOAT3(-CAMERA_POSITION_BACE.x,CAMERA_POSITION_BACE.y,0),
		XMFLOAT3(CAMERA_POSITION_BACE.x,CAMERA_POSITION_BACE.y,0),
		XMFLOAT3(0,CAMERA_POSITION_BACE.y,CAMERA_POSITION_BACE.x),
		XMFLOAT3(0,CAMERA_POSITION_BACE.y,-CAMERA_POSITION_BACE.x)
	};

	PLAYER_DIRECTION direction = PLAYER_DIRECTION::P_UP;
	PLAYER_STATE state = PLAYER_STATE::P_STAND;
	CAMERA_ANGLE cAngle = CAMERA_ANGLE::C_DOWN;

	float rot;

	const int KeyID[4][4] = {
		{ DIK_S, DIK_W, DIK_A, DIK_D },
		{ DIK_W, DIK_S, DIK_D, DIK_A },
		{ DIK_D, DIK_A, DIK_S, DIK_W },
		{ DIK_A, DIK_D, DIK_W, DIK_S }
	};
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"), hDrawModel(-1), hWokingModel(-1), hStandModel(-1) {
	//swordDirには、初期方向として、ローカルモデルの剣の根っこから
	//先端までのベクトルとして（0,1,0)を代入しておく
	//初期位置は原点
}

void Player::Initialize()
{
	hWokingModel = Model::Load("Walkinging.fbx");
	Model::SetAnimFrame(hWokingModel, 0, 59, 1.0);

	hStandModel = Model::Load("StandingIdle.fbx");
	Model::SetAnimFrame(hStandModel, 0, 59, 1.0);

	hDrawModel = hStandModel;
	counter = 0;

	XMFLOAT3 teagetPos = XMFLOAT3(0, 0, 0);
	XMFLOAT3 CameraPos = CAMERA_POSITION[cAngle];
	Camera::SetTarget(teagetPos);
	Camera::SetPosition(CameraPos);
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
	AddCollider(collision);
}

void Player::Update()
{
	XMVECTOR pos = XMLoadFloat3(&transform_.position_);
	XMVECTOR move = XMVectorSet(0,0,0,0);
	const float SPEED = 0.07f;
	const int ROTATION_TIME = 30;
	float angle = 0;
	PLAYER_DIRECTION oldDir = direction;

	if (state != PLAYER_STATE::P_TURN)
	{
		state = PLAYER_STATE::P_STAND;
		if (Input::IsKey(KeyID[(int)cAngle][0]))
		{
			direction = PLAYER_DIRECTION::P_LEFT;
			state = PLAYER_STATE::P_WALKING;

		}
		if (Input::IsKey(KeyID[(int)cAngle][1]))
		{
			direction = PLAYER_DIRECTION::P_RIGHT;
			state = PLAYER_STATE::P_WALKING;

		}
		if (Input::IsKey(KeyID[(int)cAngle][2]))
		{
			direction = PLAYER_DIRECTION::P_UP;
			state = PLAYER_STATE::P_WALKING;

		}
		if (Input::IsKey(KeyID[(int)cAngle][3]))
		{
			direction = PLAYER_DIRECTION::P_DOWN;
			state = PLAYER_STATE::P_WALKING;

		}
		if (oldDir != direction)
		{
			state = PLAYER_STATE::P_TURN;
			angle = PLAYER_ANGLE[direction];
		}
	}
	if (state == PLAYER_STATE::P_STAND)
	{
		angle = PLAYER_ANGLE[direction];
	}
	if (state == PLAYER_STATE::P_WALKING)
	{
		angle = PLAYER_ANGLE[direction];
		move += PLAYER_MOVE[direction];
	}
	else if (state == PLAYER_STATE::P_TURN)
	{
		if (counter == 0)
		{
			float oldAngle = PLAYER_ANGLE[oldDir];
			rot = angle - oldAngle;
			if (rot >= 180)
			{
				rot -= 360;
			}
			else if (rot <= -180)
			{
				rot += 360;
			}
			rot = rot / ROTATION_TIME;
		}
		angle = transform_.rotate_.y + rot;
		counter++;
		if (counter > ROTATION_TIME)
		{
			state = PLAYER_STATE::P_STAND;
			counter = 0;
		}
	}
	pos = pos + SPEED * move;
	Ground* ts = (Ground*)this->GetParent();
	XMFLOAT3 pPos;
	XMStoreFloat3(&pPos, pos);
	if (ts->IsPositionInBox(pPos) != 1)
	{
		XMStoreFloat3(&transform_.position_, pos);
	}
	transform_.rotate_.y = angle;


	CameraMove();
}

void Player::Draw()
{
	//transform_.scale_ = { 0.01,0.01,0.01 };
	//transform_.position_ = { 0, 0.0, 0 };
	if (state == PLAYER_STATE::P_STAND) { hDrawModel = hStandModel; }
	if (state == PLAYER_STATE::P_WALKING) { hDrawModel = hWokingModel; }
	if (state == PLAYER_STATE::P_TURN) { hDrawModel = hWokingModel; }
	Model::SetTransform(hDrawModel, transform_);
	Model::Draw(hDrawModel);
}


void Player::Release()
{
	Model::Release(hDrawModel);
}

void Player::CameraMove()
{
	if (Input::IsKeyDown(DIK_Q))
	{
		if (cAngle == CAMERA_ANGLE::C_DOWN)
		{
			cAngle = CAMERA_ANGLE::C_LEFT;
		}
		else if (cAngle == CAMERA_ANGLE::C_LEFT)
		{
			cAngle = CAMERA_ANGLE::C_UP;
		}
		else if (cAngle == CAMERA_ANGLE::C_UP)
		{
			cAngle = CAMERA_ANGLE::C_RIGHT;
		}
		else if (cAngle == CAMERA_ANGLE::C_RIGHT)
		{
			cAngle = CAMERA_ANGLE::C_DOWN;
		}
	}
	else if (Input::IsKeyDown(DIK_E))
	{
		if (cAngle == CAMERA_ANGLE::C_DOWN)
		{
			cAngle = CAMERA_ANGLE::C_RIGHT;
		}
		else if (cAngle == CAMERA_ANGLE::C_RIGHT)
		{
			cAngle = CAMERA_ANGLE::C_UP;
		}
		else if (cAngle == CAMERA_ANGLE::C_UP)
		{
			cAngle = CAMERA_ANGLE::C_LEFT;
		}
		else if (cAngle == CAMERA_ANGLE::C_LEFT)
		{
			cAngle = CAMERA_ANGLE::C_DOWN;
		}
	}

	//Camera::SetPosition(CAMERA_POSITION[cAngle]);
	XMFLOAT3 CameraPos = transform_.position_;
	Camera::SetPosition(XMFLOAT3(CameraPos.x, CameraPos.y + 10, CameraPos.z - 20));
	Camera::SetTarget(transform_.position_);
}

