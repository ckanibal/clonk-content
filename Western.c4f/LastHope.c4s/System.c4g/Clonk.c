/*-- Startmen�s nicht abbrechbar --*/

#strict
#appendto CLNK

protected func MenuQueryCancel()
{
 // Die Relaunchmessage verschwindet nach dem Schlie�en des Relaunchmen�s
 if (GetMenu() == WIPF) return(1);
}