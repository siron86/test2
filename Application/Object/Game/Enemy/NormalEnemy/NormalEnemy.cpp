#include "NormalEnemy.h"
#include"../../../Game/Weapon/WeaponBase.h"
#include"../../../Game/Player/Player.h"
#include"../../../Game/ObjectSystem/ObjectSystem.h"

void NormalEnemy::PreUpdate()
{
	if (m_isExpired)return;

	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->PreUpdate();
	//===================================================
}

void NormalEnemy::Update()
{
	if (m_isExpired)return;

	//�����蔻�菈��
	std::shared_ptr<ObjectSystem>objSystem = m_wpSys.lock();
	objSystem->HitPointTypeSphere(shared_from_this());
	objSystem->HitPointTypeRay(shared_from_this());

	//�U�����[�e�[�V����
	Attack();

	//���S��
	DeathAnima();

	//�ړ�
	if (!m_wpPla.expired())
	{
		Math::Vector3 moveVec = m_wpPla.lock()->GetPos() - GetPos();
		if (moveVec.Length() <= 100)moveVec = Math::Vector3::Zero;
		moveVec.Normalize();
		moveVec *= 0.5f;
		//�d�̓e�X�g
		gravity -= 0.002f;
		moveVec.y += gravity;
		Math::Matrix trans = Math::Matrix::CreateTranslation(moveVec);
		m_mWorld *= trans;
	}

	//�ړ��͂��̏�ɏ���
	RotateFromTarget(m_mWorld.Backward(), m_wpPla.lock()->GetPos() - GetPos());

	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	AttackType((int)m_attackType);
	m_spWeaBase->SetTarPos(m_wpPla.lock()->GetHitPointTypeSphere().m_sphere.Center);
	m_spWeaBase->SetScene(m_eScene);
	m_spWeaBase->Update();
	m_weaList = m_spWeaBase->GetWeaList();
	//===================================================
}

void NormalEnemy::PostUpdate()
{
	if (!m_spAnimator)	return;

	// �A�j���[�V�����̍X�V
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes(), 3.0f);
	m_spModel->CalcNodeMatrices();

	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->PostUpdate();
	//===================================================
}

void NormalEnemy::DrawLit()
{
	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawLit();
	//===================================================
}

void NormalEnemy::DrawLit_SkinMesh()
{
	if (!m_spModel)return;
	//3D���f��
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_spModel, m_mWorld);
}

void NormalEnemy::DrawDebug()
{
	m_debugWire.Draw();

	//�E�F�|���x�[�X=====================================
	if (!m_spWeaBase)return;
	m_spWeaBase->DrawDebug();
	//===================================================
}

void NormalEnemy::Init()
{
	//�傫��
	m_scale = 1.f;
	m_mWorld *= Math::Matrix::CreateScale(m_scale);

	std::random_device rnd2;
	std::mt19937 mt2(rnd2());
	std::uniform_int_distribution<int> intRand2(5, 9);

	int randNum2[5] = { 4,3,2,1 };
	int rndNum = intRand2(mt2);

	rndNum = intRand2(mt2);

	for (int j = 0; j < 5; j++)
	{
		rndNum -= randNum2[j];
		m_attackCount = rndNum;
	}

	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/Normal/NormalEnemy.gltf"));

	// �����̃A�j���[�V�������Z�b�g����
	m_spAnimator = std::make_shared<KdAnimator>();
	m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Attack"), false);

	//�����蔻��p
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy", m_spModel, KdCollider::TypeEvent);

	//�E�F�|���x�[�X===============================================================
	m_spWeaBase = std::make_shared<WeaponBase>();
	m_spWeaBase->Init();
	m_spWeaBase->SetWeaponType(1);
	m_spWeaBase->SetOwner(shared_from_this());
	//=============================================================================
}

//�U�����@
void NormalEnemy::AttackType(int _type)
{
	//�U�����G�l�~�[�Ƃ��������E�F�|���ɓ`���A��x�̍U���ŏo��e���Ȃǂ�������
	m_spWeaBase->EnemyBullet(m_maxShotCount, true);

	m_spWeaBase->SetWeaponType(_type);
	m_spWeaBase->Active();
}

void NormalEnemy::Attack()
{
	//================================================================================================
	//���[�e�[�V����
	//================================================================================================
	if (m_coolTime <= 0)
	{
		++m_attackCount;
		switch (m_attackCount)
		{
		case 1:
			m_attackType = AttackWeapon::Rifle;
			m_maxShotCount = 5;
			break;
		case 2:
			m_attackType = AttackWeapon::Rifle;
			m_maxShotCount = 5;
			break;
		case 3:
			m_attackType = AttackWeapon::Rifle;
			m_maxShotCount = 5;
			break;
		case 4:
			m_attackType = AttackWeapon::Rifle;
			m_maxShotCount = 5;
			break;
		case 5:
			m_attackType = AttackWeapon::Rifle;
			m_maxShotCount = 5;
			m_attackCount = 0;
			break;
		}
		m_coolTime = 60.0f * 5.0f;
	}
	--m_coolTime;
	//================================================================================================
}

void NormalEnemy::RotateFromTarget(Math::Vector3 _ownerVec, Math::Vector3 _tarVec)
{
	_ownerVec.Normalize();
	_tarVec.Normalize();

	//��]�p�x�����߂�(����)
	float dot = _ownerVec.Dot(_tarVec);
	//���~�b�^�[
	dot = std::clamp(dot, -1.0f, 1.0f);

	//���ς���t�]���̒藝�ŊԂ̊p�x�����߂�
	float Ang = acos(dot);
	Ang = DirectX::XMConvertToDegrees(Ang);

	float rotateAng = std::clamp(Ang, -8.0f, 8.0f);

	Math::Vector3 rotAxis;

	_ownerVec.Cross(_tarVec, rotAxis);

	if (rotAxis.LengthSquared() == 0)return;

	//�U����]����ׂ̍s��
	rotAxis = Math::Vector3(0.f, rotAxis.y, 0.f);
	//Error������
	m_rotateMat = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	//���̃��[���h���W���ꎞ�I�ɕۑ�
	Math::Vector3 pos = GetPos();
	m_mWorld.Translation(Math::Vector3(0, 0, 0));

	//��]�s��̍���
	m_mWorld *= m_rotateMat;

	//��]����������ɖ߂��i����Ȃ��ƌ��_����̊p�x���c��Ă�������j
	m_mWorld.Translation(pos);
}

void NormalEnemy::DeathAnima()
{
	if (m_hp > 0.f)return;
	//���S���G�t�F�N�g
	KdEffekseerManager::GetInstance().Play("O_patch_stElmo_area_2.efkefc", GetHitPointTypeSphere().m_sphere.Center, false);
	KdEffekseerManager::GetInstance().SetScale("O_patch_stElmo_area_2.efkefc", 3);

	if (!KdEffekseerManager::GetInstance().IsPlaying("O_patch_stElmo_area_2.efkefc")) { m_isExpired = true; }
}