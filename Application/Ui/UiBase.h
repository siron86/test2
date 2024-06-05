#pragma once
//=======================================================================================
//UIをまとめるクラス
// 
//=======================================================================================

class Player;

class UiBase :public KdGameObject
{
public:
	UiBase() {}
	~UiBase() { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawSprite()override;
	void Init()override;

	void SetTexData();

	void PopControl(bool _popFlg)
	{
		m_popFlg = _popFlg;
	}
	void GetPlayer(std::weak_ptr<Player>_wpPla)
	{
		m_wpPla = _wpPla;
	}

private:
	void Release();

	//テスト
	bool m_popFlg = true;

	std::list<std::shared_ptr<UiBase>>m_uiList;

	//情報
	std::weak_ptr<Player>m_wpPla;
};