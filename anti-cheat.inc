/*
	Make functions that sets the values both server side and client side.

	You're able to create hooks for any event listener. E.g: AC_OnPlayerConnect.
*/
new AC_timer;

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
	return 1;
}


// Functions here...