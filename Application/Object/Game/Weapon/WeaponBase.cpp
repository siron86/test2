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
	//イテレータ―で消去
	auto it = m_weaList.begin();
	//最後じゃなかったら
	while (it != m_weaList.end())
	{
		//期限切れじゃないか
		if ((*it)->IsExpired())
		{
			it = m_weaList.erase(it);
		}
		else //切れていなければ次へ
		{
			//次のデータへ
			++it;
		}
	}
}

void WeaponBase::Update()
{
	//選択中ウェポンのフラグを立てる
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
	//装備する場所を選んでね
	//要改良!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
	//ウェポン
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
	//弾の作成
	std::shared_ptr<Bullet>Bul = std::make_shared<Bullet>();

	//弾自身にベース情報を渡す
	Bul->GetWeaBase(shared_from_this());

	//どの場所に出現するか
	Bul->SetMatrix(_muzzleMat);

	//大きさ、速さの設定
	Bul->SetBulletType(_size, _speed);

	//ダメージ値を弾に与える
	//Bul->SetDamage(_damage);

	//オーナーの状態を見てロックオン中かどうかを判断し渡す情報を変える
	if (!m_wpOwn.lock()->LockOnFlg())
	{
		//発射方向(通常時)
		Bul->SetShotVec(CenterShot(_muzzleMat));
	}
	else
	{
		//ホーミング処理
		Bul->Homing(m_targetPos);
	}
	//初期化
	Bul->Init();

	//リストにプッシュバック
	AddWeapon(Bul);
}

const Math::Matrix WeaponBase::EquipmentPoint(const std::string_view& _pointName)
{
	//自機の特定の場所に配置する
	Math::Matrix setMat = Math::Matrix::Identity;

	//持たせるキャラが生きていたら
	if (!m_wpOwn.expired())
	{
		//ダウンキャスト
		const std::shared_ptr<KdGameObject>owner = std::dynamic_pointer_cast<KdGameObject>(m_wpOwn.lock());

		const KdModelWork::Node* pNode = owner->GetModel()->FindNode(_pointName);
		//ノード情報を持っていたら
		if (pNode)
		{
			//持たせたい場所のノード座標を渡す
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

	//カメラの情報
	if (!m_wpCamera.expired())
	{
		std::shared_ptr<GameCamera>cam = m_wpCamera.lock();
		cam->WorkCamera()->GenerateRayInfoFromClientPos
		({ 640, 360 },
			cam->GetPos(),
			dir,
			range
		);

		//中央に何かないか確認
		KdCollider::RayInfo weaponRayInfo;

		//発射位置を設定
		weaponRayInfo.m_pos = cam->GetPos();//原点

		//発射方向
		weaponRayInfo.m_dir = dir;

		weaponRayInfo.m_range = range;

		//当たるタイプの指定
		weaponRayInfo.m_type = KdCollider::TypeBump | KdCollider::TypeEvent | KdCollider::TypeDamage;

		std::list<std::shared_ptr<KdGameObject>> list = m_scene->GetList();

		if (!list.empty())
		{
			std::list<KdCollider::CollisionResult> retRayList;

			for (auto& obj : list)
			{
				//当たってほしいタイプのオブジェクトでなければ回す必要はない
				if (obj == shared_from_this())continue;
				//if (!(obj->GetType() & rayInfo.m_type))continue;
				obj->Intersects(weaponRayInfo, &retRayList);

				float maxOverLap = 0.f;
				Math::Vector3 hitPos = Math::Vector3::Zero;
				bool hit = false;

				for (auto& ret : retRayList)
				{
					// レイを遮断しオーバーした長さが
					// 一番長いものを探す
					if (maxOverLap < ret.m_overlapDistance)
					{
						maxOverLap = ret.m_overlapDistance;
						hitPos = ret.m_hitPos;
						hit = true;
					}
				}
				if (hit)//何かにあたっていたらそこを目標に飛ぶ
				{
					m_shotVec = hitPos - _muzzleMat.Translation();
					range = dir.Length();
					m_debugWire.AddDebugLine(_muzzleMat.Translation(), m_shotVec, range, kRedColor);
				}
				//なにとも当たっていない場合直線に飛ぶ
				else
				{
					//なにとも当たっていない場合直線に飛ぶ
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
	//選択中のウェポンを確認
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
