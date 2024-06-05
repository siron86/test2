#include "PlayerUi.h"
#include"../../Object/Game/Player/Player.h"

void PlayerUi::Update()
{
	//プレイヤーの情報を元にUiを変化させる
	if (m_wpPla.expired())return;
	std::shared_ptr<Player>pla = m_wpPla.lock();
	std::shared_ptr<PLAYERDATA> pData = pla->GetPlayerData();
	//現在のHP情報をもらう
	m_hp = pData->HpData;

	//前フレームの値と比べて変化していたら、強調する
	//前フレームの値
	float nowBoost = m_boost;

	//現在の値
	m_boost = pla->GetPlayerData()->BoostData;

	//前と今を比べる
	if (m_boost != nowBoost)
	{
		m_boostCol = kWhiteColor;
	}
	else m_boostCol = { 1.0f,1.0f,1.0f,0.4f };
}

void PlayerUi::PostUpdate()
{
}

void PlayerUi::DrawSprite()
{
	//設定用変数
	//Hpバー枠
	Math::Rectangle rc = { 0,0,450,100 };
	Math::Color     color = { 1.0f,1.0f,1.0f,1.0f };
	Math::Vector2   pivot = { 0.0f,0.5f };
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_hpFrameTex.get(), -640, 335, 450, 50, &rc, &color, pivot);
	
	rc = { 0,0,m_hp,100 };
	color = { 1.0f,1.0f,1.0f,1.0f };
	pivot = { 0.0f,0.5f };
	//Hpバー描画
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_hpTex.get(), -640, 335, m_hp, 50, &rc, &color, pivot);

	//ブーストゲージ
	rc = { 0,0,(int)m_boost,100 };
	pivot = { 0.0f,0.5f };
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_boostTex.get(), -275, -300, (int)m_boost, 50, &rc, &m_boostCol, pivot);

	//ブーストゲージ枠
	rc = { 0,0,550,100 };
	color = { 1.0f,1.0f,1.0f,0.7f };
	pivot = { 0.0f,0.5f };
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_boostFrameTex.get(), -275, -300, 550, 50, &rc, &color, pivot);

	//中心点
	rc = { 0,0,100,100 };
	color = { 1.0f,1.0f,1.0f,0.5f };
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_shotTex.get(), 0, 0, 80, 80, &rc, &color);

	//ロックオン範囲
	rc = { 0,0,300,300 };
	color = { 1.0f,1.0f,1.0f,0.5f };
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_lockTex.get(), 0, 0, 500, 500, &rc, &color);
}

void PlayerUi::Init()
{
	m_hpTex=KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Player/Hp.png");

	m_hpFrameTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Player/HPFrame.png");

	m_boostTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Player/BoostGage.png");

	m_boostFrameTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Player/BoostFrame.png");

	m_shotTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Player/ShotUi.png");

	m_lockTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Player/LockCam.png");
}

void PlayerUi::SetTexData()
{
}