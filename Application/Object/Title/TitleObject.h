#pragma once

class TitleObject :public KdGameObject
{
public:
	TitleObject() { Init(); }
	~TitleObject() {}

	void DrawLit() override;
	void Init()    override;

private:

	std::shared_ptr<KdModelWork> m_model;

	//Šgks—ñ
	Math::Matrix m_scaleMat = Math::Matrix::Identity;
	//‰ñ“]s—ñ
	Math::Matrix m_rotateMat = Math::Matrix::Identity;
	//ˆÚ“®s—ñ
	Math::Matrix m_transMat = Math::Matrix::Identity;
};