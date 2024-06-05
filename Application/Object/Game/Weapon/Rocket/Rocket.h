#pragma once
#include"../WeaponBase.h"

class Rocket :public WeaponBase
{
public:
	Rocket() { Init(); }
	~Rocket() {}

	void Update()override;
	void PostUpdate()override;
	void DrawLit()override;
	void Init()override;

	void SetRack(Math::Matrix _rackMat)
	{
		m_rackMat = _rackMat;
	}

	void SelectFlg()override
	{
		m_selectFlg = true;
	}
	
	void ShotFlg()override
	{
		m_shotFlg = true;
	}

	Math::Vector3 GetShotPos()override { return m_shotPos; }

	void SetWeaBase(const std::shared_ptr<KdGameObject>& _weaBase)
	{
		//ダウンキャスト
		m_weaBase = std::dynamic_pointer_cast<WeaponBase>(_weaBase);
	}

	Math::Vector3 ShotPos();

private:

	//発射位置
	Math::Vector3 m_setPos = {};
	Math::Vector3 m_shotPos = {};

	std::weak_ptr<WeaponBase>m_weaBase;

	Math::Matrix m_rackMat;

	std::list<std::shared_ptr<WeaponBase>> m_weaList;
};