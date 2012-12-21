
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "AggressiveMonster.h"

#include "../Vector3f.h"
#include "../Player.h"
#include "../MersenneTwister.h"





cAggressiveMonster::cAggressiveMonster()
	: m_ChaseTime(999999)
{
	m_EMPersonality = AGGRESSIVE;
}





// What to do if in Chasing State
void cAggressiveMonster::InStateChasing(float a_Dt)
{
	super::InStateChasing(a_Dt);
	m_ChaseTime += a_Dt;
	if (m_Target != NULL)
	{
		if (m_Target->IsPlayer())
		{
			cPlayer * Player = (cPlayer *) m_Target;
			if (Player->GetGameMode() == 1)
			{
				m_EMState = IDLE;
				return;
			}
		}

		Vector3f Pos = Vector3f( m_Pos );
		Vector3f Their = Vector3f( m_Target->GetPosition() );
		if ((Their - Pos).Length() <= m_AttackRange)
		{
			cMonster::Attack(a_Dt);
		}
		MoveToPosition(Their + Vector3f(0, 0.65f, 0));
	}
	else if (m_ChaseTime > 5.f)
	{
		m_ChaseTime = 0;
		m_EMState = IDLE;
	}	
} 





void cAggressiveMonster::EventSeePlayer(cEntity * a_Entity)
{
	super::EventSeePlayer(a_Entity);
	m_EMState = CHASING;
}





void cAggressiveMonster::Tick(float a_Dt)
{
	super::Tick(a_Dt);

	m_SeePlayerInterval += a_Dt;

	if (m_SeePlayerInterval > 1)
	{
		MTRand r1;
		int rem = r1.randInt() % 3 + 1;  // Check most of the time but miss occasionally

		m_SeePlayerInterval = 0.0;
		if (rem >= 2)
		{
			if (m_EMState == CHASING)
			{
				CheckEventLostPlayer();
			}
			else
			{
				CheckEventSeePlayer();
			}
		}
	}
}




