/*-- Eingangssteuerung --*/

#strict

/* Dieses Objekt stellt die Grundfunktionalit�t f�r Geb�ude
   mit T�r zur Verf�gung. Die Aktionen OpenDoor, DoorOpen
   und CloseDoor m�ssen selbstverst�ndlich in der ActMap der
   jeweiligen Geb�ude vorhanden sein. */

protected func ActivateEntrance(pObj)
{
  if (ActIdle()) SetAction("OpenDoor");
  return(1);
}
  
private func OpenEntrance()
{
  SetEntrance(1);
}

private func CloseEntrance()
{
  SetEntrance(0);
}

private func SoundOpenDoor()
{
  Sound("DoorOpen");
}

private func SoundCloseDoor()
{
  Sound("DoorClose");
}
  
private func DoorClosed()
{
  return(1);
}

protected func Completion()
{
  SetEntrance(0);
  return(_inherited());
}

func Ejection(obj) {
  if (GetAction () eq "DoorOpen") SetAction ("DoorOpen");
  return( _inherited (obj) );
}

func Collection2(obj) {
  if (GetAction () eq "DoorOpen") SetAction ("DoorOpen");
 return( _inherited (obj) );
}
