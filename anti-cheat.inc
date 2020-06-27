
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

forward OnAnticheatDetect(playerid, cheatid);

new AC_timer; // Holds the timer for the Anti-cheat checking function.
new AC_names[][] = // Holds names for the Anti-cheat.
{
	"Money hack",
	"Weapon/Ammo hack",
	"Health/Armor hack"
};

// Server-sided variables.
new PlayerWeapons[MAX_PLAYERS][12][2]; // Server-sided player weapons
new PlayerMoney [MAX_PLAYERS]; // Server-sided player money.

new Float:PlayerHealth[MAX_PLAYERS]; // Server-sided player weapons
new Float:PlayerArmor[MAX_PLAYERS]; // Server-sided player health.

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

	new weapon, ammo, Float:health, Float:armor; // Used to get weapon data.

	for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
	{
		GetPlayerHealth(playerid, health); // Gets client-side health.
		GetPlayerArmour(playerid, armor); // Gets client-side armor.

		for(new j = 0; j < 12; j++)
		{
			GetPlayerWeaponData(playerid, j, weapon, ammo); // Gets the client-sided weapon.
			if(PlayerWeapons[playerid][j][0] != weapon || PlayerWeapons[playerid][j][1] != ammo) // If the client-side and server-side doesn't match...
			{
				OnAnticheatDetect(playerid, AC_CHEAT_WEAPONS);
			}
		}
		if(PlayerHealth[playerid] != health && PlayerArmor[playerid] != armor)
		{
			OnAnticheatDetect(playerid, AC_CHEAT_HEALTH_ARMOR);
		}
		if (GetPlayerMoney (playerid) != PlayerMoney [playerid]) // Checks if the client-side and server-side money doesn't match.
		{
			OnAnticheatDetect (playerid, AC_CHEAT_MONEY);
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
	PlayerMoney[playerid] = 0; // Sets the player server-sided money to 0.

	PlayerHealth[playerid] = 100.0;
	PlayerHealth[playerid] = 0.0;
	return 1;
}
forward AC_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
public AC_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	new Float:armor, Float:health;
	GetPlayerArmour(playerid, armor);
	GetPlayerHealth(playerid, health);

	if(armor < amount)
	{
		PlayerArmor[playerid] = 0.0;

		new Float:hamount = amount - armor;
		PlayerHealth[playerid] = hamount;
	}
	else PlayerArmor[playerid] = armor - amount;
	return 1;
}

/*
	Functions needed for each Anti-cheat.
*/

// Money
stock GivePlayerMoneyEx (playerid, money) 
{
	PlayerMoney [playerid] += money;
	GivePlayerMoney (playerid, money);
	return 1;
}

// Weapons
stock GivePlayerWeaponEx(playerid, weapon, ammo)
{
	stock GetWeaponSlot(weaponid)
{
	new slot;
	switch(weapon)
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
	PlayerWeapons[playerid][slot][0] = weapon;
	PlayerWeapons[playerid][slot][1] = ammo;
	GivePlayerWeapon(playerid, weapon, ammo);
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
