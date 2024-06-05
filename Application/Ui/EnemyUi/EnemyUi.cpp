#include "EnemyUi.h"
#include"../../Object/Game/Enemy/NormalEnemy/NormalEnemy.h"

void EnemyUi::Update()
{
	//プレイヤーの情報を元にUiを変化させる
	if (m_wpEne.expired())return;
	std::shared_ptr<NormalEnemy>ene = m_wpEne.lock();

	//現在のHP情報をもらう
	//前フレームの値
	int nowHp = m_hp;

	//前フレームの値と比べて変化していたら、強調する
	if (nowHp != m_hp)
	{
		m_hitFlg = true;
	}
	else m_hitFlg=false;
}

void EnemyUi::PostUpdate()
{
}

void EnemyUi::DrawSprite()
{
	//設定用変数
	Math::Rectangle rc = { 0,0,100,100 };
	Math::Color     color = { 1.0f,1.0f,1.0f,1.0f };
	//Hpバー描画
	//if (m_hitFlg)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_hitTex, 0, -200, 100, 100, &rc, &color);
	}
}

void EnemyUi::Init()
{
	m_hitTex = new KdTexture;
	m_hitTex->Load("Asset/Textures/Ui/Enemy/Hit.png");

}

void EnemyUi::SetTexData()
{
}

void EnemyUi::Release()
{
	m_hitTex->Release();
	delete m_hitTex;
}
