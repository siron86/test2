#include "TitleUi.h"

void TitleUi::Update()
{
	//�^�C�g�����S�����X�ɕ\�����A�\���I����w�i����
	//�^�C�g�����S���c���A�w��̃I�u�W�F�N�g��������
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
	//�ݒ�p�ϐ�
	Math::Rectangle rc = { 0,0,1280,720 };
	Math::Color     color = { 1.0f,1.0f,1.0f,m_backAlpha };
	//�V�[���ڍs�摜�`��
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_backTex.get(), 0, 0, 1280, 720, &rc, &color);


	rc = { 0,0,1280,46 };
	color = { 1.0f,1.0f,1.0f,m_titleLogoAlpha };
	//���S�`��
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_titleLogoTex.get(), 0, 100, 1000, 46, &rc, &color);

	rc = { 0,0,421,19 };
	color = { 1.0f,1.0f,1.0f,m_startLogoAlpha };
	//�{�^���`��
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_startLogoTex.get(), 0, -200, 400, 46, &rc, &color);
}

void TitleUi::Init()
{
	//�w�i
	m_backTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Title/Back.png");
	
	//���S
	m_titleLogoTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Title/TitleLogo.png");
	
	//�{�^��
	m_startLogoTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/Ui/Title/StartLogo.png");
}

void TitleUi::SetTexData()
{
}