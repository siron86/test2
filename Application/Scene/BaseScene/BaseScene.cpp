#include "BaseScene.h"

void BaseScene::ImGuiUpdate()
{
	for (auto& obj : m_objList)
	{
		obj->ImGuiUpdate();
	}
}

void BaseScene::PreUpdate()
{
	for (auto& obj : m_objList)
	{
		obj->PreUpdate();
	}
	ListIterator();
}

void BaseScene::Update()
{
	//オブジェクトのUpdate
	for (auto& obj : m_objList)
	{
		obj->Update();
	}

	Event();
}

void BaseScene::PostUpdate()
{
	//最終更新
	for (auto& obj : m_objList)
	{
		obj->PostUpdate();
	}
}

void BaseScene::PreDraw()
{
	for (auto& obj : m_objList)
	{
		obj->PreDraw();
	}
}

void BaseScene::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光を遮るオブジェクト
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& obj : m_objList)
		{
			obj->GenerateDepthMapFromLight();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight();

	// スキンメッシュかつ光を遮るオブジェクト
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight_SkinMesh();
	{
		for (auto& obj : m_objList)
		{
			obj->GenerateDepthMapFromLight_SkinMesh();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight_SkinMesh();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト
	KdShaderManager::Instance().m_HD2DShader.BeginLit();
	{
		//オブジェクト描画
		for (auto& obj : m_objList)
		{
			obj->DrawLit();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
    // スキンメッシュかつ陰影のあるオブジェクト
	KdShaderManager::Instance().m_HD2DShader.BeginLit_SkinMesh();
	{
		for (auto& obj : m_objList)
		{
			obj->DrawLit_SkinMesh();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit_SkinMesh();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_objList)
		{
			obj->DrawUnLit();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();
}

void BaseScene::DrawSprite()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 2D描画
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		for (auto& obj : m_objList)
		{
			obj->DrawSprite();
		}
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void BaseScene::DrawDebug()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_objList)
		{
			obj->DrawDebug();
		}
		KdShaderManager::Instance().m_HD2DShader.EndUnLit();
	}
}