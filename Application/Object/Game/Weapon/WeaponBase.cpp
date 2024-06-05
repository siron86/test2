#include "WeaponBase.h"
#include"../Weapon/Rifle/Rifle.h"
#include"../Weapon/Rocket/Rocket.h"
#include"../Weapon/EleGun/EleGun.h"
#include"../Weapon/Bullet/Bullet.h"

#include"../../Camera/GameCamera/GameCamera.h"
#include "../../../Scene/GameScene/GameScene.h"

void WeaponBase::ImGuiUpdate()
{
	//
	if (m_weaList.empty())return;
	for (auto& wea : m_weaList)
	{
		wea->ImGuiUpdate();
	}
}

void WeaponBase::PreUpdate()
{
	//�C�e���[�^�\�ŏ���
	auto it = m_weaList.begin();
	//�Ōザ��Ȃ�������
	while (it != m_weaList.end())
	{
		//�����؂ꂶ��Ȃ���
		if ((*it)->IsExpired())
		{
			it = m_weaList.erase(it);
		}
		else //�؂�Ă��Ȃ���Ύ���
		{
			//���̃f�[�^��
			++it;
		}
	}
}

void WeaponBase::Update()
{
	//�I�𒆃E�F�|���̃t���O�𗧂Ă�
	switch (m_weaType)
	{
	case Rif:
		rif->SelectFlg();
		break;
	case Roc:
		roc->SelectFlg();
		break;
	case Ele:
		ele->SelectFlg();
		break;
	default:
		break;
	}


	for (auto& wea : m_weaList)
	{
		wea->Update();
	}
}

void WeaponBase::PostUpdate()
{
	//��������ꏊ��I��ł�
	//�v����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	rif->SetRack(EquipmentPoint("HandPoint_R"));
	//roc->SetRack(EquipmentPoint("WeaponRackR"));
	//ele->SetRack(EquipmentPoint("WeaponRackL"));

	for (auto& wea : m_weaList)
	{
		wea->SetRack(EquipmentPoint("HandPoint_R"));
		wea->PostUpdate();
	}
}

void WeaponBase::DrawLit()
{
	for (auto& wea : m_weaList)
	{
		wea->DrawLit();
	}
}

void WeaponBase::DrawLit_SkinMesh()
{
	for (auto& wea : m_weaList)
	{
		wea->DrawLit_SkinMesh();
	}
}

void WeaponBase::DrawUnLit()
{
	for (auto& wea : m_weaList)
	{
		wea->DrawUnLit();
	}
}

void WeaponBase::DrawDebug()
{
	for (auto& wea : m_weaList)
	{
		wea->DrawDebug();
	}
}

void WeaponBase::Init()
{
	//�E�F�|��
	rif = std::make_shared<Rifle>();
	rif->SetWeaBase(shared_from_this());
	m_weaList.push_back(rif);

	//roc = std::make_shared<Rocket>();
	//roc->SetWeaBase(shared_from_this());
	//m_weaList.push_back(roc);

	//ele = std::make_shared<EleGun>();
	//ele->SetWeaBase(shared_from_this());
	//m_weaList.push_back(ele);
}

void WeaponBase::ShotBullet(const Math::Matrix& _muzzleMat, const Math::Vector3& _size, const float& _speed, const int& _damage, const Math::Vector3&)
{
	//�e�̍쐬
	std::shared_ptr<Bullet>Bul = std::make_shared<Bullet>();

	//�e���g�Ƀx�[�X����n��
	Bul->GetWeaBase(shared_from_this());

	//�ǂ̏ꏊ�ɏo�����邩
	Bul->SetMatrix(_muzzleMat);

	//�傫���A�����̐ݒ�
	Bul->SetBulletType(_size, _speed);

	//�_���[�W�l��e�ɗ^����
	//Bul->SetDamage(_damage);

	//�I�[�i�[�̏�Ԃ����ă��b�N�I�������ǂ����𔻒f���n������ς���
	if (!m_wpOwn.lock()->LockOnFlg())
	{
		//���˕���(�ʏ펞)
		Bul->SetShotVec(CenterShot(_muzzleMat));
	}
	else
	{
		//�z�[�~���O����
		Bul->Homing(m_targetPos);
	}
	//������
	Bul->Init();

	//���X�g�Ƀv�b�V���o�b�N
	AddWeapon(Bul);
}

const Math::Matrix WeaponBase::EquipmentPoint(const std::string_view& _pointName)
{
	//���@�̓���̏ꏊ�ɔz�u����
	Math::Matrix setMat = Math::Matrix::Identity;

	//��������L�����������Ă�����
	if (!m_wpOwn.expired())
	{
		//�_�E���L���X�g
		const std::shared_ptr<KdGameObject>owner = std::dynamic_pointer_cast<KdGameObject>(m_wpOwn.lock());

		const KdModelWork::Node* pNode = owner->GetModel()->FindNode(_pointName);
		//�m�[�h���������Ă�����
		if (pNode)
		{
			//�����������ꏊ�̃m�[�h���W��n��
			setMat = pNode->m_worldTransform * m_wpOwn.lock()->GetMatrix();
		}
	}
	return setMat;
}

Math::Vector3 WeaponBase::CenterShot(const Math::Matrix& _muzzleMat)
{
	//
	Math::Vector3 dir;
	float range;
	m_shotVec = Math::Vector3::Zero;

	//�J�����̏��
	if (!m_wpCamera.expired())
	{
		std::shared_ptr<GameCamera>cam = m_wpCamera.lock();
		cam->WorkCamera()->GenerateRayInfoFromClientPos
		({ 640, 360 },
			cam->GetPos(),
			dir,
			range
		);

		//�����ɉ����Ȃ����m�F
		KdCollider::RayInfo weaponRayInfo;

		//���ˈʒu��ݒ�
		weaponRayInfo.m_pos = cam->GetPos();//���_

		//���˕���
		weaponRayInfo.m_dir = dir;

		weaponRayInfo.m_range = range;

		//������^�C�v�̎w��
		weaponRayInfo.m_type = KdCollider::TypeBump | KdCollider::TypeEvent | KdCollider::TypeDamage;

		std::list<std::shared_ptr<KdGameObject>> list = m_scene->GetList();

		if (!list.empty())
		{
			std::list<KdCollider::CollisionResult> retRayList;

			for (auto& obj : list)
			{
				//�������Ăق����^�C�v�̃I�u�W�F�N�g�łȂ���Ή񂷕K�v�͂Ȃ�
				if (obj == shared_from_this())continue;
				//if (!(obj->GetType() & rayInfo.m_type))continue;
				obj->Intersects(weaponRayInfo, &retRayList);

				float maxOverLap = 0.f;
				Math::Vector3 hitPos = Math::Vector3::Zero;
				bool hit = false;

				for (auto& ret : retRayList)
				{
					// ���C���Ւf���I�[�o�[����������
					// ��Ԓ������̂�T��
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap = ret.m_overlapDistance;
						hitPos = ret.m_hitPos;
						hit = true;
					}
				}
				if (hit)//�����ɂ������Ă����炻����ڕW�ɔ��
				{
					m_shotVec = hitPos - _muzzleMat.Translation();
					range = dir.Length();
					m_debugWire.AddDebugLine(_muzzleMat.Translation(), m_shotVec, range, kRedColor);
				}
				//�ȂɂƂ��������Ă��Ȃ��ꍇ�����ɔ��
				else
				{
					//�ȂɂƂ��������Ă��Ȃ��ꍇ�����ɔ��
					m_shotVec = m_wpOwn.lock()->GetMatrix().Backward();
				}
			}
		}
	}
	m_shotVec.Normalize();
	return m_shotVec;
}

void WeaponBase::Active()
{
	//�I�𒆂̃E�F�|�����m�F
	switch (m_weaType)
	{
	case Rif:
		rif->ShotFlg();
		break;
	case Roc:
		roc->ShotFlg();
		break;
	case Ele:
		ele->ShotFlg();
		break;
	default:
		break;
	}
}
