#include "stdafx.h"
#include "../Scene/GameScene.h"
#include "Map/Map.h"
#include "../Player/Player.h"
#include "../UI/MiniMap/MiniMap.h"
#include "../UI/WeaponSelect/WeaponSelect.h"
#include "../UI/PlayerHp/PlayerHp.h"
void CGameScene::Init()

{
	map = New<Map>(0);
	map->Init(0);

	CMiniMap* miniMap = New<CMiniMap>(0);
	miniMap->Init();

	damageNumber.Init();

	CWeaponSelect* weaponSelect = New<CWeaponSelect>(0);
	weaponSelect->Init();

	PlayerHp* playerHp = New<PlayerHp>(0);
	playerHp->Init();
}

void CGameScene::Update()
{
	//damageNumber.DamageCalculation(&GetPlayer().GetInstance(), map->GetEnemy());
}
