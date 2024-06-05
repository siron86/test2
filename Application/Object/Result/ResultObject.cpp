#include "ResultObject.h"

void ResultObject::Update()
{
	if (!resultFlg)return;

	clearPos.y += 3.f;
	if (clearPos.y >= 100.f)
	{
		clearPos.y = 100.f;
		endFlg = true;
	}
}

void ResultObject::DrawSprite()
{
	if (!resultFlg)return;
	//設定用変数
	Math::Rectangle rc = { 0,0,453,71 };
	Math::Color     color = { 1.f,1.f,1.f,1.f };
	Math::Vector2   pivot = { 0.5f,1.f };
	//Hpバー描画
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_clearTex, (int)clearPos.x, (int)clearPos.y, 453, 71, &rc, &color, pivot);

	//設定用変数
	rc = { 0,0,1280,720 };
	color = { 1.f,1.f,1.f,0.3f };
	pivot = { 0.5f,0.5f };
	//Hpバー描画
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_backTex, 0, 0, 1280, 720, &rc, &color, pivot);

	//設定用変数
	//rc = { 0,0,611,55 };
	//color = { 1.f,1.f,1.f,0.5f };
	//pivot = { 0.5f,0.5f };
	////Hpバー描画
	//if (endFlg)
	//{
	//	KdShaderManager::Instance().m_spriteShader.DrawTex(m_guideTex, 0, -200, 611, 55, &rc, &color, pivot);
	//}
}

void ResultObject::Init()
{
	//背景画像（半透明）
	m_clearTex = new KdTexture;
	m_clearTex->Load("Asset/Textures/Result/Result.png");
	clearPos = Math::Vector2(0.f, -400.f);

	//ゲームクリア（そのまま）
	m_backTex = new KdTexture;
	m_backTex->Load("Asset/Textures/Result/Back.png");

	//ガイド（そのまま）
	//m_guideTex = new KdTexture;
	//m_guideTex->Load("Asset/Textures/Result/ResultBottn.png");

}

void ResultObject::Release()
{
	m_clearTex->Release();
	delete m_clearTex;
	m_backTex->Release();
	delete m_backTex;
	//m_guideTex->Release();
	//delete m_guideTex;
}