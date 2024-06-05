#include "TitleScene.h"
#include"../SceneManager.h"

#include"../../Object/Camera/FreeCamera/FreeCamera.h"
#include"../../Object/Title/TitleObject.h"
#include"../../Ui/TitleUi/TitleUi.h"

void TitleScene::Init()
{
	//KdAudioManager::Instance().Play("Asset/BGM/TitleBgm.wav", true);

	m_camera = std::make_unique<KdCamera>();
	m_camera->SetProjectionMatrix(60);

	//カメラ
	std::shared_ptr<FreeCamera> fC = std::make_shared<FreeCamera>();
	fC->SetStartPos(Math::Vector3(0.0f, 25.0f, 15.0f));
	fC->SetAngle(Math::Vector3(0.f, 180.f, 0.f));
	m_objList.push_back(fC);

	//エフェクシアにカメラをセット
	KdEffekseerManager::GetInstance().SetCamera(fC);

	//タイトルオブジェクト
	std::shared_ptr<TitleObject>tObj = std::make_shared<TitleObject>();
	m_objList.push_back(tObj);

	tUi = std::make_shared<TitleUi>();

	m_objList.push_back(tUi);
	//ShowCursor(false);
}

void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		tUi->SetLoadFlg(true);
	}

	if (tUi->GetNextFlg() == true)
	{
		//外部からは次の場面を引数で入れることしかできない
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Select);
	}
}
