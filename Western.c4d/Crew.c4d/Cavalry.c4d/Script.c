/*-- Kavallerist --*/

#strict
#include COWB

local fGrabbedInfo;
local fCaptain;

// Der Kavallerist benutzt immer die InfoSektion eines Cowboys
protected func Recruitment()
{
  if(!fGrabbedInfo) // Ein Hack f�r Peter
    {
    // Cowboy erstellen
    var cb = CreateObject(COWB, 0, 10, GetOwner());
    MakeCrewMember(cb, GetOwner());
    // InfoSektion klauen
    fGrabbedInfo = true;
    GrabObjectInfo(cb);
    // Den anderen Clonk wieder l�schen
    RemoveObject(cb);
    // Volles Leben
    DoEnergy(100);
    // Neues Portrait
    AdjustPortrait();
    // Workaround f�r HEAD-Bug
    SetPlrViewRange(500);
    }
  // Weiter im Text
  return(_inherited());
}

// Ist ein Kavallerist
public func IsCavalryman() { return(1); }

/* S�belkampf */

protected func ControlDown()
{
  if(IsFighting() )
  {
    StopFighting();
    return(1);
  }
  return(_inherited());
}

private func Fighting(pEnemy)
{
  // Kampf mit Handwaffe
  var pWeapon;
  if (pWeapon = GetFightWeapon())
  {
    // Erstmal Kampfaktion setzen
    if (GetAction() ne pWeapon->~GetFightAction())
      return (SetAction(pWeapon->~GetFightAction(), pEnemy));
    // Je nach Waffengeschwindigkeit Schlag durchf�hren
    if (!Random(pWeapon->~GetStrikeTime()))
      return (SetAction(pWeapon->~GetStrikeAction()));
    // Weiterk�mpfen
    return(1);
  }
  // Keine Waffe: Gegner schlagen
  if (!Random(2))
    SetAction("Punch");
  // Weiterk�mpfen
  return(1);
}

public func GetFightWeapon(pClonk)
{
  // Anderer Clonk
  if (pClonk)
    return (pClonk->~GetFightWeapon());
  // Dieser Clonk
  var pObj;
  for (var i = 0; pObj = Contents(i); i++)
    if (pObj->~IsFightWeapon() && CanUse(GetID(pObj)))
      return(pObj);
  // Keine Waffe
  return(0);
}

public func CanUse(idType)
{
  // S�bel
  if (idType == SABR) return(1);
  // Unbekannt
  return(0);
}

private func Striking()
{
  // Waffe bestimmen
  var pWeapon;
  if (!(pWeapon = GetFightWeapon())) return(0);
  // Sound
  pWeapon->~StrikeSound();
  // Kein Schaden
  if (!Random(2)) return(1);
  // Schaden ausrechnen
  var pEnemy = GetActionTarget();
  var iDamage = pWeapon->~GetStrikeDamage();
  iDamage = iDamage * (100 - pEnemy->~IsShielded()) / 100;
  // Captain macht ein bischen mehr Schaden
  if(IsCaptain()) iDamage += 1;
  // Schlagen!
  Punch(pEnemy, iDamage);
  return(1);
}

// Pr�ft, ob der Clonk gerade kaempft.
private func IsFighting()
{
  if (WildcardMatch(GetAction(),"*Fight") ) return(true);
  if (WildcardMatch(GetAction(),"*Strike") ) return(true);
  if (WildcardMatch(GetAction(),"*Punch*") ) return(true);
  if (WildcardMatch(GetAction(),"*Thrust") ) return(true);
  return(false);
}

// Erst 25 Frames nach dem ControlDown-Kommando 
private func StopFighting() 
{ 
  // ComDir auf NONE setzen, damit der Clonk nach dem Kampfabbruch nicht weiterlaeuft. 
  SetComDir(COMD_None() ); 
  // StopFighting-Effekt anfuegen, der den Kampf nach 25 Frames abbricht, wenn nicht schon vorhanden. 
  if(!GetEffect("IntCVRMStopFighting",this(),0,1) ) AddEffect("IntCVRMStopFighting", this(), 1, 25, this() ); 
}

protected func FxIntCVRMStopFightingStop()
{
  // Kaempfen wir noch?
  if(IsFighting() )
  {
    // Letzten Gegner fuer weitere 25 Frames speichern
    AddEffect("IntCVRMLastEnemy", this(), 1, 25, this(), 0, GetActionTarget() );
    // Kampf abbrechen
    SetAction("Walk");
  }
}

protected func FxIntCVRMLastEnemyStart(pTarget, iEffect, pEnemy)
{
  EffectVar(0, pTarget, iEffect) = pEnemy;
}

protected func RejectFight(object pEnemy)
{
  // Ist ein letzter Gegner gespeichert?
  if(GetEffect("IntCVRMLastEnemy", this(), 0, 1) )
    // Ist der letzte Gegner der, mit dem wir hier kaempfen wuerden?
    if(pEnemy == EffectVar(0, this(), GetEffect("IntCVRMLastEnemy", this()) ) )
      // Ja, also abbrechen
      return(1);
  // Kampf erlauben
  return(0);
}

/* Captain */
public func MakeCaptain()
{
  if(fCaptain) return();
  fCaptain = 1;
  // Grafik �ndern
  SetGraphics("Captain");
  SetPortrait("Captain1", this(), GetID());
  // Besser k�mpfen
  SetPhysical("Fight", Max(GetPhysical("Fight", 0)+25000, 75000), 2);
  return(1);
}

// Captain?
public func IsCaptain() { return(fCaptain); }
