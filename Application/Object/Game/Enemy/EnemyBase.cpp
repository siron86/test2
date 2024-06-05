#include "EnemyBase.h"
#include"TrainingEnemy/TrainingEnemy.h"
#include"NormalEnemy/NormalEnemy.h"

#include"../../../Scene/GameScene/GameScene.h"
#include"../../Game/ObjectSystem/ObjectSystem.h"
#include"../../Camera/GameCamera/GameCamera.h"

void EnemyBase::PreUpdate()
{
	//�C�e���[�^�\�ŏ���
	auto it = m_eneList.begin();
	//�Ōザ��Ȃ�������
	while (it != m_eneList.end())
	{
		//�����؂ꂶ��Ȃ���
		if ((*it)->IsExpired())
		{
			it = m_eneList.erase(it);
		}
		else //�؂�Ă��Ȃ���Ύ���
		{
			//���̃f�[�^��
			++it;
		}
	}

	//���X�g���󂩂ǂ������肷��
	if (m_eneList.empty())
	{
		if (m_waveCount >= m_endWave)
		{
			endFlg = true;
			return;
		}
		++m_waveCount;
		Init();
	}

	for (auto& enemy : m_eneList)
	{
		enemy->PreUpdate();
	}
}

void EnemyBase::Update()
{
	if (!m_controlFlg)return;
	for (auto& enemy : m_eneList)
	{
		enemy->Update();
	}
}

void EnemyBase::PostUpdate()
{
	for (auto& enemy : m_eneList)
	{
		enemy->PostUpdate();
	}
}

void EnemyBase::DrawLit()
{
	for (auto& enemy : m_eneList)
	{
		enemy->DrawLit();
	}
}

void EnemyBase::DrawLit_SkinMesh()
{
	for (auto& enemy : m_eneList)
	{
		enemy->DrawLit_SkinMesh();
	}
}

void EnemyBase::DrawSprite()
{
	for (auto& enemy : m_eneList)
	{
		enemy->DrawSprite();
	}
}

void EnemyBase::DrawDebug()
{
	for (auto& enemy : m_eneList)
	{
		enemy->DrawDebug();
	}
}

void EnemyBase::Init()
{
	//wave���i�ނƃJ�E���g��������
	//�Ⴄ�C������
	//wave�������ɂȂ�����ǂ�����H�H

	//�G�l�~�[Json�f�[�^
	auto& eneData = m_stageData["Enemy"].object_items();

	//�G�l�~�[Json�f�[�^
	m_endWave = eneData["MaxWave"].int_value();
	//m_waveCount = 1;
	//�eWaveJson�f�[�^
	test = "Wave1";
	switch (m_waveCount)
	{
	case 1:
		test = "Wave2";
		break;
	case 2:
		test = "Wave3";
		break;
	}
	auto& waveData = eneData[test].object_items();
	if (waveData.empty()) { assert(0 && "�f�[�^������܂���[Wave]"); }

	//�g���[�j���O�G�l�~�[Json�f�[�^
	auto& trainingData = waveData["Training"].object_items();
	if (waveData.empty()) { assert(0 && "�f�[�^������܂���[Training]"); }

	//�m�[�}���G�l�~�[Json�f�[�^
	auto& normalData = waveData["Normal"].object_items();
	if (waveData.empty()) { assert(0 && "�f�[�^������܂���[Normal]"); }

	bool popFlg = false;
	//��񂪂Ȃ��ꍇ
	if (trainingData.empty())popFlg = true;

	//�g���[�j���O�G�l�~�[�쐬
	if (!popFlg)
	{
		std::shared_ptr<TrainingEnemy>traEne;

		//Json���瑗��ꂽ�J�E���g���o��������
		for (int tnum = 0; tnum < trainingData["PopCount"].int_value(); tnum++)
		{
			traEne = std::make_shared<TrainingEnemy>();
			//Json�̏������炢����
			auto& pos = trainingData["PopPos"].array_items();
			//�o�����钆�S�n�_�̐ݒ�
			traEne->SetPos({
		(float)pos[0].number_value(),
		(float)pos[1].number_value(),
		(float)pos[2].number_value(),
				});
			traEne->SetCamera(m_wpECamera.lock());
			traEne->Init();
			m_eneList.push_back(traEne);
			//�Q�[���V�[����HitObjList�ɒ���push
			m_scene->AddHitObject(traEne);
		}
	}

	popFlg = false;
	//��񂪂Ȃ��ꍇ
	if (normalData.empty())popFlg = true;

	//�m�[�}���G�l�~�[�쐬
	if (!popFlg)
	{
		std::shared_ptr<NormalEnemy>norEne;
		for (int nnum = 0; nnum < normalData["PopCount"].int_value(); nnum++)
		{
			norEne = std::make_shared<NormalEnemy>();
			auto& pos = normalData["PopPos"].array_items();
			//�o�����钆�S�n�_�̐ݒ�
			norEne->SetPos({
		(float)pos[0].number_value(),
		(float)pos[1].number_value(),
		(float)pos[2].number_value(),
				});
			norEne->Init();
			norEne->SetEScene(m_scene);
			norEne->SetNPlayer(m_wpPla);
			m_eneList.push_back(norEne);
			//�Q�[���V�[����HitObjList�ɒ���push
			m_scene->AddHitObject(norEne);
		}
	}
}