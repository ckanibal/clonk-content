/*-- K�derfleisch --*/

#strict 2

local iBait;

protected func Initialize()
{
  SetAction("MeatBait");
//  iBait = 80;
  iBait = 50;
  return(1);
}

protected func Hit() { Sound("WoodHit*"); }

/* K�derqualit�t */  
  
public func BaitPower(int iPower)
{
  // K�der mit variabler Qualit�t
  iBait = iPower;
}

public func IsBait() { return(iBait); }

/* Action-Call */

private func Baiting() {
  // Wurde eingesammelt
  if(Contained())
    return(IdleBait());
  // Clonk-Aktion �berpr�fen
  if(GetAction(GetActionTarget()) != "Fish")
    return(IdleBait());
  // Sollte hier noch die Position des Clonks
  // �berpr�fen, falls dieser verrutscht ist.
}

public func IdleBait() { return(SetAction("MeatBait")); }

/* Einsammelbarkeit */

public func RejectEntrance(pObject)
{
  if(GetAction(this()) == "Bait")
    if(GetActionTarget(0,this()) != pObject)
      return(1);
  return(0);
}