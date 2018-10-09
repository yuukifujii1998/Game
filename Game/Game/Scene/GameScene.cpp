#include "stdafx.h"
#include "../Scene/GameScene.h"
#include "Map/Map.h"
#include "../Player/Player.h"
#include "../UI/MiniMap/MiniMap.h"
#include "../UI/WeaponSelect/WeaponSelect.h"
#include "../UI/PlayerHp/PlayerHp.h"
#include "../UI/Menu/Menu.h"
#include "../UI/Result/Result.h"
#include "../UI/LevelUp/LevelUp.h"
#include"../../Game/Camera/GameCamera.h"

void CGameScene::Init()
{
	map = New<Map>(0);
	map->Init(0);
	
	CMiniMap* miniMap = New<CMiniMap>(0);
	miniMap->Init();

	GetGameCamera().Init();

	m_weaponSelect = New<CWeaponSelect>(0);
	m_weaponSelect->Init();

	CPlayerHp* playerHp = New<CPlayerHp>(0);
	playerHp->Init();

	CLevelUp* levelUp = New<CLevelUp>(0);
	levelUp->Init();

	CMenu* menu = New<CMenu>(0);
	menu->Init();

	CResult* result = New<CResult>(0);
	result->Init();
}

void CGameScene::Update()
{
}
