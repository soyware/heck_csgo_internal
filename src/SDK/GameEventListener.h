#pragma once

// A safer method than inheriting straight from IGameEventListener2.
// Avoids requiring the user to remove themselves as listeners in 
// their deconstructor, and sets the serverside variable based on
// our dll location.
class CGameEventListener : public IGameEventListener2
{
public:
	CGameEventListener() : m_bRegisteredForEvents(false)
	{
		m_nDebugID = EVENT_DEBUG_ID_INIT;
	}

	~CGameEventListener()
	{
		m_nDebugID = EVENT_DEBUG_ID_SHUTDOWN;
		StopListeningForAllEvents();
	}

	void ListenForGameEvent(const char* name)
	{
		m_bRegisteredForEvents = true;
		I::GameEventManager->AddListener(this, name, false);
	}

	void ListenForAllGameEvents()
	{
		I::GameEventManager->AddListenerGlobal(this, false);
	}

	void StopListeningForAllEvents()
	{
		if (m_bRegisteredForEvents)
		{
			m_bRegisteredForEvents = false;
			I::GameEventManager->RemoveListener(this);
		}
	}

	// Intentionally abstract
	virtual void FireGameEvent(IGameEvent* event) = 0;

	virtual int GetEventDebugID(void) { return m_nDebugID; }

private:
	int m_nDebugID;
	bool m_bRegisteredForEvents;
};