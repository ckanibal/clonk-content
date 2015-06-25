/*-- Truhe --*/

#strict

local fLocked;

// �ffnen und Schlie�en

protected func Initialize()
{
  SetAction("Closed");
  return(1);
}

protected func Open()
{
  if (GetAction() ne "Closed") return();
  // Truhe ist abgeschlossen
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Click");
    return();
  }
  SetAction("Opening");
}
  
protected func Close()
{
  if (GetAction() ne "Open") return();  
  SetAction("Closing");	    
}

protected func Grabbed(object pClonk, bool fGrab)
{
  // Die Truhe wurde gerade angefasst oder losgelassen
  CheckGrab();
}

protected func GrabLost(object pClonk)
{
  // Ein anfassender Clonk hat seinen Halt verloren
  CheckGrab();
}

// Status �berpr�fen, wenn die Truhe...
// * angefasst oder losgelassen wird
// * sich fertig ge�ffnet oder geschlossen hat
// * auf- oder zugeschlossen wurde
// * ein anfassender Clonk seinen Halt verloren hat
// * bekanntes Problem: wird ein Clonk durch Schaden weggeschleudert, bekommt die Truhe das nicht mit (es gibt kein GrabLost)

private func CheckGrab()
{
  // Die Truhe soll sich selbst �ffnen, wenn (mindestens) ein Clonk sie anfasst...
  if (FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",this()))
    Open();
  // ...und schlie�en, wenn nicht
  else
    Close();
}

// Auf- und zuschlie�en

public func Lock()
{
  // Bereits abgeschlossen
  if (fLocked) return(1);
  // Abschlie�en    
  fLocked = true;
  Sound("Click");  
  CheckGrab();
  return(1);
}

public func Unlock()
{
  // Nicht abgeschlossen
  if (!fLocked) return(1);
  // Aufschlie�en
  fLocked = false;
  Sound("Click");  
  CheckGrab();
  return(1);
}

protected func RejectCollect(id idObj, object pObj)
{
  // Abgeschlossen: Hineinlegen nicht m�glich
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Error");
    return(true);
  }
  // Nicht abgeschlossen: Hineinlegen okay
  return(false);
}

protected func RejectContents()
{
  // Abgeschlossen: Herausnehmen nicht m�glich
  if (fLocked)
  {
    Message("$MsgLocked$", this());
    Sound("Error");
    return(true);
  }
  // Nicht abgeschlossen: Herausnehmen okay
  return(false);
}

// Sound-Effekte

protected func Opening() 
{
  Sound("ChestOpen"); 
}
  
protected func Closing() 
{
  Sound("ChestClose");
}

// Steuerung

protected func ControlDig(object pClonk)
{
  // Herausnehmen per Graben: Holen-Men� �ffnen
  pClonk->SetCommand(0, "Get", this(), 0,0, 0, 1);
}
