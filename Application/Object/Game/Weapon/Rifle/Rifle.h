#pragma once
#include"../WeaponBase.h"

class Rifle :public WeaponBase
{
public:
	Rifle() { Init(); }
	~Rifle() {}

	//更新
	void Update()override;
	void PostUpdate()override;

	//描画
	void DrawLit_SkinMesh() override;
	void DrawDebug()override;
	
	//初期化
	void Init()override;

	//機能関数

	//リロード処理
	void Reload();

	//攻撃（射撃）処理
	void Shot();

	//攻撃間隔の設定
	void CoolTime();

	void SetRack(const Math::Matrix& _rackMat)override { m_rackMat = _rackMat; }

	void SelectFlg()override { m_selectFlg = true; }

	void ShotFlg()override { m_shotFlg = true; }

	void SetWeaBase(const std::shared_ptr<KdGameObject>& _weaBase)
	{
		//ダウンキャスト
		m_weaBase = std::dynamic_pointer_cast<WeaponBase>(_weaBase);
	}

private:

	std::weak_ptr <WeaponBase>m_weaBase;

	//エフェクト用====================================================================================
	std::shared_ptr<KdEffekseerObject> effObj;
	Math::Matrix effMat = Math::Matrix::Identity;
	std::list<std::shared_ptr<WeaponBase>> m_weaList;
};