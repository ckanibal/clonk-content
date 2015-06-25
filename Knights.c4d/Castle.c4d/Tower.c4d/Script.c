/*-- Turm --*/

#strict

#include CPT1 // Burgteil - Treppenhausfunktion

/* Burgteil */

public func CastlePartWidth() { return(0); } // Wand hinter dem Turm gilt als Burgteil
public func StaircaseExit() { return(-31); } // Oberer Ausgang f�rs Treppenhaus
public func CastlePartBasement() { return(0); } // Wand hinter dem Turm erzeugt das Fundament


/* Torsteuerung */


protected func CtxControlLeft(object pClonk)
{
  // Kontextfunktion: Nur Kontrolle erm�glichen, wenn der Clonk auch tats�chlich das Tor anfasst
  // Zur Abw�rtskompatibilit�t mit Szenarienscripten, die Tore �ffnen, wird dies in eine eigene Funktion ausgelagert
  if (GetProcedure(pClonk) eq "PUSH")
    if (GetActionTarget(0, pClonk) == this())
      return (ControlLeft(pClonk));
}

protected func CtxControlRight(object pClonk)
{
  if (GetProcedure(pClonk) eq "PUSH")
    if (GetActionTarget(0, pClonk) == this())
      return (ControlRight(pClonk));
}

protected func FindGateLeft() { return(FindObject(0,0,0,0,0,0,"GateLeft",this())); }
protected func FindGateRight() { return(FindObject(0,0,0,0,0,0,"GateRight",this())); }

protected func ControlLeft()
{
  [$TxtLeftgate$|Image=GATL|Condition=FindGateLeft]
  var pGate = FindGateLeft();
  if (!pGate) return(0);
  pGate->Activate();
  return(1);
}
 
protected func ControlRight()
{
  [$TxtRightgate$|Image=GATR|Condition=FindGateRight]
  var pGate = FindGateRight();
  if (!pGate) return(0);
  pGate->Activate();
}

/* Zugbr�cke */

protected func FindDrawbridge()
{
	var pBridge;
  // Zugbr�cke rechts suchen
  while (pBridge = FindObject(CPBR, +40,0,0,0, 0, 0,0, NoContainer(), pBridge))
  	if (pBridge->IsAttachedTo(this()))
      return(pBridge);
  // Zugbr�cke links suchen
  pBridge = 0;
  while (pBridge = FindObject(CPBL, -40,0,0,0, 0, 0,0, NoContainer(), pBridge))
  	if (pBridge->IsAttachedTo(this()))
      return(pBridge);
  // Keine Zugbr�cke gefunden
  return(0);
}

// Heruntergelassene Zugr�cke suchen 
protected func FindDrawbridgeDown()
{
	var pBridge;
  if (!(pBridge = FindDrawbridge())) return(0);
  return(pBridge->IsDown());
}

// Hochgezogene Zugr�cke suchen 
protected func FindDrawbridgeUp()
{
	var pBridge;
  if (!(pBridge = FindDrawbridge())) return(0);
  return(pBridge->IsUp());
}  

protected func DrawbridgeUp(object pBridge)
{
  Sound("Click");
  pBridge->ControlUp();
  UpdateTransferZone();  
  return(1); 
}

protected func DrawbridgeDown(object pBridge)
{
  Sound("Click");
  pBridge->ControlDown();
  UpdateTransferZone();  
  return(1); 
}
 
/* Kontext */ 
 
public func ContextGateLeft(object pClonk)
{
  [$TxtLeftgate$|Image=GATL|Condition=FindGateLeft]
  SetCommand(pClonk, "Grab", this());
  AppendCommand(pClonk, "Call", this(), 0,0,0,0, "CtxControlLeft");  
  return(1); 
}
 
public func ContextGateRight(object pClonk)
{
  [$TxtRightgate$|Image=GATR|Condition=FindGateRight]
  SetCommand(pClonk, "Grab", this());
  AppendCommand(pClonk, "Call", this(), 0,0,0,0, "CtxControlRight");  
  return(1); 
}

public func ContextBridgeUp(object pClonk)
{
  [$TxtDrawbridgeup$|Image=CPBR|Condition=FindDrawbridgeDown]
  SetCommand(pClonk, "Grab", this());
  AppendCommand(pClonk, "Call", this(), 0,0,0,0, "CtxControlUp");  
  return(1); 
}
 
public func ContextBridgeDown(object pClonk)
{
  [$TxtDrawbridgedown$|Image=CPBR|Condition=FindDrawbridgeUp]
  SetCommand(pClonk, "Grab", this());
  AppendCommand(pClonk, "Call", this(), 0,0,0,0, "CtxControlDownSingle");  
  return(1); 
}
  
/* Wenn der Turm mal runterf�llt */

protected func Hit()
{
  UpdateTransferZone();
}

/* Initialisierung */

protected func Initialize()
{
  // Hintergrund-Wand erzeugen
  var pWall;
  pWall = CreateObject(CPW3, 0, +30, GetOwner());
  // An Hintergrund-Wand ausrichten
  SetPosition(pWall->GetX(), pWall->GetY() + 7);
  // Brustwehren erzeugen
  CreateObject(CPB1, -21, -27, GetOwner())->AttachTo(pWall, DIR_Left());
  CreateObject(CPB1, +22, -27, GetOwner())->AttachTo(pWall, DIR_Right());
  // Niedergang erzeugen
  CreateObject(CPTE, 0, -31, GetOwner());
  // T�r ist immer offen
  SetEntrance(1);
  // Turmspezifische Teile erzeugen
  InitializeTower();
  // Transferzone setzen
  UpdateTransferZone();
  // Objekte im Umkreis ihre �berpr�fungen durchf�hren lassen
  CastlePartInitialize();
  // Fertig
  return(1);
}

private func InitializeTower()
{
  // Tore erzeugen
  CreateObject(CPGL, -14, +10, GetOwner())->SetAction("GateLeft", this());
  CreateObject(CPGR, +14, +10, GetOwner())->SetAction("GateRight", this());
}

/* Transfer */

public func UpdateTransferZone()
{
  // Mit dar�berliegendem Niedergang
  if (FindStaircaseAbove())
    if (GetOCF() & OCF_Fullcon())
      return(SetTransferZone(-19, -47, 38, 61));
  // Ohne dar�berliegenden Niedergang (kein Auf- oder Abstieg m�glich)
  return(SetTransferZone(-19, +10, 38, 4));
}

public func ControlTransfer(object pObj, int iTx, int iTy)
{ 
	//Log("Zugbr�cke - ControlTransfer");
  // Aufstieg
  if (Inside(iTx - GetX(), -20, +20) && Inside(iTy - GetY(), -50, -20))
    if ((pObj->Contained() == this()) || (GetY(pObj) > GetY() - 20))
      return(HandleTransferAscend(pObj, iTx, iTy)); // -> CPT1
  // Abstieg
  if (Inside(iTx - GetX(), -20, +20) && Inside(iTy - GetY(), -20, +37))
    if ((pObj->Contained() == this()) || (GetY(pObj) < GetY() - 20))
      return(HandleTransferDescend(pObj, iTx, iTy)); // -> CPT1
  // Ausgang rechts
  if (Inside(iTx - GetX(), +16, +30) && Inside(iTy - GetY(), -20, +37))
    return(HandleTransferGateRight(pObj, iTx, iTy));
  // Ausgang links
  if (Inside(iTx - GetX(), -30, -16) && Inside(iTy - GetY(), -20, +37))
    return(HandleTransferGateLeft(pObj, iTx, iTy));

  // Zugbr�cke
  if (FindDrawbridge())
  	// Seitlicher Ausgang gew�nscht
  	if (Inside(iTx - GetX(), +16, +30) || Inside(iTx - GetX(), -30, -16))
  		// Der n�chste Wegpunkt liegt unten im Burggraben
  		if (iTy > GetY() + 30)
  			// Zugbr�cke �ffnen
	  		return(HandleTransferDrawbridge(pObj, iTx, iTy));
    
  // Transfer nicht m�glich
	//Log("can't handle");
  return(0);
}
  
private func HandleTransferGateRight(object pObj, int iTx, int iTy)
{
	//Log("HandleTransferGateRight");
  // Tor suchen
  var pGate = FindGateRight();
  if (!pGate) return(HandleTransferDrawbridge(pObj, iTx, iTy));
  // Tor ist offen: Turm loslassen
  if (pGate->GetY() - GetY() < 0)
    if ((pObj->GetAction() S= "Push") && (pObj->GetActionTarget() == this()))
      return(AddCommand(pObj, "UnGrab", this()));      
  // Tor ist offen: erledigt
  if (pGate->GetY() - GetY() < 0) return(0);
  // Clonk soll den Turm anfassen
  if (!((pObj->GetAction() S= "Push") && (pObj->GetActionTarget() == this())))
    return(AddCommand(pObj, "Grab", this()));
  // Tor bewegt sich: warten
  if (pGate->GetComDir() == COMD_Up()) return(1);
  // Tor �ffnen
  return(ControlRight(pObj));
}

private func HandleTransferGateLeft(object pObj, int iTx, int iTy)
{
	//Log("HandleTransferGateLeft");
  // Tor suchen
  var pGate = FindGateLeft();
  if (!pGate) return(HandleTransferDrawbridge(pObj,  iTx,  iTy));
  // Tor ist offen: Turm loslassen
  if (pGate->GetY() - GetY() < 0) 
    if ((pObj->GetAction() S= "Push") && (pObj->GetActionTarget() == this()))
      return(AddCommand(pObj, "UnGrab", this()));      
  // Tor ist offen: erledigt
  if (pGate->GetY() - GetY() < 0) return(0);
  // Clonk soll den Turm anfassen
  if (!((pObj->GetAction() S= "Push") && (pObj->GetActionTarget() == this())))
    return(AddCommand(pObj, "Grab", this()));
  // Tor bewegt sich: warten
  if (pGate->GetComDir() == COMD_Up()) return(1);
  // Tor �ffnen
  return(ControlLeft(pObj));
}

private func HandleTransferDrawbridge(object pObj, int iTx, int iTy)
{
	//Log("HandleTransferDrawbridge");
  // Zugbr�cke suchen
  var pBridge = FindDrawbridge();
  if (!pBridge) return(0);
  // Zugbr�cke ist offen
  if (pBridge->IsOpen())
  {
  	// Turm loslassen
    if ((pObj->GetAction() S= "Push") && (pObj->GetActionTarget() == this()))
      return(AddCommand(pObj, "UnGrab", this()));      
    // Keine weitere Transferaktion n�tig
  	return(0);
  }
  // Clonk soll den Turm anfassen
  if (!((pObj->GetAction() S= "Push") && (pObj->GetActionTarget() == this())))
    return(AddCommand(pObj, "Grab", this()));
  // Zugbr�cke bewegt sich: warten
  if (pBridge->IsOpening()) return(1);
  // Zugbr�cke �ffnen
  return(DrawbridgeDown(pBridge));
}  
