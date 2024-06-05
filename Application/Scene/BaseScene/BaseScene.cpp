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
	//�I�u�W�F�N�g��Update
	for (auto& obj : m_objList)
	{
		obj->Update();
	}

	Event();
}

void BaseScene::PostUpdate()
{
	//�ŏI�X�V
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
	// �����Ղ�I�u�W�F�N�g
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& obj : m_objList)
		{
			obj->GenerateDepthMapFromLight();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight();

	// �X�L�����b�V���������Ղ�I�u�W�F�N�g
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight_SkinMesh();
	{
		for (auto& obj : m_objList)
		{
			obj->GenerateDepthMapFromLight_SkinMesh();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight_SkinMesh();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂���I�u�W�F�N�g
	KdShaderManager::Instance().m_HD2DShader.BeginLit();
	{
		//�I�u�W�F�N�g�`��
		for (auto& obj : m_objList)
		{
			obj->DrawLit();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
    // �X�L�����b�V�����A�e�̂���I�u�W�F�N�g
	KdShaderManager::Instance().m_HD2DShader.BeginLit_SkinMesh();
	{
		for (auto& obj : m_objList)
		{
			obj->DrawLit_SkinMesh();
		}
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit_SkinMesh();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �A�e�̂Ȃ��I�u�W�F�N�g
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
	// 2D�`��
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
	// �A�e�̂���I�u�W�F�N�g
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
		for (auto& obj : m_objList)
		{
			obj->DrawDebug();
		}
		KdShaderManager::Instance().m_HD2DShader.EndUnLit();
	}
}