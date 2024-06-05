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

	//‰æ‘œ•\¦
	KdTexture* m_backTex;
	KdTexture* m_clearTex;
	KdTexture* m_guideTex;

	Math::Vector2 clearPos = Math::Vector2::Zero;
	bool resultFlg = false;
	bool endFlg = false;
	//ŒÄ‚Î‚ê‚½‚çî•ñ‚ğW‚ß‚Ä•\¦‚·‚é
};