#pragma once
#include "PlayerState.h"

class CPlayerStun : public IPlayerState
{
public:
	void Init()override;

	void Update()override;
private:
	float m_timer = 0.0f;//時間を図る変数(単位は秒
};