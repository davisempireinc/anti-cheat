
/*
	Make functions that sets the values both server side and client side.
	Please define the ID and string equivilent of the cheat. E.g
	#define AC_CHEAT_HEALTH_ARMOR		0
	and the string inside the array, the index of the specified cheat must be the same ID as the defined preprocessor ID.
*/
// Defining cheat ID's

#define AC_CHEAT_MONEY	      		0
#define AC_CHEAT_WEAPONS 			1
#define AC_CHEAT_HEALTH_ARMOR		2
#define AC_CHEAT_POSITION			3

forward OnAnticheatDetect(playerid, cheatid);

new AC_timer; // Holds the timer for the Anti-cheat checking function.
new AC_names[][] = // Holds names for the Anti-cheat.
{
	"Money hack",
	"Weapon/Ammo hack",
	"Health/Armor hack",
	"TP/Speed hack"
};

#pragma unused AC_names

// Server-sided variables.
new IsDead[MAX_PLAYERS];

new PlayerMoney [MAX_PLAYERS]; // Server-sided player money.
new PlayerWeapons[MAX_PLAYERS][12][2]; // Server-sided player weapons

new Float:PlayerHealth[MAX_PLAYERS]; // Server-sided player weapons
new Float:PlayerArmor[MAX_PLAYERS]; // Server-sided player health.

new Float:PlayerPosition[MAX_PLAYERS][3]; // Server-sided player position.

GetWeaponSlot(weaponid)
{
	new slot;
	switch(weaponid)
	{
		case 0,1: slot = 0;
		case 2 .. 9: slot = 1;
		case 10 .. 15: slot = 10;
		case 16 .. 18, 39: slot = 8;
		case 22 .. 24: slot =2;
		case 25 .. 27: slot = 3;
		case 28, 29, 32: slot = 4;
		case 30, 31: slot = 5;
		case 33, 34: slot = 6;
		case 35 .. 38: slot = 7;
		case 40: slot = 12;
		case 41 .. 43: slot = 9;
		case 44 .. 46: slot = 11;
	}
	return slot;
}

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

	new weapon, ammo;// Used to get weapon data.
	new Float:health, Float:armor; // Used to get client-side health & armor.
	new Float:X, Float:Y, Float:Z; // Used to get client-side position.

	for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
	{
		if(IsDead[playerid] == 1 || GetPVarInt(playerid, "ready") != 1) continue;

		GetPlayerHealth(playerid, health); // Gets client-side health.
		GetPlayerArmour(playerid, armor); // Gets client-side armor.

		if((floatround(PlayerHealth[playerid], floatround_round) != floatround(health, floatround_round)) || 
			(floatround(PlayerArmor[playerid], floatround_round) != floatround(armor, floatround_round)))
		{
			OnAnticheatDetect(playerid, AC_CHEAT_HEALTH_ARMOR);
		}
		if (GetPlayerMoney (playerid) != PlayerMoney [playerid]) // Checks if the client-side and server-side money doesn't match.
		{
			OnAnticheatDetect (playerid, AC_CHEAT_MONEY);
		}
		if((!IsPlayerInRangeOfPoint(playerid, 20.0, 
			PlayerPosition[playerid][0], 
			PlayerPosition[playerid][1], 
			PlayerPosition[playerid][2]) && !IsPlayerInAnyVehicle(playerid)) || 

			(!IsPlayerInRangeOfPoint(playerid, 70.0, 
			PlayerPosition[playerid][0], 
			PlayerPosition[playerid][1], 
			PlayerPosition[playerid][2])) && IsPlayerInAnyVehicle(playerid))
		{
			OnAnticheatDetect (playerid, AC_CHEAT_POSITION);
		}
		else
		{
			GetPlayerPos(playerid, X, Y, Z); // Gets client-side position.
			PlayerPosition[playerid][0] = X;
			PlayerPosition[playerid][1] = Y;
			PlayerPosition[playerid][2] = Z;
		}
		for(new j = 0; j < 12; j++)
		{
			GetPlayerWeaponData(playerid, j, weapon, ammo); // Gets the client-sided weapon.
			if(PlayerWeapons[playerid][j][0] != weapon || (PlayerWeapons[playerid][j][1] > ammo + 4 && PlayerWeapons[playerid][j][1] < ammo - 4)) // If the client-side and server-side doesn't match...
			{
				OnAnticheatDetect(playerid, AC_CHEAT_WEAPONS);
				break;
			}
		}
	}
	return 1;
}

/*
	Server events.
	You're able to create hooks for any event listener. E.g: AC_OnPlayerConnect.
*/
forward AC_OnPlayerConnect(playerid);
public AC_OnPlayerConnect(playerid)
{
	// Resetting the Server-sided weapons.
	for(new slot = 0; slot < 12; slot++)
	{
		PlayerWeapons[playerid][slot][0] = 0;
		PlayerWeapons[playerid][slot][1] = 0;
	}
	PlayerMoney[playerid] = 0;

	PlayerHealth[playerid] = 100.0;
	PlayerArmor[playerid] = 0.0;

	IsDead[playerid] = 0;
	return 1;
}

forward AC_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
public AC_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	new Float:armor;
	GetPlayerArmour(playerid, armor);

	if(armor < amount)
	{
		PlayerArmor[playerid] = 0.0;

		new Float:hamount = PlayerHealth[playerid] - (amount - armor);
		PlayerHealth[playerid] = hamount;
	}
	else PlayerArmor[playerid] = armor - amount;
	return 1;
}
forward AC_OnPlayerWeaponShot(playerid, weaponid);
public AC_OnPlayerWeaponShot(playerid, weaponid)
{
	new slot = GetWeaponSlot(weaponid);
	PlayerWeapons[playerid][slot][1]--;

	if(PlayerWeapons[playerid][slot][1] == 0)
	{
		RemovePlayerWeapon(playerid, weaponid);
	}
	return 1;
}

forward AC_OnPlayerDeath(playerid);
public AC_OnPlayerDeath(playerid)
{
	IsDead[playerid] = 1;
	return 1;
}

forward AC_OnPlayerSpawn(playerid);
public AC_OnPlayerSpawn(playerid)
{
	SetPlayerHealthEx(playerid, 100.0);
	SetPlayerArmorEx(playerid, 0.0);
	ResetPlayerWeapons(playerid);

	IsDead[playerid] = 0;
	return 1;
}
/*
	Functions needed for each Anti-cheat.
*/

// Money
stock GivePlayerMoneyEx (playerid, money) 
{
	PlayerMoney[playerid] += money;
	GivePlayerMoney (playerid, money);
	return 1;
}
stock SetPlayerMoney(playerid, money)
{
	PlayerMoney[playerid] = money;
	GivePlayerMoney(playerid, money - GetPlayerMoney(playerid));
	return 1;
}

// Weapons
stock GivePlayerWeaponEx(playerid, weapon, ammo)
{
	new slot = GetWeaponSlot(weapon);
	if(PlayerWeapons[playerid][slot][0] == weapon) PlayerWeapons[playerid][slot][1] += ammo;
	else 
	{
		PlayerWeapons[playerid][slot][0] = weapon;
		PlayerWeapons[playerid][slot][1] = ammo;
	}
	GivePlayerWeapon(playerid, weapon, ammo);
	return 1;
}
stock ResetPlayerWeaponsEx(playerid)
{
	ResetPlayerWeapons(playerid);
	for(new slot = 0; slot < 12; slot++)
	{
		PlayerWeapons[playerid][slot][0] = 0;
		PlayerWeapons[playerid][slot][1] = 0;
	}
	return 1;
}
stock RemovePlayerWeapon(playerid, weapon)
{
	new weapons[13][2];
	for (new i = 0; i <= 12; i++)
	{
	    GetPlayerWeaponData(playerid, i, weapons[i][0], weapons[i][1]);
	}
	ResetPlayerWeaponsEx(playerid);
	for(new i = 0; i <= 12; i++)
	{
		if(weapons[i][0] != weapon) GivePlayerWeaponEx(playerid, weapons[i][0], weapons[i][1]);
	}
	return 1;
}

// Health/Armor
stock SetPlayerHealthEx(playerid, Float:health)
{
	SetPlayerHealth(playerid, health);
	PlayerHealth[playerid] = health;
	return 1;
}
stock SetPlayerArmorEx(playerid, Float:armor)
{
	SetPlayerArmour(playerid, armor);
	PlayerArmor[playerid] = armor;
	return 1;
}

// Position
stock SetPlayerPosEx(playerid, Float:X, Float:Y, Float:Z)
{
	IsDead[playerid] = 1;
	PlayerPosition[playerid][0] = X;
	PlayerPosition[playerid][1] = Y;
	PlayerPosition[playerid][2] = Z;
	SetPlayerPos(playerid, X, Y, Z);
	SetTimerEx("RemoveDead", 500, false, "i", playerid);
	return 1;
}

forward RemoveDead(playerid);
public RemoveDead(playerid)
{
	IsDead[playerid] = 0;
	return 1;
}