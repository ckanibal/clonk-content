/*-- Verbrannter Holz-Wachturm --*/

#strict

local exit;
local battlements;

/* Zerst�rung */

protected func Incineration()
{
  if (exit) RemoveObject(exit);
}

protected func IncinerationEx() { Incineration(); }
protected func Destruction() { Incineration(); }
