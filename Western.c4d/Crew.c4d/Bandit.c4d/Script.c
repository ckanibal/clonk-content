/*-- Bandit --*/

#strict
#include COWB

local fGrabbedInfo;
local fBoss;

// Der Bandit benutzt immer die InfoSektion eines Cowboys
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

/* Boss */
public func MakeBoss()
{
  if(fBoss) return();
  fBoss = 1;
  // Grafik �ndern
  SetGraphics("Boss");
  SetPortrait("BanditBoss", this(), GetID());
  // Besser k�mpfen
  SetPhysical("Fight", Max(GetPhysical("Fight", 0)+25000, 75000), 2);
  return(1);
}

/* Begrenzung der Steuerung w�rend er verhaftet ist */
local fIsCaught;

// Weder Graben noch Werfen
func ControlThrow()
{
  if(fIsCaught) return(1);
  return(_inherited());
}

func ControlDig()
{
  if(fIsCaught) return(1);
  return(_inherited());
}

func ControlDigSingle()
{
  if(fIsCaught) return(1);
  return(_inherited());
}

// Auf kein Inventarwechsel
func ControlSpecial()
{
  [$CtrlInventoryDesc$|Image=CXIV]
  if(fIsCaught) return(1);
  return(_inherited());
}

// Gefesselte schlagen nicht zu
private func Fighting()
{
  if (!Random(2) && !fIsCaught) SetAction("Punch");
  return(1);
}

// Clonk darf sein gefesseltes Men� nicht beenden
func MenuQueryCancel(iSelection, pMenuObj)
{
  if(GetMenu() == SHSI) return(1);
  return(_inherited(iSelection, pMenuObj));
}

// Nichts aufnehmen
public func RejectCollect(a,b)
{
  if(fIsCaught) return(1);	
  return(_inherited(a,b));
}

// Captain? (hat die gleiche Funktion wie der Kavalleriehauptmann)
public func IsCaptain() { return(fBoss); }

// Ist ein Bandit
public func IsBandit() { return(1); }
