#include "TitleUi.h"

void TitleUi::Update()
{
	//タイトルロゴを徐々に表示し、表示終了後背景透過
	//タイトルロゴを残し、背後のオブジェクトを見せる
	if (!m_loadFlg)
	{
		m_titleLogoAlpha += 0.01f;
		if (m_titleLogoAlpha >= 1.0f)
		{
			m_titleLogoAlpha = 1.0f;
			m_logoFlg = true;
		}
		if (m_logoFlg)
		{
			m_backAlpha -= 0.01f;
			if (m_backAlpha <= 0.0f)
			{
				m_backAlpha = 0.0f;
			}
		}
	}
	else
	{
		m_backAlpha += 0.01f;
		m_titleLogoAlpha -= 0.01f;
		if (m_backAlpha >= 1.0f)
		{
			m_backAlpha = 1.0f;
			m_nextFlg = true;
		}
	}
}

void TitleUi::PostUpdate()
{
}

void TitleUi::DrawSprite()
{
	//設定用変数
	Math::Rectangle rc = { 0,0,1280,720 };
	Math::Color     color = { 1.0f,1.0f,1.0f,m_backAlpha };
	//シーン移行画像描画
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_backTex.get(), 0, 0, 1280, 720, &rc, &color);


	rc = { 0,0,1280,46 };
	color = { 1.0f,1.0f,1.0f,m_titleLogoAlpha };
	//ロゴ描画
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_titleLogoTex.get(), 0, 100, 1000, 46, &rc, &color);

	rc = { 0,0,421,19 };
	color = { 1.0f,1.0f,1.0f,m_startLogoAlpha };
	//ボタン描画
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_startLogoTex.get(), 0, -200, 400, 46, &rc, &color);
}

void TitleUi::Init()
{
	//背景
	m_backTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Title/Back.png");
	
	//ロゴ
	m_titleLogoTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Title/TitleLogo.png");
	
	//ボタン
	m_startLogoTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Title/StartLogo.png");
}

void TitleUi::SetTexData()
{
}