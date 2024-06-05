#include "SelectUi.h"

void SelectUi::Update()
{
	//�I������Ă�����������\���i�傫����ς��A�����x��1�ɂ���j
	//��ʂ̂ǂ����𓮂�������
	if (!m_loadFlg)
	{
		m_backAlpha -= 0.01f;
		if (m_backAlpha <= 0.0f)
		{
			m_backAlpha = 0.0f;
		}
	}
	else
	{
		m_backAlpha += 0.01f;
		if (m_backAlpha >= 1.0f)
		{
			m_backAlpha = 1.0f;
			m_nextFlg = true;
		}
	}
	switch (m_selectMode)
	{
	case 1:
		m_trainingAlpha = 1.0f;
		m_storyAlpha = 0.5f;
		m_trainingScale = { 270,68 };
		m_storyScale = {102,34 };
		break;
	case 2:
		m_trainingAlpha = 0.5f;
		m_storyAlpha = 1.0f;
		m_trainingScale = { 135, 34 };
		m_storyScale = { 204,68 };
		break;
	}
}

void SelectUi::PostUpdate()
{
}

void SelectUi::DrawSprite()
{
	//�ݒ�p�ϐ�
	Math::Rectangle rc = { 0,0,1280,720 };
	Math::Color     color = { 1.0f,1.0f,1.0f,m_backAlpha };
	//�V�[���ڍs�摜�`��
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_backTex, 0, 0, 1280, 720, &rc, &color);

	rc = { 0,0,135, 34 };
	color = { 1.0f,1.0f,1.0f,m_trainingAlpha };
	//�g���[�j���OUi�`��
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_trainingLogoTex, 100, 0, (int)m_trainingScale.x, (int)m_trainingScale.y, &rc, &color);

	rc = { 0,0,102,34 };
	color = { 1.0f,1.0f,1.0f,m_storyAlpha };
	//�X�g�[���[Ui�`��
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_storyLogoTex, -100, 0, (int)m_storyScale.x, (int)m_storyScale.y, &rc, &color);
}

void SelectUi::Init()
{
	m_backTex = new KdTexture;
	m_backTex->Load("Asset/Textures/Ui/Select/Back.png");
	m_trainingLogoTex = new KdTexture;
	m_trainingLogoTex->Load("Asset/Textures/Ui/Select/TrainingLogo.png");
	m_storyLogoTex = new KdTexture;
	m_storyLogoTex->Load("Asset/Textures/Ui/Select/StoryLogo.png");
}

void SelectUi::SetTexData()
{
}

void SelectUi::Release()
{
	m_backTex->Release();
	delete m_backTex;
	m_trainingLogoTex->Release();
	delete m_trainingLogoTex;
	m_storyLogoTex->Release();
	delete m_storyLogoTex;
}