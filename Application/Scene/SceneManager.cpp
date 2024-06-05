#include "SceneManager.h"
#include"BaseScene/BaseScene.h"
#include"TitleScene/TitleScene.h"
#include"SelectScene/SelectScene.h"
#include"GameScene/GameScene.h"

void SceneManager::ImGuiUpdate()
{
	if (ImGui::Begin("Game"))
	{
		m_currentScene->ImGuiUpdate();
	}
	ImGui::End();
}

void SceneManager::PreUpdate()
{
	//シーン切り替え
	//今のシーンと予約が違っていたら
	if (m_currentSceneType != m_nextSceneType)
	{
		//シーンの変更
		ChangeScene(m_nextSceneType);
	}

	//各シーンのアップデート
	m_currentScene->PreUpdate();
}

void SceneManager::Update()
{
	m_currentScene->Update();
}

void SceneManager::PostUpdate()
{
	m_currentScene->PostUpdate();
}

void SceneManager::PreDraw()
{
	m_currentScene->PreDraw();
}

void SceneManager::Draw()
{
	m_currentScene->Draw();
}

void SceneManager::DrawSprite()
{
	m_currentScene->DrawSprite();
}

void SceneManager::DrawDebug()
{
	m_currentScene->DrawDebug();
}

void SceneManager::HitListIterator()
{
	m_currentScene->HitListIterator();
}

const std::list< std::shared_ptr<KdGameObject>>& SceneManager::GetObjList()const
{
	return m_currentScene->GetObjList();
}

void SceneManager::AddObject(const std::shared_ptr<KdGameObject>& _obj)
{
	m_currentScene->AddObject(_obj);
}

const std::list<std::shared_ptr<KdGameObject>>& SceneManager::GetHitObjList()const
{
	return m_currentScene->GetHitObjList();
}

void SceneManager::AddHitObject(const std::shared_ptr<KdGameObject>& _hitObj)
{
	m_currentScene->AddHitObject(_hitObj);
}


void SceneManager::ChangeScene(SceneType _sceneType)
{
	//現在のシーンを削除
	switch (_sceneType)
	{
	case SceneType::Title:
		m_currentScene = std::make_shared<TitleScene>();
		break;
	case SceneType::Select:
		m_currentScene = std::make_shared<SelectScene>();
		break;
	case SceneType::Game:
		m_currentScene = std::make_shared<GameScene>();
		break;
	}
	m_currentSceneType = _sceneType;
}