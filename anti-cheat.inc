/*
	Make functions that sets the values both server side and client side.

	You're able to create hooks for any event listener. E.g: AC_OnPlayerConnect.

	Please define the ID and string equivilent of the cheat. E.g
	#define AC_CHEAT_HEALTH			0

	and the string inside the array, the index of the specified cheat must be the same ID as the defined preprocessor ID.
*/

#define AC_CHEAT_WEAPONS 			1

new AC_timer;
new AC_names[][] =
{
	"Weapon/Ammo hacks"
};

// Create your variables here.
new PlayerWeapons[MAX_PLAYERS][12][2];

forward AC_OnGameModeInit();
public AC_OnGameModeInit()
{
	AC_timer = SetTimer("OnAnticheatCheck", 1000, true);
	return 1;
}

forward AC_OnGameModeExit();
public AC_OnGameModeExit()
{
	KillTimer(AC_timer);
	return 1;
}

forward OnAnticheatCheck();
public OnAnticheatCheck()
{
	new weapon, ammo;
	for(new i = 0; i < 12; i++)
	{
		GetPlayerWeaponData(playerid, i, weapon, ammo);
		if(PlayerWeapons[playerid][i][0] != weapon || PlayerWeapons[playerid][i][1] != ammo)
		{
			OnAnticheatDetect(playerid, AC_CHEAT_WEAPONS);
		}
	}
	return 1;
}

#if !defined OnAnticheatDetect
	forward OnAnticheatDetect(playerid, cheatid);
	public OnAnticheatDetect(playerid, cheatid)
	{
		return 1;
	}
#endif


forward AC_OnPlayerConnect(playerid);
public AC_OnPlayerConnect(playerid)
{
	// Resetting the Server-sided weapons.
	for(new i = 0; i < 12; i++)
	{
		PlayerWeapons[playerid][i][0] = 0;
		PlayerWeapons[playerid][i][1] = 0;
	}
	return 1;
}

// Functions here...
stock GivePlayerWeaponEx(playerid, weapon, ammo)
{
	PlayerWeapons[playerid][i][0] = weapon;
	PlayerWeapons[playerid][i][1] = ammo;
	GivePlayerWeapon(playerid, weapon, ammo);
	return 1;
}