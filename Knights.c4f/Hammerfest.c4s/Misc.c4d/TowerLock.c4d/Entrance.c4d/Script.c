/*-- Niedergang --*/

#strict

local Blocked; // Treppenhausfunktionalit�t blockieren

/* Initialisierung */

protected func Initialize()
{
  // Eingang ist immer frei
  SetEntrance(1);
  // Treppenhausfunktionalit�t aktiv
  Blocked = 0;
  // Gleich pr�fen, ob es Sinn macht
  CheckBack();
  // F�r Neuaufbau: Schloss aktualisieren
  UpdateAllCastleParts();
  return(1);
}

/* Bewegung im Treppenhaus */

public func ContainedDown()
{
  [$TxtDown$]
  // Darunterliegendes Treppenhaus suchen
  var pStaircase;
  if (!(pStaircase = FindStaircase(0, 13 / 2 + 10))) return(0);
  // Inhalt ins Zielobjekt und verlassen
  var pObj;
  while (pObj = Contents())
  {
  	pObj->Enter(pStaircase);
  	AddCommand(pObj, "Exit");
  }
  return(1);
}

public func ContainedLeft()
{
  [$TxtExit$]
  // Inhalt raus
  for (var i = 0; Contents(i); i++)
  	AddCommand(Contents(i), "Exit");
  return(1);
}

public func ContainedRight()
{
  [$TxtExit$]
  // Inhalt raus
  for (var i = 0; Contents(i); i++)
  	AddCommand(Contents(i), "Exit");
  return(1);
}

private func FindStaircase(int iOffsetX, int iOffsetY)
{
  // Anderes Treppenhaus in entsprechender Richtung suchen
  var pStaircase;
  while (pStaircase = FindObject(0, iOffsetX,iOffsetY,0,0, 0,0,0, NoContainer(), pStaircase))
    if (pStaircase->GetOCF() & OCF_Fullcon())
    	if (pStaircase->~IsStaircase())
        return(pStaircase);
  // Keins gefunden
  return(0);
}

/* Status */

public func IsStaircase() { return(!Blocked); }

/* KEIN TimerCall */

private func CheckBack(bool fImmediateRemoval)
{
  // Nicht, wenn es noch f�llt
  if (!GetContact(0, -1, 15)) // CNAT_All
    // Aber sp�ter nochmal checken (fertig runtergefallen)
    return(ScheduleCall(this(), "CheckBack", 37));
  // Anderes Bauteil im Hintergrund
  if (FindCastlePart(0, -10) || !FindStaircase(0, 16))
    if (fImmediateRemoval)
      return(RemoveObject());
    else
      // Ansonsten: N�chstes Frame nochmal checken und entfernen
      // Verz�gert f�r h�ngende SolidMasks?
      ScheduleCall(this(), "CheckBack", 1, 1, true);
}

public func CastleChange() { return(CheckBack()); }

// Bei Aufschlag pr�fen, ob es �berhaupt noch Sinn macht!
protected func Hit()
  {
  // Alles aktualisieren f�r TransferZone-�nderungen...
  // Eigenes CheckBack steckt gleich mit drin
  UpdateAllCastleParts();
  }


private func FindCastlePart(int iOffsetX, int iOffsetY)
{
  // Anderes Bauteil in entsprechender Richtung suchen
  var pPart; 
  while (pPart = FindObject(0, iOffsetX,iOffsetY,0,0, OCF_Fullcon(),0,0, NoContainer(), pPart))
  	if (pPart->~CastlePartWidth())
      return(pPart);
  // Keins gefunden
  return(0);
}

/* Objekt wird entfernt */

protected func Destruction()
{
  // Inhalt auswerfen, damit man nicht einfach umgebracht wird...
  var pObj;
  while (pObj = Contents())	
  	pObj->Exit();
  // Treppenhausfunktionalit�t blockieren...
  Blocked = 1; // (k�nnte statdessen auch zeitverz�gertes CastleChange aufrufen)
  // ...und darunterliegendes Treppenhaus aktualisieren
  var pStaircase;
  if (pStaircase = FindStaircase(0, 13 / 2 + 10))
  	pStaircase->~CastleChange();
  return(1);
}
