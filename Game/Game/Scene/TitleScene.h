#pragma once
class CTitleScene :public IGameObject
{
public:
	//初期化
	void Init();

	//更新
	void Update();

	//遅延描画
	void AfterDraw();

private:
	CSprite			m_title;					//Title画面のスプライト
	CTexture		m_titleTexture;				//Title画面のテクスチャ
};

