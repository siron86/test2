#pragma once
#include"../UiBase.h"

class TitleUi :public UiBase
{
public:
	TitleUi() { Init(); }
	~TitleUi() {}

	void Update()override;
	void PostUpdate()override;
	void DrawSprite()override;
	void Init()override;

	void SetTexData();

	//セッター
	void SetLoadFlg(bool _loadFlg)
	{
		m_loadFlg = _loadFlg;
	}

	//ゲッター
	bool GetNextFlg()
	{
		return m_nextFlg;
	}


private:

	bool m_logoFlg = false;
	bool m_loadFlg = false;
	bool m_nextFlg = false;

	float m_backAlpha = 1.0f;
	float m_titleLogoAlpha = 0.0f;
	float m_startLogoAlpha = 1.0f;

	std::shared_ptr<KdTexture> m_backTex;
	std::shared_ptr<KdTexture> m_titleLogoTex;
	std::shared_ptr<KdTexture> m_startLogoTex;
};