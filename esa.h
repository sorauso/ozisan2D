#pragma once
#include "Engine/GameObject.h"

enum ESA_TYPE
{
	ESA_TYPE_NORMAL,
	ESA_TYPE_POWER,
	ESA_TYPE_MAX
};


class Esa :
	public GameObject
{
	ESA_TYPE myType_;
	int hDrawModel;
	float counter_;
	XMFLOAT3 setPos_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Esa(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
	void SetEsaPoint(int x, int z);
	void SetEsaType(ESA_TYPE by);
	void OnCollision(GameObject* terget) override;
};

