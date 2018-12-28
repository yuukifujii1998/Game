#include "engineStdafx.h"
#include "GameTime.h"


void CGameTime::Update(CStopWatch& sw)
{
	float limitTime = 1.0f / 60.0f;
	float deltaTime = sw.GetElapsedTime();
	if (deltaTime < limitTime)
	{
		DWORD sleepTime = max(0.0, limitTime * 1000.0 - (DWORD)sw.GetElapsedTimeMill());
		Sleep(sleepTime);
		deltaTime = limitTime;
	}
	deltaTime = min(1.0f / 10.0f, deltaTime);

	m_frameDeltaTime = deltaTime;
	m_gameDeltaTime = deltaTime;
	if (0.0f < m_slowDelayTime || 0 < m_slowDelayCount )
	{

		m_slowDelayTime -= deltaTime;
		m_slowDelayCount--;
	}
	else
	{
		if (0.0f < m_slowTime || 0 < m_slowCount)
		{
			m_gameDeltaTime *= m_slowScale;
			m_slowTime -= deltaTime;
			m_slowCount--;
		}
	}
	
}