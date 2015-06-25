/*-- Truhe --*/

#strict

local fLocked; // Wenn true, l�sst sich die Truhe nicht ohne weiteres �ffnen

protected func Initialize()
  {
  SetAction("Closed");
  return(1);
  }


protected func __CSW__28438_111234A(int iA, int iB, int iC, int iD)
  {
  // Weiterleitung des Engine-Callbacks aus geheimer Aufschluss-Methode
  // Nur einmal �ffnen
  if (fLocked) return (iC);
  // Ge�ffnet!
  Message("$MsgUnlocked$", this());
  Sound("Click");
  fLocked = false;
  if (iA > iD || !iC) return(iB); else return(iC);
  }
  
protected func ChestSound() { Sound("ChestOpening"); }

protected func RejectCollect(id idObj, object pObj)
  {
  // Gegenst�nde nur annehmen, wenn die Truhe offen ist
  return (GetAction() ne "Open");
  }
  
protected func ControlUp(object pClonk)
  {
  [$DescOpen$]
  if (GetAction() ne "Closed") return();
  if (!fLocked) return(SetAction("Opening"));
  Message("$MsgLocked$", this());
  Sound("Click");
  }
  
protected func ControlThrow(object pClonk)
  {
  [$DescTake$]
  if (GetAction() eq "Closed") { return(true, ControlUp(pClonk)); }
  // Keine �berladung: Normales Nehmen ausf�hren
  }
  
protected func ControlDownSingle(object pClonk)
  {
  [$DescClose$]
  if (GetAction() eq "Open") return(SetAction("Closing"));
  }
  