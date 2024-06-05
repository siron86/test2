#include "UiBase.h"
#include"PlayerUi/PlayerUi.h"

void UiBase::Update()
{
	if (!m_popFlg)return;
	for (auto& ui : m_uiList)
	{
		ui->Update();
	}

}

void UiBase::PostUpdate()
{
	for (auto& ui : m_uiList)
	{
		ui->PostUpdate();
	}

}

void UiBase::DrawSprite()
{
	if (!m_popFlg)return;
	for (auto& ui : m_uiList)
	{
		ui->DrawSprite();
	}
}

void UiBase::Init()
{
	std::shared_ptr<PlayerUi>plaUi = std::make_shared<PlayerUi>();
	plaUi->GetPlayer(m_wpPla);
	m_uiList.push_back(plaUi);
}

void UiBase::SetTexData()
{
	//for (auto& ui : m_uiList)
	//{
	//	ui->PostUpdate();
	//}
}

void UiBase::Release()
{
	for (auto& ui : m_uiList)
	{
		ui->Release();
	}
}