/*
	Make functions that sets the values both server side and client side.

	You're able to create hooks for any event listener. E.g: AC_OnPlayerConnect.

	Please define the ID and string equivilent of the cheat. E.g
	#define AC_CHEAT_HEALTH			0

	and the string inside the array, the index of the specified cheat must be the same ID as the defined preprocessor ID.
*/

new AC_timer;
new AC_names[][] =
{
	// Add the string for the cheat here.
} 

// Create your variables here.


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
	// Checking happens here.
	// Call the OnAnticheatDetect() with the defined preprocessor ID for the cheat.
	return 1;
}

#if !defined OnAnticheatDetect
	forward OnAnticheatDetect(playerid, cheatid);
	public OnAnticheatDetect(playerid, cheatid)
	{
		return 1;
	}
#endif


// Functions here...