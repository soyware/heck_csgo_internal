#pragma once

namespace Features
{
	namespace Misc
	{
		namespace Events
		{
			void OnConnect()
			{
				Settings::SkyNameSetOriginal();
				Settings::SkyNameChanged(nullptr, nullptr, 0.f);
				Misc::Log::ResetTeamDmg();
			}

			static class : public CGameEventListener
			{
				virtual void FireGameEvent(IGameEvent* event)
				{
					const char* eventName = event->GetName();

					if (!strcmp(eventName, "bullet_impact"))
					{
						if (Settings::Visuals::ShowImpacts::Enable.GetInt())
							ShowImpacts(event->GetInt("userid"), vec3(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z")));
					}
					else if (!strcmp(eventName, "player_hurt"))
					{
						const int attackerIndex = I::EngineClient->GetPlayerForUserID(event->GetInt("attacker"));
						const int victimIndex = I::EngineClient->GetPlayerForUserID(event->GetInt("userid"));
						const int damage = event->GetInt("dmg_health");
						const int health = event->GetInt("health");

						if (Settings::Misc::Log::TeamDmg.GetInt())
							Log::TeamDmg(attackerIndex, victimIndex, damage, health);

						// next events are only if we are the attacker
						if (I::EngineClient->GetLocalPlayer() != attackerIndex)
							return;

						if (Settings::Misc::Log::Hit.GetInt())
							Log::Hit(victimIndex, event->GetInt("hitgroup"), damage, health);

						if (Settings::Misc::HitSound::Enable.GetInt())
							HitSound();
					}
					else if (!strcmp(eventName, "item_purchase"))
					{
						if (Settings::Misc::Log::Buy.GetInt())
							Log::Buy(event->GetInt("userid"), event->GetInt("team"), event->GetString("weapon"));
					}
					else if (!strcmp(eventName, "player_death"))
					{
						if (Settings::Misc::DeathSound::Enable.GetInt())
							DeathSound(event->GetInt("userid"), event->GetInt("attacker"));
					}
					else if (!strcmp(eventName, "vote_cast"))
					{
						VoteRevealer(event->GetInt("entityid"), event->GetInt("vote_option"));
					}
					else if (!strcmp(eventName, "server_spawn"))
					{
						OnConnect();
					}
				}
			} Listener;

			void Init()
			{
				Listener.ListenForGameEvent("bullet_impact");
				Listener.ListenForGameEvent("player_hurt");
				Listener.ListenForGameEvent("item_purchase");
				Listener.ListenForGameEvent("player_death");
				Listener.ListenForGameEvent("vote_cast");
				Listener.ListenForGameEvent("server_spawn");
			}

			void Unregister()
			{
				Listener.StopListeningForAllEvents();
			}
		}
	}
}