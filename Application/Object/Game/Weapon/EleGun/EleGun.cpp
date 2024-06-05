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

	//�I������Ă����甭�ˉ\��
	if (m_selectFlg)
	{
		//�p�x����
		m_ang.x += 0.5f;
		if (m_ang.x >= 25)m_ang.x = 25;
		//����
		if (m_shotFlg)
		{
			//���˂��Ă��Ȃ�������
			if (!m_fireFlg && !m_weaBase.expired())
			{
				if (!m_relFlg)
				{
					//�e���g�p
					--STR_Weapon.bulletCount;

					//�e�𔭎�
					m_weaBase.lock()->ShotBullet(m_muzzleMat, { 27.f,3.f,0.f }, 100,20);
					m_fireFlg = true;
				}
			}
		}
		//�I������
		m_selectFlg = false;
		m_shotFlg = false;
	}
	else
	{
		//�p�x���Z�b�g
		m_ang.x += -0.5f;
		if (m_ang.x <= 0)m_ang.x = 0;
	}
	//�N�[���^�C��
	if (m_fireFlg)
	{
		//�A�ˊԊu
		STR_Weapon.coolTime -= 1.0f;
		if (STR_Weapon.coolTime <= 0)
		{
			STR_Weapon.coolTime = 60.0f * 1.0f;
			m_fireFlg = false;
		}
	}

	//�ʖ�����Y���
	if (STR_Weapon.bulletCount <= 0)
	{
		m_relFlg = true;
	}

	//�������͎蓮�����[�h
	if (m_relFlg)
	{
		//�J�E���g�J�n
		--STR_Weapon.nowRelTime;

		//���ԂɂȂ�����
		if (STR_Weapon.nowRelTime <= 0)
		{
			//�����[�h����
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

	//�e��
	STR_Weapon.bulletMax = 10;
	STR_Weapon.bulletCount = STR_Weapon.bulletMax;
	//�����[�h����
	STR_Weapon.relTime = 60.0f * 2.5f;
	STR_Weapon.nowRelTime = STR_Weapon.relTime;
}