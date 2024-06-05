#pragma once
#include"../WeaponBase.h"

class EleGun :public WeaponBase
{
public:
	EleGun() { Init(); }
	~EleGun() {}

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

	void SetWeaBase(const std::shared_ptr<KdGameObject>& _weaBase)
	{
		//ダウンキャスト
		m_weaBase = std::dynamic_pointer_cast<WeaponBase>(_weaBase);
	}

private:

	std::weak_ptr<WeaponBase>m_weaBase;

	Math::Matrix m_rackMat;

	std::list<std::shared_ptr<WeaponBase>> m_weaList;
};