#include "Rocket.h"
#include"../../Player/Player.h"
#include"../Bullet/Bullet.h"

void Rocket::Update()
{
	const KdModelWork::Node* pNode = GetModel()->FindNode("ShotPoint");
	if (pNode)
	{
		m_muzzleMat = pNode->m_worldTransform * Math::Matrix::CreateTranslation(0.f, 5.f, 0.f) * m_mWorld;
	}

	//�G�l�~�[��������
	if (m_weaBase.lock()->Enemy() && m_enemyFlg == false)
	{
		STR_Weapon.bulletCount = m_weaBase.lock()->EnemyMaxShot();
		STR_Weapon.nowRelTime = 60 * 3;
		m_enemyFlg = true;
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
					m_shotPos = ShotPos();
					//�e�𔭎�
					m_weaBase.lock()->ShotBullet(m_muzzleMat, { 5.f,5.f,5.f }, 2.0f, 8, m_shotPos);
					--STR_Weapon.bulletCount;
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
		--STR_Weapon.coolTime;
		if (STR_Weapon.coolTime <= 0)
		{
			STR_Weapon.coolTime = 60.0f * 0.2f;
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
			m_enemyFlg = false;
			STR_Weapon.bulletCount = STR_Weapon.bulletMax;
			STR_Weapon.nowRelTime = STR_Weapon.relTime;
		}
	}
}

void Rocket::PostUpdate()
{
	m_scaleMat = Math::Matrix::CreateScale(2.0f, 2.0f, 2.0f);

	m_rotateMat = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_ang.x));

	m_transMat = Math::Matrix::CreateTranslation(m_pos);

	m_mWorld = m_scaleMat * m_rotateMat * m_transMat * m_rackMat;
}

void Rocket::DrawLit()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Rocket::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Weapon/Rocket/Rocket2.gltf"));

	STR_Weapon.coolTime = 60.0f * 0.2f;
	STR_Weapon.bulletMax = 12;
	STR_Weapon.bulletCount = STR_Weapon.bulletMax;
	STR_Weapon.relTime = 60.0f * 1.0f;
	STR_Weapon.nowRelTime = STR_Weapon.relTime;
}

Math::Vector3 Rocket::ShotPos()
{
	switch (STR_Weapon.bulletCount)
	{
	case 0:
		break;
	case 1:
	case 7:
		m_setPos = { -1.0f,-1.0f,0 };
		break;
	case 2:
	case 8:
		m_setPos = { 1.0f,-1.0f,0 };
		break;
	case 3:
	case 9:
		m_setPos = { -1.0f,0.0f,0 };
		break;
	case 4:
	case 10:
		m_setPos = { 1.0f,0.0f,0 };
		break;
	case 5:
	case 11:
		m_setPos = { -1.0f,1.0f,0 };
		break;
	case 6:
	case 12:
		m_setPos = { 1.0f,1.0f,0 };
		break;

	default:
		break;
	}

	return m_setPos;
}