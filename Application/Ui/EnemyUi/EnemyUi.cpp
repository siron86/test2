#include "EnemyUi.h"
#include"../../Object/Game/Enemy/NormalEnemy/NormalEnemy.h"

void EnemyUi::Update()
{
	//�v���C���[�̏�������Ui��ω�������
	if (m_wpEne.expired())return;
	std::shared_ptr<NormalEnemy>ene = m_wpEne.lock();

	//���݂�HP�������炤
	//�O�t���[���̒l
	int nowHp = m_hp;

	//�O�t���[���̒l�Ɣ�ׂĕω����Ă�����A��������
	if (nowHp != m_hp)
	{
		m_hitFlg = true;
	}
	else m_hitFlg=false;
}

void EnemyUi::PostUpdate()
{
}

void EnemyUi::DrawSprite()
{
	//�ݒ�p�ϐ�
	Math::Rectangle rc = { 0,0,100,100 };
	Math::Color     color = { 1.0f,1.0f,1.0f,1.0f };
	//Hp�o�[�`��
	//if (m_hitFlg)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_hitTex, 0, -200, 100, 100, &rc, &color);
	}
}

void EnemyUi::Init()
{
	m_hitTex = new KdTexture;
	m_hitTex->Load("Asset/Textures/Ui/Enemy/Hit.png");

}

void EnemyUi::SetTexData()
{
}

void EnemyUi::Release()
{
	m_hitTex->Release();
	delete m_hitTex;
}
