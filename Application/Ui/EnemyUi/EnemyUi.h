#pragma once
#include"../UiBase.h"
class NormalEnemy;

class EnemyUi :public UiBase
{
public:
	EnemyUi() { Init(); }
	~EnemyUi() { Release(); }

	void Update()override;
	void PostUpdate()override;
	void DrawSprite()override;
	void Init()override;

	void SetTexData();

	void GetEnemy(std::weak_ptr<NormalEnemy>_wpEne)
	{
		m_wpEne = _wpEne;
	}

private:
	void Release();

	//Hpî•ñ
	bool m_hitFlg = false;
	int m_hp = 400;
	int m_boost = 400;
	Math::Color m_boostCol = { 1.0f,1.0f,1.0f,0.4f };

	//‰æ‘œ•\¦
	KdTexture* m_hitTex;

	//î•ñ
	std::weak_ptr<NormalEnemy>m_wpEne;
};