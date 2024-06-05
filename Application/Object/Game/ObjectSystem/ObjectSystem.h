#pragma once

class ObjectSystem
{
public:
	ObjectSystem() {}
	~ObjectSystem() {}
	//=======================================================================================
	//当たり判定
	//当たってほしいタイプと当たった時当たったタイプを返すことで当たり判定をする
	//=======================================================================================
	void HitPointTypeSphere(const std::weak_ptr<KdGameObject>& _owner);


	void HitPointTypeRay(const std::weak_ptr<KdGameObject>& _owner);

	void HitSearch(const std::weak_ptr<KdGameObject>& _owner, const KdCollider::SphereInfo& _sphereInfo);
	void HitSearch(const std::weak_ptr<KdGameObject>& _owner, const KdCollider::RayInfo& _rayInfo);

	//bool TestHitPoint(KdCollider::SphereInfo _ownerInfo ,KdCollider::Type _type);
	//================================================================================================

	//=======================================================================================
	//ロックオン機能(単体用)
	// ロックオンは対象の情報をもらい、対象の座標を用いて対象への方向ベクトルを作成し、向きを合わせる
	//=======================================================================================
	//void LockOn(const std::weak_ptr<KdGameObject>& _owner, const std::weak_ptr<KdGameObject>& _target);

	//=======================================================================================
	//ロックオン機能(複数用)
	// ロックオンは対象の情報をもらい、対象の座標を用いて対象への方向ベクトルを作成し、向きを合わせる
	// ターゲット側のリストをもらい
	//=======================================================================================
	//void LockOnMulti(const std::weak_ptr<KdGameObject>& _owner, const std::weak_ptr<KdGameObject>& _target) {}

	//ソートしたターゲット情報を保持、与える
	std::list<std::shared_ptr<KdGameObject>> GetTargetSortList() {}

private:

	//当たり判定をしたい物をいれるリストつくります
	std::list<std::shared_ptr<KdGameObject>> m_hitList;
};