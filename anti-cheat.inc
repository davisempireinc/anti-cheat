/*
	Make functions that sets the values both server side and client side.
	You're able to create hooks for any event listener. E.g: AC_OnPlayerConnect.
	Please define the ID and string equivilent of the cheat. E.g
	#define AC_CHEAT_HEALTH			0
	and the string inside the array, the index of the specified cheat must be the same ID as the defined preprocessor ID.
*/
// Defining cheat ID's

#define AC_CHEAT_MONEY	      0
#define AC_CHEAT_WEAPONS 			1

new AC_timer; // Holds the timer for the Anti-cheat checking function.
new AC_names[][] = // Holds names for the Anti-cheat.
{
	"Money Hack",
	"Weapon/Ammo hacks"
};

// Server-sided variables.
new PlayerWeapons[MAX_PLAYERS][12][2]; // Server-sided player weapons
new PlayerMoney [MAX_PLAYERS]; // Server-sided player money.

forward AC_OnGameModeInit();
public AC_OnGameModeInit()
{
	AC_timer = SetTimer("OnAnticheatCheck", 1000, true); // Starting the timer.
	return 1;
}

forward AC_OnGameModeExit();
public AC_OnGameModeExit()
{
	KillTimer(AC_timer); // Killing the timer, 'cause why not?
	return 1;
}

forward OnAnticheatCheck();
public OnAnticheatCheck()
{
	// Checking happens here.
	// Calls the OnAnticheatDetect() with the defined preprocessor ID for the cheat.

	new weapon, ammo; // Used to get weapon data.
	for(new i = 0; i < 12; i++)
	{
		GetPlayerWeaponData(playerid, i, weapon, ammo); // Gets the client-sided weapon.
		if(PlayerWeapons[playerid][i][0] != weapon || PlayerWeapons[playerid][i][1] != ammo) // If the client-side and server-side doesn't match...
		{
			OnAnticheatDetect(playerid, AC_CHEAT_WEAPONS);
		}
	}
	if (GetPlayerMoney (playerid) != PlayerMoney [playerid]) // Checks if the client-side and server-side money doesn't match.
	{
		OnAnticheatDetect (playerid, AC_CHEAT_MONEY);
	}
	return 1;
}

// Used as a place holder incase one didn't create his own implementation of OnAnticheatDetect().
forward OnAnticheatDetect(playerid, cheatid);
#if !defined OnAnticheatDetect
	public OnAnticheatDetect(playerid, cheatid)
	{
		return 1;
	}
#endif

// Anti-cheat connect event.
forward AC_OnPlayerConnect(playerid);
public AC_OnPlayerConnect(playerid)
{
	// Resetting the Server-sided weapons.
	for(new i = 0; i < 12; i++)
	{
		PlayerWeapons[playerid][i][0] = 0;
		PlayerWeapons[playerid][i][1] = 0;
	}
	PlayerMoney[playerid] = 0; // Sets the player server-sided money to 0.
	return 1;
}

// Anti-cheat disconnect event.
forward AC_OnPlayerDisconnect(playerid, reason);
public AC_OnPlayerDisconnect(playerid, reason)
{
	return 1;
}

// Functions here...
stock GivePlayerMoneyEx (playerid, money) 
{
	PlayerMoney [playerid] += money;
	GivePlayerMoney (playerid, money);
	return 1;
}
stock GivePlayerWeaponEx(playerid, weapon, ammo)
{
	PlayerWeapons[playerid][i][0] = weapon;
	PlayerWeapons[playerid][i][1] = ammo;
	GivePlayerWeapon(playerid, weapon, ammo);
	return 1;
}
