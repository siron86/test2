#include "EnemyBase.h"
#include"TrainingEnemy/TrainingEnemy.h"
#include"NormalEnemy/NormalEnemy.h"

#include"../../../Scene/GameScene/GameScene.h"
#include"../../Game/ObjectSystem/ObjectSystem.h"
#include"../../Camera/GameCamera/GameCamera.h"

void EnemyBase::PreUpdate()
{
	//イテレータ―で消去
	auto it = m_eneList.begin();
	//最後じゃなかったら
	while (it != m_eneList.end())
	{
		//期限切れじゃないか
		if ((*it)->IsExpired())
		{
			it = m_eneList.erase(it);
		}
		else //切れていなければ次へ
		{
			//次のデータへ
			++it;
		}
	}

	//リストが空かどうか判定する
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
	//waveが進むとカウントが増える
	//違う気がする
	//waveが無限になったらどうする？？

	//エネミーJsonデータ
	auto& eneData = m_stageData["Enemy"].object_items();

	//エネミーJsonデータ
	m_endWave = eneData["MaxWave"].int_value();
	//m_waveCount = 1;
	//各WaveJsonデータ
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
	if (waveData.empty()) { assert(0 && "データがありません[Wave]"); }

	//トレーニングエネミーJsonデータ
	auto& trainingData = waveData["Training"].object_items();
	if (waveData.empty()) { assert(0 && "データがありません[Training]"); }

	//ノーマルエネミーJsonデータ
	auto& normalData = waveData["Normal"].object_items();
	if (waveData.empty()) { assert(0 && "データがありません[Normal]"); }

	bool popFlg = false;
	//情報がない場合
	if (trainingData.empty())popFlg = true;

	//トレーニングエネミー作成
	if (!popFlg)
	{
		std::shared_ptr<TrainingEnemy>traEne;

		//Jsonから送られたカウント数出現させる
		for (int tnum = 0; tnum < trainingData["PopCount"].int_value(); tnum++)
		{
			traEne = std::make_shared<TrainingEnemy>();
			//Jsonの情報をもらいたい
			auto& pos = trainingData["PopPos"].array_items();
			//出現する中心地点の設定
			traEne->SetPos({
		(float)pos[0].number_value(),
		(float)pos[1].number_value(),
		(float)pos[2].number_value(),
				});
			traEne->SetCamera(m_wpECamera.lock());
			traEne->Init();
			m_eneList.push_back(traEne);
			//ゲームシーンのHitObjListに直接push
			m_scene->AddHitObject(traEne);
		}
	}

	popFlg = false;
	//情報がない場合
	if (normalData.empty())popFlg = true;

	//ノーマルエネミー作成
	if (!popFlg)
	{
		std::shared_ptr<NormalEnemy>norEne;
		for (int nnum = 0; nnum < normalData["PopCount"].int_value(); nnum++)
		{
			norEne = std::make_shared<NormalEnemy>();
			auto& pos = normalData["PopPos"].array_items();
			//出現する中心地点の設定
			norEne->SetPos({
		(float)pos[0].number_value(),
		(float)pos[1].number_value(),
		(float)pos[2].number_value(),
				});
			norEne->Init();
			norEne->SetEScene(m_scene);
			norEne->SetNPlayer(m_wpPla);
			m_eneList.push_back(norEne);
			//ゲームシーンのHitObjListに直接push
			m_scene->AddHitObject(norEne);
		}
	}
}