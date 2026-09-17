#include "TestScene.h"
#include "Player.h"
#include "Ground.h"
#include "Box.h"
#include "Engine/Input.h"
#include "Engine/sceneManager.h"


//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{	


	//pWp = Instantiate<Weapon>(this);
	Instantiate<Ground>(this);

	pText_ = new Text;
	pText_->Initialize();

}

//更新
void TestScene::Update()
{
	if (esaNoKazu_ <= 0)
	{
		SceneManager* sm = (SceneManager*)this->GetParent();
		sm->ChangeScene(SCENE_ID_RESULT);
	}
}

//描画
void TestScene::Draw()
{
	char drawText[255];
	sprintf_s(drawText,"score:%d esa:%d",score_,esaNoKazu_);
	pText_->Draw(20, 20, drawText);
}

//開放
void TestScene::Release()
{
	//pText_->Release();
}