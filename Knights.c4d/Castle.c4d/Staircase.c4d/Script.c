/*-- Treppenhaus --*/

#strict

#include CPW2 // Burgteilfunktion

protected func Initialize()
{
  // T�r ist immer offen
  SetEntrance(1);
  // Transferzone
  UpdateTransferZone();
	// Basisimplementation
  _inherited();
  // Niedergang erzeugen
  CreateObject(CPTE, 0, -24, GetOwner());
}

/* Bei Ver�nderung der Burg */

public func CastleChange()
{
  UpdateTransferZone();
}

/* Bewegung im Treppenhaus */

public func ContainedUp()
{
  [$TxtUp$]
  var pCase = FindStaircaseAbove();
  if (!pCase) return(0);  
  TransferContents(pCase);
  return(1);
}

public func ContainedDown()
{
  [$TxtDown$]
  var pCase = FindStaircaseBelow();
  if (!pCase) return(0);  
  TransferContents(pCase);
  return(1);
}

public func ContainedLeft()
{
  [$TxtExit$]
  // Inhalt raus
  TransferContents(this());
  return(1);
}

public func ContainedRight()
{
  [$TxtExit$]
  // Inhalt raus
  TransferContents(this());
  return(1);
}

private func TransferContents(object pTarget)
{
  // Von oben runterz�hlen, damit Sachen wie TransferContents(this()) funktionieren.
  var i;
  for (i = ContentsCount() - 1; i >= 0; --i)
  {
    var obj = Contents(i);
    if (!obj) continue;
    Enter(pTarget, obj);
    AddCommand(obj, "Exit");
  }
}

/* Treppenhaus */

private func FindStaircase(int ox, int oy)
{
  // Anderes Treppenhaus in entsprechender Richtung suchen
  var obj;
  while(obj = FindObject(0, ox,oy, 0,0, OCF_Fullcon(), 0,0, NoContainer(), obj) )
    if (obj->~IsStaircase())
      return(obj);
  // Keins gefunden
  return(0);
}

private func FindStaircaseAbove()
{
  return(FindStaircase(0, -CastlePartHeight()/2 - 10) );
}

private func FindStaircaseBelow()
{
  return(FindStaircase(0, CastlePartHeight()/2 + 10) );
}

/* Status */

public func IsStaircase() { return(1); }
public func StaircaseExit() { return(-25); } // Oberer Ausgang

/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (40); }
public func CastlePartHeight() { return (74); }
public func CastlePartTop() { return (-13); }
public func CastlePartBasement() { return(BSC1); }

/* Transfer */

public func UpdateTransferZone()
{
  // Nur mit dar�berliegendem Niedergang
  if (FindStaircaseAbove() )
    if (GetOCF() & OCF_Fullcon() )
      return(SetTransferZone(-8, -40, 16, 61));
  // Ohne dar�berliegenden Niedergang (kein Auf- oder Abstieg m�glich)
  return(SetTransferZone());
}

public func ControlTransfer(object pObj, int tx, int ty)
{
  // Aufstieg
  if (Inside(tx - GetX(), -20, 20) && Inside(ty - GetY(), -50, -20) )
    if (Contained(pObj) == this() || GetY(pObj) > GetY() - 20)
      return(HandleTransferAscend(pObj, tx, ty) );

  // Abstieg
  if (Inside(tx - GetX(), -20, 20) && Inside(ty - GetY(), -20, 37) )
    if (Contained(pObj) == this() || GetY(pObj) < GetY() - 20)
      return(HandleTransferDescend(pObj, tx, ty) );

  // Transfer nicht m�glich
  return(0);
}

private func HandleTransferAscend(object pObj, int tx, int ty)
{
  // Treppenhaus betreten
  if (Contained(pObj) != this())
    return(AddCommand(pObj, "Enter", this()));

  // Aufstieg
  return(ContainedUp(pObj));
}
  
private func HandleTransferDescend(object pObj, int tx, int ty)
{
  // Dar�berliegendes Treppenhaus suchen
  var pCase = FindStaircaseAbove();
  if (!pCase)
    return(HandleTransferDescendPlatform(pObj, tx, ty));
    
  if (Contained(pObj) != pCase)
    return(AddCommand(pObj, "Enter", pCase));
  
  return(pCase->ContainedDown(pObj));
} 

/* Direkter Abstieg von der Platform
 Ohne Treppenhaus l�uft da eigentlich gar nix
 W�re ja noch sch�ner, wenn Mausschummler einfach die Sprengpfeile aus CoFuT holen k�nnten...
 Damit ist der Pathfinder allerdings am Ende. Meist fehlt das Treppenhaus aber eh nur da,
 wo man nicht rein sollte */

private func HandleTransferDescendPlatform(object pObj, int tx, int ty)
{
  return(0);
}

/* Forschung */

public func GetResearchBase() { return(CPW2); }
