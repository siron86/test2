#include "ObjectSystem.h"
#include"../../../Scene/SceneManager.h"

//�Ăяo���ꂽ�ꏊ����������炤
void ObjectSystem::HitPointTypeSphere(const std::weak_ptr<KdGameObject>& _owner)
{
	if (_owner.expired())return;
	std::shared_ptr<KdGameObject> owner = _owner.lock();

	//�I�[�i�[�̃R���C�_�[���
	KdCollider::SphereInfo sphereInfo = owner->GetHitPointTypeSphere();


	//�I�[�i�[�̓����肽�����𔻒肵�A���ꂲ�ƂɃC���t�H�ƃ��X�g���쐬
	if (sphereInfo.m_type & KdCollider::TypeGround)
	{
		KdCollider::SphereInfo sphereGroundInfo = sphereInfo;
		sphereGroundInfo.m_type = KdCollider::TypeGround;

		HitSearch(owner, sphereGroundInfo);
	}
	if (sphereInfo.m_type & KdCollider::TypeBump)
	{
		KdCollider::SphereInfo sphereBumpInfo = sphereInfo;
		sphereBumpInfo.m_type = KdCollider::TypeBump;

		HitSearch(owner, sphereBumpInfo);
	}
	if (sphereInfo.m_type & KdCollider::TypeDamage)
	{
		KdCollider::SphereInfo sphereDamageInfo = sphereInfo;
		sphereDamageInfo.m_type = KdCollider::TypeDamage;
		HitSearch(owner, sphereDamageInfo);
	}
	if (sphereInfo.m_type & KdCollider::TypeEvent)
	{
		KdCollider::SphereInfo sphereEventInfo = sphereInfo;
		sphereEventInfo.m_type = KdCollider::TypeEvent;
		HitSearch(owner, sphereEventInfo);
	}
}

void ObjectSystem::HitPointTypeRay(const std::weak_ptr<KdGameObject>& _owner)
{
	std::shared_ptr<KdGameObject> owner = _owner.lock();

	//�I�[�i�[�̃R���C�_�[���
	KdCollider::RayInfo rayInfo = owner->GetHitPointTypeRay();


	//�I�[�i�[�̓����肽�����𔻒肵�A���ꂲ�ƂɃC���t�H�ƃ��X�g���쐬
	if (rayInfo.m_type & KdCollider::TypeGround)
	{
		KdCollider::RayInfo rayGroundInfo = rayInfo;
		rayGroundInfo.m_type = KdCollider::TypeGround;

		HitSearch(owner, rayGroundInfo);
	}
	if (rayInfo.m_type & KdCollider::TypeBump)
	{
		KdCollider::RayInfo rayBumpInfo = rayInfo;
		rayBumpInfo.m_type = KdCollider::TypeBump;

		HitSearch(owner, rayBumpInfo);
	}
	if (rayInfo.m_type & KdCollider::TypeDamage)
	{
		KdCollider::RayInfo rayDamageInfo = rayInfo;
		rayDamageInfo.m_type = KdCollider::TypeDamage;

		HitSearch(owner, rayDamageInfo);
	}
	if (rayInfo.m_type & KdCollider::TypeEvent)
	{
		KdCollider::RayInfo rayEventInfo = rayInfo;
		rayEventInfo.m_type = KdCollider::TypeEvent;

		HitSearch(owner, rayEventInfo);
	}
}

void ObjectSystem::HitSearch(const std::weak_ptr<KdGameObject>& _owner, const KdCollider::SphereInfo& _sphereInfo)
{
	std::list<KdCollider::CollisionResult>resultList;

	//���o�p
	float maxOverLap = 0;
	bool hit = false;

	//�������Ă������
	Math::Vector3 hitDir;

	//���������ꏊ
	Math::Vector3 hitPos;

	//�����蔻��p���X�g���񂵂ē������Ă��镨��T��
	for (auto& target : SceneManager::Instance().GetHitObjList())
	{
		if (_owner.lock() == target)continue;
		target->Intersects
		(
			_sphereInfo,
			&resultList
		);

		//���o
		for (auto& ret : resultList)
		{
			//��Ԃ߂荞�񂾂��̂�T��
			if (maxOverLap < ret.m_overlapDistance)
			{
				maxOverLap = ret.m_overlapDistance;
				hitPos = ret.m_hitPos;
				hitDir = ret.m_hitDir;
				hitDir.Normalize();
				hit = true;
			}
		}
		//�q�b�g������
		if (hit)
		{
			hitDir = hitDir * maxOverLap;
			hitPos = hitPos - Math::Vector3(0.f, 0.f, 0.5f);
			//if (_sphereInfo.m_type & KdCollider::TypeDamage)int damage = target->GetDamege();
			_owner.lock()->Hit(_sphereInfo.m_type, hitDir, hitPos);
			target->Hit(_owner.lock()->GetType(), hitDir, hitPos);
			SceneManager::Instance().HitListIterator();
			break;
		}
	}
}

void ObjectSystem::HitSearch(const std::weak_ptr<KdGameObject>& _owner, const KdCollider::RayInfo& _rayInfo)
{
	std::list<std::shared_ptr<KdGameObject>> targetList = SceneManager::Instance().GetHitObjList();

	if (!targetList.empty())
	{
		std::list<KdCollider::CollisionResult> retRayList;

		for (auto& target : targetList)
		{
			target->Intersects(_rayInfo, &retRayList);

			float maxOverLap = 0;
			Math::Vector3 hitPos = Math::Vector3::Zero;
			//�������Ă������
			Math::Vector3 hitDir = Math::Vector3::Zero;
			bool hit = false;

			for (auto& ret : retRayList)
			{
				// ���C���Ւf���I�[�o�[����������
				// ��Ԓ������̂�T��
				if (maxOverLap < ret.m_overlapDistance)
				{
					maxOverLap = ret.m_overlapDistance;
					hitPos = ret.m_hitPos;
					hitDir = ret.m_hitDir;
					hitDir.Normalize();
					hit = true;
				}
				//else continue;
			}
			if (hit)
			{
				hitDir = hitDir * maxOverLap;
				_owner.lock()->Hit(_rayInfo.m_type, hitDir, hitPos);
				target->Hit(_owner.lock()->GetType(), hitDir, hitPos);
				SceneManager::Instance().HitListIterator();
				break;
			}
		}
	}
}

//void ObjectSystem::LockOn(const std::weak_ptr<KdGameObject>& _owner, const std::weak_ptr<KdGameObject>& _target)
//{
//	////���݌����Ă�������x�N�g��
//}