#pragma once

class ResultObject :public KdGameObject
{
public:
	ResultObject() { Init(); }
	~ResultObject() { Release(); }
	
	void Update()override;
	void DrawSprite()override;
	void Init()override;

	void PlayResultFlg(bool _flg) { resultFlg = _flg; }

private:
	void Release();

	//�摜�\��
	KdTexture* m_backTex;
	KdTexture* m_clearTex;
	KdTexture* m_guideTex;

	Math::Vector2 clearPos = Math::Vector2::Zero;
	bool resultFlg = false;
	bool endFlg = false;
	//�Ă΂ꂽ������W�߂ĕ\������
};