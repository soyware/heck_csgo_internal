#pragma once

// size limits on Rich Presence data
constexpr int k_cchMaxRichPresenceKeys = 20;
constexpr int k_cchMaxRichPresenceKeyLength = 64;
constexpr int k_cchMaxRichPresenceValueLength = 256;

class ISteamFriends
{
public:
	// Rich Presence data is automatically shared between friends who are in the same game
	// Each user has a set of Key/Value pairs
	// Note the following limits: k_cchMaxRichPresenceKeys, k_cchMaxRichPresenceKeyLength, k_cchMaxRichPresenceValueLength
	// There are five magic keys:
	//		"status"  - a UTF-8 string that will show up in the 'view game info' dialog in the Steam friends list
	//		"connect" - a UTF-8 string that contains the command-line for how a friend can connect to a game
	//		"steam_display"				- Names a rich presence localization token that will be displayed in the viewing user's selected language
	//									  in the Steam client UI. For more info: https://partner.steamgames.com/doc/api/ISteamFriends#richpresencelocalization
	//		"steam_player_group"		- When set, indicates to the Steam client that the player is a member of a particular group. Players in the same group
	//									  may be organized together in various places in the Steam UI.
	//		"steam_player_group_size"	- When set, indicates the total number of players in the steam_player_group. The Steam client may use this number to
	//									  display additional information about a group when all of the members are not part of a user's friends list.
	// GetFriendRichPresence() returns an empty string "" if no value is set
	// SetRichPresence() to a NULL or an empty string deletes the key
	// You can iterate the current set of keys for a friend with GetFriendRichPresenceKeyCount()
	// and GetFriendRichPresenceKeyByIndex() (typically only used for debugging)
	inline void SetRichPresence(const char* pchKey, const char* pchValue)
	{
		typedef void(__thiscall* Fn)(ISteamFriends*, const char*, const char*);
		GetVF<Fn>(this, 43)(this, pchKey, pchValue);
	}
};