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

	//�g�k�s��
	Math::Matrix m_scaleMat = Math::Matrix::Identity;
	//��]�s��
	Math::Matrix m_rotateMat = Math::Matrix::Identity;
	//�ړ��s��
	Math::Matrix m_transMat = Math::Matrix::Identity;
};