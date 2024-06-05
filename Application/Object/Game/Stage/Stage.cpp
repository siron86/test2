#include "Stage.h"
#include"Ground/Ground.h"
#include"Sky/Sky.h"
#include"StageObject/StageObject.h"

#include"../../../Scene/SceneManager.h"
#include"../../../Scene/GameScene/GameScene.h"

void Stage::ImGuiUpdate()
{
	for (auto& stage : m_stageList)
	{
		stage->ImGuiUpdate();

		//選ばれていたら起動
		if (stage->GetSelect())
		{
		}
	}
}

void Stage::Update()
{
	for (auto& stage : m_stageList)
	{
		stage->Update();
	}
}

void Stage::DrawLit()
{
	for (auto& stage : m_stageList)
	{
		stage->DrawLit();
	}
}

void Stage::DrawUnLit()
{
	for (auto& stage : m_stageList)
	{
		stage->DrawUnLit();
	}
}

void Stage::DrawDebug()
{
	for (auto& stage : m_stageList)
	{
		stage->DrawDebug();
	}
}

void Stage::Init()
{
	//地面
	std::shared_ptr<Ground> gro;
	for (int i = 0; i < 1; i++)
	{
		gro = std::make_shared<Ground>();
		//gro->SetGroundData();
		m_stageList.push_back(gro);
		m_scene->AddHitObject(gro);
	}
	//json11::Json jsonData = JsonLoad();
	//auto& buildingData = jsonData["Building"].array_items();
	//std::shared_ptr<StageObject> sObj;
	//for (int i = 1; i <= 5; i++)
	//{
	//	sObj = std::make_shared<StageObject>();
	//	for (auto&& data : buildingData)
	//	{
	//		//jsonデータと個数が同じなら
	//		if (i == data["number"].number_value())
	//		{
	//			sObj->BuildingLoad(data);
	//			sObj->Init();
	//			m_stageList.push_back(sObj);
	//			m_scene->AddHitObject(sObj);
	//			break;
	//		}
	//		//値が違う場合もう一度
	//		else continue;
	//	}
	//}

	//空
	std::shared_ptr<Sky>sky = std::make_shared<Sky>();
	m_stageList.push_back(sky);
	m_scene->AddHitObject(sky);
}

void Stage::Collider(std::shared_ptr<KdModelWork> _objName)
{
	_objName = std::make_shared<KdModelWork>();
}