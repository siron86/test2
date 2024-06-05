#pragma once

class SelectSObject :public KdGameObject
{
public:
	SelectSObject() { Init(); }
	~SelectSObject() {}

	void Update()override;
	void PostUpdate()override;
	void DrawLit() override;
	void Init()    override;

private:

	std::shared_ptr<KdModelWork> m_model;

	Math::Vector3 m_ang = Math::Vector3::Zero;

	//�g�k�s��
	Math::Matrix m_scaleMat = Math::Matrix::Identity;
	//��]�s��
	Math::Matrix m_rotateMat = Math::Matrix::Identity;
	//�ړ��s��
	Math::Matrix m_transMat = Math::Matrix::Identity;
};