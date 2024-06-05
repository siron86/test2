#include "SelectScene.h"
#include"../SceneManager.h"

#include"../../Object/Camera/FreeCamera/FreeCamera.h"
#include"../../Object/Select/SelectObject.h"
#include"../../Ui/SelectUi/SelectUi.h"

void SelectScene::Init()
{
	//SceneManager::Instance().GetObjList();

	m_camera = std::make_unique<KdCamera>();
	m_camera->SetProjectionMatrix(60);

	//カメラ
	std::shared_ptr<FreeCamera> fC = std::make_shared<FreeCamera>();
	fC->SetStartPos(Math::Vector3(10.0f, 20.0f, -20.0f));
	fC->SetAngle(Math::Vector3(-15.f, -20.f, 0.f));
	m_objList.push_back(fC);

	//エフェクシアにカメラをセット
	KdEffekseerManager::GetInstance().SetCamera(fC);

	//タイトルオブジェクト
	std::shared_ptr<SelectSObject>sObj = std::make_shared<SelectSObject>();
	m_objList.push_back(sObj);

	sUi = std::make_shared<SelectUi>();
	m_objList.push_back(sUi);
	//ShowCursor(false);

}

void SelectScene::Event()
{
	if (GetAsyncKeyState('G') & 0x8000)
	{
		sUi->SetLoadFlg(true);
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		sUi->SetMode(1);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		sUi->SetMode(2);
	}
	if (sUi->GetNextFlg())
	{
		switch (sUi->GetMode())
		{
		case 1:
			//トレーニングモード用
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game, SceneManager::GameMode::Training);
			break;

		case 2:
			//ストーリーモード用
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game, SceneManager::GameMode::Story);
			break;
		}
	}
}
