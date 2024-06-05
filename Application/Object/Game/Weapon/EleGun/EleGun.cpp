#include "EleGun.h"
#include"../../Player/Player.h"
#include"../Bullet/Bullet.h"

void EleGun::Update()
{
	const KdModelWork::Node* pNode = GetModel()->FindNode("ShotPoint");
	if (pNode)
	{
		m_muzzleMat = pNode->m_worldTransform * Math::Matrix::CreateTranslation(0.f, 5.f, 0.f) * m_mWorld;
	}

	//選択されていたら発射可能に
	if (m_selectFlg)
	{
		//角度調整
		m_ang.x += 0.5f;
		if (m_ang.x >= 25)m_ang.x = 25;
		//発射
		if (m_shotFlg)
		{
			//発射していなかったら
			if (!m_fireFlg && !m_weaBase.expired())
			{
				if (!m_relFlg)
				{
					//弾数使用
					--STR_Weapon.bulletCount;

					//弾を発射
					m_weaBase.lock()->ShotBullet(m_muzzleMat, { 27.f,3.f,0.f }, 100,20);
					m_fireFlg = true;
				}
			}
		}
		//選択解除
		m_selectFlg = false;
		m_shotFlg = false;
	}
	else
	{
		//角度リセット
		m_ang.x += -0.5f;
		if (m_ang.x <= 0)m_ang.x = 0;
	}
	//クールタイム
	if (m_fireFlg)
	{
		//連射間隔
		STR_Weapon.coolTime -= 1.0f;
		if (STR_Weapon.coolTime <= 0)
		{
			STR_Weapon.coolTime = 60.0f * 1.0f;
			m_fireFlg = false;
		}
	}

	//玉無し野郎状態
	if (STR_Weapon.bulletCount <= 0)
	{
		m_relFlg = true;
	}

	//自動又は手動リロード
	if (m_relFlg)
	{
		//カウント開始
		--STR_Weapon.nowRelTime;

		//時間になったら
		if (STR_Weapon.nowRelTime <= 0)
		{
			//リロード完了
			m_relFlg = false;
			STR_Weapon.bulletCount = STR_Weapon.bulletMax;
			STR_Weapon.nowRelTime = STR_Weapon.relTime;
		}
	}

}

void EleGun::PostUpdate()
{
	m_scaleMat = Math::Matrix::CreateScale(2.5f);

	m_rotateMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_ang.x));

	m_transMat = Math::Matrix::CreateTranslation(m_pos);

	m_mWorld = m_scaleMat * m_rotateMat * m_transMat * m_rackMat;

}

void EleGun::DrawLit()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void EleGun::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Weapon/ElectromagneticGun/ElectromagneticGun2.gltf"));

	STR_Weapon.coolTime = 60.0f * 1.0f;

	//弾数
	STR_Weapon.bulletMax = 10;
	STR_Weapon.bulletCount = STR_Weapon.bulletMax;
	//リロード時間
	STR_Weapon.relTime = 60.0f * 2.5f;
	STR_Weapon.nowRelTime = STR_Weapon.relTime;
}