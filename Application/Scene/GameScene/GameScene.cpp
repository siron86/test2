#include "GameScene.h"
#include"../SceneManager.h"
#include"../../Object/Camera/GameCamera/GameCamera.h"
#include"../../Object/Camera/FreeCamera/FreeCamera.h"
#include"../../Object/Game/Stage/Stage.h"
#include"../../Object/Game/Player/Player.h"
#include"../../Object/Game/Enemy/EnemyBase.h"
#include"../../Object/Game/Weapon/WeaponBase.h"
#include"../../Ui/UiBase.h"
#include"../../Object/Result/ResultObject.h"

void GameScene::Init()
{
	//フォグ(霧)
	KdShaderManager::Instance().WorkAmbientController().SetFogEnable(false, true);
	
	//距離フォグ
	KdShaderManager::Instance().WorkAmbientController().SetDistanceFog({ 1.f,1.f,1.f }, 0.0001f);
	KdShaderManager::Instance().WorkAmbientController().SetheightFog({ 1.f,1.f,1.f }, 500.f, -15.f, 2000.f);	//オブジェクトリスト

	m_camera = std::make_unique<KdCamera>();
	m_camera->SetProjectionMatrix(60);

	//プレイヤー
	std::shared_ptr<Player> pla = std::make_shared<Player>();
	pla->SetData(StageData(SceneManager::Instance().GetGameMode()));
	pla->SetScene(this);
	pla->Init();
	m_objList.push_back(pla);
	AddHitObject(pla);
	m_wpPla = pla;

	//カメラ
	std::shared_ptr<GameCamera> c = std::make_shared<GameCamera>();
	c->SetScene(this);
	c->SetTarget(pla);
	pla->SetCamera(c);
	m_objList.push_back(c);
	AddHitObject(c);
	m_wpTpsCam = c;

	//エフェクシアにカメラをセット
	KdEffekseerManager::GetInstance().SetCamera(c);

	//エネミー
	std::shared_ptr<EnemyBase>ene = std::make_shared<EnemyBase>();
	ene->SetData(StageData(SceneManager::Instance().GetGameMode()));
	ene->SetPlayer(pla);
	pla->SetTarget(ene);
	ene->SetScene(this);
	ene->SetCamera(c);
	ene->Init();
	m_objList.push_back(ene);
	m_wpEne = ene;

	//ステージ情報
	std::shared_ptr<Stage>stage = std::make_shared<Stage>();
	stage->SetScene(this);
	stage->Init();
	m_objList.push_back(stage);

	//Ui
	std::shared_ptr<UiBase> ui = std::make_shared<UiBase>();
	ui->GetPlayer(pla);
	ui->Init();
	m_objList.push_back(ui);
	m_wpUi = ui;

	//Ui
	std::shared_ptr<ResultObject> reOb = std::make_shared<ResultObject>();
	m_objList.push_back(reOb);
	m_wpReOb = reOb;

	ShowCursor(false);
	//KdAudioManager::Instance().SoundReset();
	//KdAudioManager::Instance().Play("Asset/BGM/GameBgm.wav", true);

	//マスターボリュームの調整はできそう
	//KdAudioManager::Instance().GetAudioEngine()->SetMasterVolume(1);
}

void GameScene::Event()
{
	if (GetAsyncKeyState('P'))
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}
	if (GetAsyncKeyState('O'))
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Select);
	}
	if (m_wpEne.lock()->EndFlg())
	{
		m_wpReOb.lock()->PlayResultFlg(true);
		m_wpPla.lock()->SetControl(false);
		m_wpEne.lock()->SetControl(false);
		m_wpUi.lock()->PopControl(false);
	}
	//フリーカメラへ切り替え
	//テスト：TPSカメラ切り
	if (GetAsyncKeyState('L'))
	{
		m_wpTpsCam.lock()->FreeCamera(true);
		m_wpPla.lock()->SetControl(false);
		m_wpEne.lock()->SetControl(false);
		m_wpUi.lock() ->PopControl(false);
	}

	//被写界深度
	m_camera->SetFocus(5, 0.5f, 5);

}