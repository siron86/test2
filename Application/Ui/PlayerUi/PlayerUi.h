#pragma once
#include"../UiBase.h"
class Player;

class PlayerUi :public UiBase
{
public:
	PlayerUi() { Init(); }
	~PlayerUi() {}

	void Update()override;
	void PostUpdate()override;
	void DrawSprite()override;
	void Init()override;

	void SetTexData();

	void GetPlayer(const std::weak_ptr<Player>_wpPla) { m_wpPla = _wpPla; }

private:
	struct TexData
	{
		Math::Rectangle rc;
		Math::Color     color;
		Math::Vector2   pivot;
	};

	//Hpî•ñ
	bool m_hpFlg = false;
	int m_hp = 400;
	float m_boost = 400.f;
	Math::Color m_boostCol = { 1.0f,1.0f,1.0f,0.4f };

	//‰æ‘œ•\¦
	std::shared_ptr<KdTexture> m_hpTex;
	std::shared_ptr<KdTexture> m_hpFrameTex;
	std::shared_ptr<KdTexture> m_boostTex;
	std::shared_ptr<KdTexture> m_boostFrameTex;
	std::shared_ptr<KdTexture> m_shotTex;
	std::shared_ptr<KdTexture> m_weaponTex;
	std::shared_ptr<KdTexture> m_targetTex;
	std::shared_ptr<KdTexture> m_lockTex;

	//î•ñ
	std::weak_ptr<Player>m_wpPla;
};