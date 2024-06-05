#pragma once
#include"../UiBase.h"

class SelectUi :public UiBase
{
public:
	SelectUi() { Init(); }
	~SelectUi() { Release(); }

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
	void SetMode(int _selectMode)
	{
		m_selectMode = _selectMode;
	}

	//ゲッター
	bool GetNextFlg()
	{
		return m_nextFlg;
	}

	const int& GetMode()const
	{
		return m_selectMode;
	}

private:
	void Release();

	bool m_logoFlg = false;
	bool m_loadFlg = false;
	bool m_nextFlg = false;

	int m_selectMode = 0;

	float m_backAlpha = 1.0f;
	float m_trainingAlpha = 0.5f;
	float m_storyAlpha = 0.5f;

	Math::Vector2 m_trainingScale = { 121, 18 };
	Math::Vector2 m_storyScale = { 88, 18 };

	KdTexture* m_backTex;
	KdTexture* m_trainingLogoTex;
	KdTexture* m_storyLogoTex;
};