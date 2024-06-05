#include "ObjectSystem.h"
#include"../../../Scene/SceneManager.h"

//呼び出された場所から情報をもらう
void ObjectSystem::HitPointTypeSphere(const std::weak_ptr<KdGameObject>& _owner)
{
	if (_owner.expired())return;
	std::shared_ptr<KdGameObject> owner = _owner.lock();

	//オーナーのコライダー情報
	KdCollider::SphereInfo sphereInfo = owner->GetHitPointTypeSphere();


	//オーナーの当たりたい物を判定し、それごとにインフォとリストを作成
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

	//オーナーのコライダー情報
	KdCollider::RayInfo rayInfo = owner->GetHitPointTypeRay();


	//オーナーの当たりたい物を判定し、それごとにインフォとリストを作成
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

	//検出用
	float maxOverLap = 0;
	bool hit = false;

	//当たっている方向
	Math::Vector3 hitDir;

	//当たった場所
	Math::Vector3 hitPos;

	//当たり判定用リストを回して当たっている物を探す
	for (auto& target : SceneManager::Instance().GetHitObjList())
	{
		if (_owner.lock() == target)continue;
		target->Intersects
		(
			_sphereInfo,
			&resultList
		);

		//検出
		for (auto& ret : resultList)
		{
			//一番めり込んだものを探す
			if (maxOverLap < ret.m_overlapDistance)
			{
				maxOverLap = ret.m_overlapDistance;
				hitPos = ret.m_hitPos;
				hitDir = ret.m_hitDir;
				hitDir.Normalize();
				hit = true;
			}
		}
		//ヒットしたら
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
			//当たっている方向
			Math::Vector3 hitDir = Math::Vector3::Zero;
			bool hit = false;

			for (auto& ret : retRayList)
			{
				// レイを遮断しオーバーした長さが
				// 一番長いものを探す
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
//	////現在向いている方向ベクトル
//}