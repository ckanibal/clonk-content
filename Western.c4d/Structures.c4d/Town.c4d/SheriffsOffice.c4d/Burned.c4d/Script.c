/*-- Verbranntes B�ro --*/

#strict
#include BAS9

local arms, prison;

protected func Incineration()
{
  // Angez�ndet!
  if(!prison) return();
  // wenn Banditen fliehen k�nnen, gleich wieder l�schen
  if(prison->Jailbreak()) AddEffect("Extinguish", this(), 99, 50, this());
}

public func FxExtinguishTimer(target)
{
  Extinguish(target);
  return(-1);
}
