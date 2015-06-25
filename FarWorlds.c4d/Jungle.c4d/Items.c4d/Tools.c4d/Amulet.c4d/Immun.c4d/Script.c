/*-- Immunamulett --*/

#strict

public func IsAmulet() { return(1); }

protected func Initialize() { Entrance(Contained()); }

/* Eintritt */

protected func Entrance(object pContainer) { AddEffect("BanPoison",pContainer,242,0,0,GetID()); }

/* Austritt oder Zerst�rung */

protected func Departure(object pContainer) { RemoveEffect("BanPoison",pContainer); }
protected func Destruction() { RemoveEffect("BanPoison",Contained()); }

/* Giftimmunit�tsamulett */

func FxBanPoisonStart(object pTarget,int iEffectNumber, int iTemp)
{
  // tempor�re Effekte ignorieren
  if(iTemp) return();
  // schon bestehendes Gift neutralisieren
  RemoveEffect("PoisonCurse",pTarget);
}

func FxBanPoisonEffect(string szNewEffectName)
{
  // keine Vergiftungen zulassen
  if(szNewEffectName eq "PoisonCurse") return(-1);
}

func FxBanPoisonDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(iDmgEngy > 0) return(iDmgEngy);
  return(iDmgEngy*3/4);
}