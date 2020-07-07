
/*
	Make functions that sets the values both server side and client side.
	Please define the ID and string equivilent of the cheat. E.g
	#define AC_CHEAT_HEALTH_ARMOR		0
	and the string inside the array, the index of the specified cheat must be the same ID as the defined preprocessor ID.
*/
// Defining cheat ID's

#define AC_CHEAT_POSITION			0
#define AC_CHEAT_FLOOD_1			1
#define AC_CHEAT_FLYING				2
#define AC_CHEAT_SPEED				3

#define FLOOD_LIMIT 27

forward OnAnticheatDetect(playerid, cheatid);

new AC_timer; // Holds the timer for the Anti-cheat checking function.
new AC_names[][] = // Holds names for the Anti-cheat.
{
	"TP/Speed hack",
	"Bullet flood hack",
	"Flying hack"
};

#pragma unused AC_names

// Server-sided variables.
new IsDisableAnticheat[MAX_PLAYERS];
new IsDetected[MAX_PLAYERS];

new WeaponShot [MAX_PLAYERS];
new FloodTimer [MAX_PLAYERS];

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
	new Float:X, Float:Y, Float:Z, Float:minZ; // Used to get client-side position.
	new Float:velX, Float:velY, Float:velZ; // Used to get the speed of the player.
	for(new playerid = 0, count = GetPlayerPoolSize(); playerid <= count; playerid++)
	{

		if(IsDetected[playerid] == 0)
		{
			if(IsDisableAnticheat[playerid] != 1)
			{
				GetPlayerPos(playerid, X, Y, Z);
				if(GetPlayerVehicleID(playerid) == 0)
				{
					CA_FindZ_For2DCoord(X, Y, minZ);
					GetPlayerVelocity(playerid, velX, velY, velZ);
					if(Z - minZ >= 5.0 && GetPlayerInterior(playerid) == 0)
					{
						OnAnticheatDetect (playerid, AC_CHEAT_FLYING);
						IsDetected[playerid] = 1;
						SetTimerEx("UndetectPlayer", 10000, false, "i", playerid);			
					}
				}
				
			}
		}
	}
	return 1;
}

forward AC_OnPlayerUpdate(playerid);
public AC_OnPlayerUpdate(playerid)
{
	// Checking happens here.
	// Calls the OnAnticheatDetect() with the defined preprocessor ID for the cheat.

	new weapon, ammo;// Used to get weapon data.
	if(IsDisableAnticheat[playerid] != 1)
	{
		GivePlayerMoney(playerid, PlayerMoney[playerid] - GetPlayerMoney(playerid));
		SetPlayerHealth(playerid, PlayerHealth[playerid]);
		SetPlayerArmour(playerid, PlayerArmor[playerid]);

		for(new j = 0; j < 12; j++)
		{
			GetPlayerWeaponData(playerid, j, weapon, ammo); // Gets the client-sided weapon.
			if(PlayerWeapons[playerid][j][0] != weapon) // If the client-side and server-side doesn't match...
			{
				if(PlayerWeapons[playerid][j][0] == 0)
				{
					RemovePlayerWeapon(playerid, weapon);
				}
				else
				{
					GivePlayerWeapon(playerid, PlayerWeapons[playerid][j][0], PlayerWeapons[playerid][j][1]);
				}
			} 
			else if(PlayerWeapons[playerid][j][1] > ammo + 4 || PlayerWeapons[playerid][j][1] < ammo - 4)
			{
				SetPlayerAmmo(playerid, j, PlayerWeapons[playerid][j][1]);
			}
		}
	}
	return 1;
}

forward UndetectPlayer(playerid);
public UndetectPlayer(playerid)
{
	IsDetected[playerid] = 0;
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

	IsDetected[playerid] = 0;

	TogglePlayerAnticheat(playerid, false);
	return 1;
}

forward AC_OnPlayerDisconnect (playerid);
public AC_OnPlayerDisconnect (playerid)
{
	if (WeaponShot [playerid])
	{
		WeaponShot [playerid] = 0;
		KillTimer (FloodTimer [playerid]);
	}
	IsDetected[playerid] = 0;
	return 1;
}

forward AC_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
public AC_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	new Float:armor, Float:health;
	GetPlayerArmour(playerid, armor);
	GetPlayerHealth(playerid, health);

	if(weaponid == WEAPON_COLLISION)
	{
		PlayerHealth[playerid] = health - amount;
	}
	else 
	{
		if(armor < amount)
		{
			SetPlayerArmour(playerid, 0.0);
			PlayerArmor[playerid] = 0;

			new Float:hamount = amount - armor;
			PlayerHealth[playerid] = health - hamount;
		}
		else SetPlayerArmour(playerid, armor - amount);
	}
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
	
	if (weaponid != 38)
	{
		if (WeaponShot [playerid] >= 1)
		{
			KillTimer (FloodTimer [playerid]);
			FloodTimer [playerid] = SetTimerEx ("ResetFloodCheck", 40, false, "i", playerid);

			WeaponShot [playerid]++;
		}
		else if (!WeaponShot [playerid])
		{
			WeaponShot [playerid] = 1;
			FloodTimer [playerid] = SetTimerEx ("ResetFloodCheck", 40, false, "i", playerid);
		}

		if (WeaponShot [playerid] >= FLOOD_LIMIT)
		{
			OnAnticheatDetect (playerid, AC_CHEAT_FLOOD_1);
		}
	}
	return 1;
}

forward ResetFloodCheck (playerid);
public ResetFloodCheck (playerid)
{
	WeaponShot[playerid] = 0;
	return 1;
}


forward AC_OnPlayerDeath(playerid);
public AC_OnPlayerDeath(playerid)
{
	TogglePlayerAnticheat(playerid, false);
	return 1;
}

forward AC_OnPlayerSpawn(playerid);
public AC_OnPlayerSpawn(playerid)
{
	SetPlayerHealthEx(playerid, 100.0);
	SetPlayerArmorEx(playerid, 0.0);
	ResetPlayerWeapons(playerid);

	IsDetected[playerid] = 0;
	return 1;
}
/*
	Functions needed for each Anti-cheat.
*/

// General
stock TogglePlayerAnticheat(playerid, bool:toggle)
{
	if(toggle) IsDisableAnticheat[playerid] = 0;
	else IsDisableAnticheat[playerid] = 1;
	IsDetected[playerid] = 0;
	return 1;
}

// Money
stock GivePlayerMoneyEx (playerid, money) 
{
	PlayerMoney[playerid] += money;
	return 1;
}
stock SetPlayerMoney(playerid, money)
{
	PlayerMoney[playerid] = money;
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
	return 1;
}
stock ResetPlayerWeaponsEx(playerid)
{
	for(new slot = 0; slot < 12; slot++)
	{
		PlayerWeapons[playerid][slot][0] = 0;
		PlayerWeapons[playerid][slot][1] = 0;
	}
	return 1;
}
RemovePlayerWeapon(playerid, weapon)
{
	new weapons[13][2];
	for (new i = 0; i <= 12; i++)
	{
	    GetPlayerWeaponData(playerid, i, weapons[i][0], weapons[i][1]);
	}
	ResetPlayerWeapons(playerid);
	for(new i = 0; i <= 12; i++)
	{
		if(weapons[i][0] != weapon) GivePlayerWeapon(playerid, weapons[i][0], weapons[i][1]);
	}
	return 1;
}
stock RemovePlayerWeaponEx(playerid, weapon)
{
	new slot = GetWeaponSlot(weapon);
	PlayerWeapons[playerid][slot][0] = 0;
	PlayerWeapons[playerid][slot][1] = 0;
	return 1;
}
// Health/Armor
stock SetPlayerHealthEx(playerid, Float:health)
{
	PlayerHealth[playerid] = health;
	return 1;
}
stock SetPlayerArmorEx(playerid, Float:armor)
{
	PlayerArmor[playerid] = armor;
	return 1;
}

// Position
stock SetPlayerPosEx(playerid, Float:X, Float:Y, Float:Z)
{
	PlayerPosition[playerid][0] = X;
	PlayerPosition[playerid][1] = Y;
	PlayerPosition[playerid][2] = Z;
	SetPlayerPos(playerid, X, Y, Z);
	return 1;
}