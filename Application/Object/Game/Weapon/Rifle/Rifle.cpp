#include "Rifle.h"
#include"../../Player/Player.h"
#include"../Bullet/Bullet.h"

void Rifle::Update()
{
	//�e���̏��
	const KdModelWork::Node* pNode = GetModel()->FindNode("ShotPoint");
	if (pNode) { m_muzzleMat = pNode->m_worldTransform * m_mWorld; }

	//�I������Ă����甭�ˉ\��
	if (m_selectFlg)
	{
		Shot();
	}

	//�U���Ԋu
	CoolTime();

	//�c�e�Ȃ�
	if (STR_Weapon.bulletCount <= 0)
	{
		m_relFlg = true;
	}

	//�����[�h
	Reload();
}

void Rifle::PostUpdate()
{
	m_scaleMat = Math::Matrix::CreateScale(STR_Weapon.scale);

	m_rotateMat = Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_ang.y),
		DirectX::XMConvertToRadians(m_ang.x),
		DirectX::XMConvertToRadians(m_ang.z)
	);
	m_transMat = Math::Matrix::CreateTranslation(m_pos);

	//�g�偖�ړ����ݒu�ꏊ
	m_mWorld = m_scaleMat * m_rotateMat * m_transMat * m_rackMat;
}

void Rifle::DrawLit_SkinMesh()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void Rifle::DrawDebug()
{
	m_debugWire.Draw();
}

void Rifle::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Weapon/Rifle/RifleT.gltf"));


	//json������===================================================================
	//�t�@�C�����J��
	std::ifstream ifs("Asset/Data/Object.json");
	if (ifs.fail()) { assert(0 && "json�t�@�C�����J���܂���ł���"); };

	//�S�ǂݍ���
	std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	//�ǂݍ��񂾕��������͂���
	std::string err;
	json11::Json jsonObj = json11::Json::parse(strJson, err);
	if (err.size() > 0) { assert(0 && "�ϊ����s"); };

	auto& weaponData = jsonObj["Weapon"].object_items();
	auto& rifleData = weaponData["Rifle"].object_items();

	STR_Weapon.scale = (float)rifleData["Scale"].number_value();
	m_ang = { 0.f,-90.f,180.f };
	m_pos = { 1.f,0.5f,0.f };

	//�e��
	STR_Weapon.bulletMax = 30;
	STR_Weapon.bulletCount = STR_Weapon.bulletMax;

	//�����[�h����
	STR_Weapon.relTime = 60.0f * 2.5f;
	STR_Weapon.nowRelTime = STR_Weapon.relTime;

	//���ˊԊu
	STR_Weapon.coolTime = STR_Weapon.defaultCoolTime;
}

void Rifle::Reload()
{
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

void Rifle::Shot()
{
	//����
	if (m_shotFlg)
	{
		//���˂��Ă��Ȃ�������
		if (STR_Weapon.coolTime == STR_Weapon.defaultCoolTime && !m_weaBase.expired())
		{
			//�c�e���Ȃ�������A�������˂��������[�h�x��Ui�N��
			if (!m_relFlg)
			{
				//�e���g�p
				--STR_Weapon.bulletCount;
				//�e�𔭎�
				m_weaBase.lock()->ShotBullet(m_muzzleMat, { 27.f,3.f,0.f }, 10, 5);
				//�}�Y���t���b�V��
				effObj = KdEffekseerManager::GetInstance().Play("09-1.efkefc", m_muzzleMat.Translation(), false);
				if (!effObj)return;
				effObj->SetWorldMatrix(Math::Matrix::CreateScale(2.f) * m_muzzleMat);
			}
		}

	}
	m_debugWire.AddDebugLine(m_muzzleMat.Translation(), m_weaBase.lock()->CenterShot(m_muzzleMat), 100.f, kRedColor);
	//�I������
	m_selectFlg = false;
	m_shotFlg = false;
}

void Rifle::CoolTime()
{
	//�N�[���^�C��
	if (STR_Weapon.coolTime <= STR_Weapon.defaultCoolTime)
	{
		//�A�ˊԊu
		--STR_Weapon.coolTime;
		if (STR_Weapon.coolTime <= 0.f)
		{
			STR_Weapon.coolTime = STR_Weapon.defaultCoolTime;
		}
	}
}