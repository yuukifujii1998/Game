#include "stdafx.h"
#include "Menu.h"
#include "../../Player/Player.h"
#include "../../Map/Map.h"
#include "../../Scene/SceneManager.h"
#include "../../Enemy/IEnemy.h"

CMenu::CMenu()
{
}


CMenu::~CMenu()
{
}


void CMenu::Init()
{
	m_Texture.Load(L"Assets/sprite/Menu.png");
	m_menu.Init(&m_Texture);
	m_menu.SetPosition({ -1.0f, -1.0f });
	m_menu.SetSize({ 1290.0f,720.0f });

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_numberTexture[i][j] = new CTexture;
			m_number[i][j] = new CSprite;
			m_numberTexture[i][j] = TextureResource().LoadTexture(L"Assets/sprite/number/number0.png");
			m_number[i][j]->Init(m_numberTexture[i][j]);
			m_number[i][j]->SetPosition({ m_numberPos});
			m_number[i][j]->SetSize({ 65.0f,65.0f });
			m_numberPos.x += 30.0;
		}
		if(i==4)
		{ 
			m_numberPos.y -= 43.0f;
		}
		m_numberPos.x = 500.0f;
		m_numberPos.y -= 57.0f;
	}
}


void CMenu::Update()
{
	if (GetPlayer().GetIsDied()) { return; }
	if (Pad().IsTriggerButton(enButtonSelect))
	{
		if (m_Draw)
		{

			m_Draw = false;
		}
		else
		{
			m_Draw = true;

		}
		std::list<IEnemy*> enemyList = GetSceneManager().GetGameScene().GetMap()->GetEnemyList();
		//敵のアクティブ設定
		for (auto& enemy : enemyList)
		{
			enemy->SetIsActive(!m_Draw);

		}
		//プレイヤーの動きの設定
		GetPlayer().PlayerMoveSetIsActive(!m_Draw);
	}


	 //プレイヤーのステータスを格納
	 PlayerStatusInput();

	if (Pad().IsTriggerButton(enButtonSelect))
	{

		StatusMath();
	}
}

void CMenu::PlayerStatusInput()
{
	m_PlayerStatus[0] = GetPlayer().GetStatus().Health;
	m_PlayerStatus[1] = GetPlayer().GetStatus().Level;
	m_PlayerStatus[2] = GetPlayer().GetStatus().Strength;
	m_PlayerStatus[3] = GetPlayer().GetStatus().Defense;
	m_PlayerStatus[4] = GetPlayer().GetStatus().ExperiencePoint;
	m_PlayerStatus[5] = GetPlayer().GetStatus().NextExp;
	m_PlayerStatus[6] = GetPlayer().GetStatus().Gold;

}


//メニュー画面に表示されるプレイヤーのステータスの数値を決める処理
void CMenu::StatusMath()
{


	for (int i = 0; i < 7; i++)
	{
		
			
			wchar_t filePath[256];
			//1000の位
			int Box = m_PlayerStatus[i] / 1000;
			swprintf(filePath, L"Assets/sprite/number/number%d.png", Box);
			{
				if (Box == 0)
				{
					m_number[i][0]->SetIsDraw(false);
				}
				else
				{

					m_numberTexture[i][0] = m_numberTexture[i][0] = TextureResource().LoadTexture(filePath);
					m_number[i][0]->SetTexture(m_numberTexture[i][0]);
					m_number[i][0]->SetSize({ 65.0f,65.0f });
					m_PlayerStatus[i] %= 1000;
					m_number[i][0]->SetIsDraw(true);
				}

			}

			//100の位
			Box = m_PlayerStatus[i] / 100;
			swprintf(filePath, L"Assets/sprite/number/number%d.png",Box );
			{
				if (Box == 0 && !m_number[i][0]->IsDraw())
				{
					m_number[i][1]->SetIsDraw(false);
				}
				else
				{

					m_numberTexture[i][1] = m_numberTexture[i][1] = TextureResource().LoadTexture(filePath);
					m_number[i][1]->SetTexture(m_numberTexture[i][1]);
					m_number[i][1]->SetSize({ 65.0f,65.0f });
					m_PlayerStatus[i] %= 100;
					m_number[i][1]->SetIsDraw(true);
				}
			}

			//10の位
			Box = m_PlayerStatus[i] / 10;
			swprintf(filePath, L"Assets/sprite/number/number%d.png", Box);
			{
				if (Box == 0 && !m_number[i][1]->IsDraw())
				{
					m_number[i][2]->SetIsDraw(false);
				}
				else
				{

					m_numberTexture[i][2] = m_numberTexture[i][2] = TextureResource().LoadTexture(filePath);
					m_number[i][2]->SetTexture(m_numberTexture[i][2]);
					m_number[i][2]->SetSize({ 65.0f,65.0f });
					m_PlayerStatus[i] %= 10;
					m_number[i][2]->SetIsDraw(true);
				}

			}

			//1の位
			swprintf(filePath, L"Assets/sprite/number/number%d.png", m_PlayerStatus[i]);
			m_numberTexture[i][3] = m_numberTexture[i][3] = TextureResource().LoadTexture(filePath);
			m_numberTexture[i][3]->Load(filePath);
			m_number[i][3]->SetTexture(m_numberTexture[i][3]);
			m_number[i][3]->SetSize({ 65.0f,65.0f });

			


	
	}


}

//メニュー画面の描画処理
void CMenu::AfterDraw()
{
	if (GetPlayer().GetIsDied()) { return; }
	if (m_Draw)
	{
		m_menu.Draw();
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 4; j++)
			{

				m_number[i][j]->Draw();
			}
		}


		

	}

}