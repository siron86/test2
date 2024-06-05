#pragma once
//=======================================================================================
//UI���܂Ƃ߂�N���X
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

	//�e�X�g
	bool m_popFlg = true;

	std::list<std::shared_ptr<UiBase>>m_uiList;

	//���
	std::weak_ptr<Player>m_wpPla;
};