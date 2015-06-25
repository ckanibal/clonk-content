/*-- Capturelimit --*/

#strict

local iCapture;
local fNoMoreSettings;
local pMenuClonk;

protected func Initialize()
{
  // Standart Einstellungen hohlen
  iCapture = capturelimit;
}

protected func Activate(iByPlayer)
{
  // Nicht der Host?
  if(iByPlayer!=GetPlayerByIndex())
  {
    PlayerMessage(iByPlayer, "$MsgOnlyHost$", 0, GetPlayerName(GetPlayerByIndex()));          
    return(1);
  }
  // Nur ein Clonk des Hosts soll hier �ndern k�nnen
  if(pMenuClonk && pMenuClonk!=GetCursor(iByPlayer))
  {
    CloseMenu(pMenuClonk);
  }
  // Schon Geb�ude zerst�rt oder Gruppen gestorben?
  if(iCapture == capturelimit)//!fNoMoreSettings)
  {
    iCapture = capturelimit;
    return(Menu(GetCursor(iByPlayer)));
  }   
  // Dann nichts machen
  else return(PlayerMessage(iByPlayer, "$MsgTooLate$"));
}

public func Menu(pClonk, iSelection)
{
  pMenuClonk = pClonk;
  // Men� mit den Eintr�gen f�llen
  CreateMenu (GetID(), pClonk, this(), 0, "", 0, 0, 0);
  // Frags erh�hen oder erniedrigen
  AddMenuItem (Format("$MsgFrags$", iCapture), "AddFrag", CAPT, pClonk, 0, pClonk, "$DescAddFrag$",2, 1);
  AddMenuItem (Format("$MsgFrags$", iCapture), "RemFrag", CAPT, pClonk, 0, pClonk, "$DescRemFrag$",2,2);
  // Best�tigen und Abbrechen
  AddMenuItem ("$MsgOK$", "OK", CAPT, pClonk, 0, pClonk, "$DescOk$",2,3); // CXCN
  SelectMenuItem(iSelection, pClonk);
  return(1);
}

public func AddFrag(idType, pClonk)
{
  // Frags erh�hen
  iCapture = Min(100, iCapture+1);
  return(Menu(pClonk, 0));
}

public func RemFrag(idType, pClonk)
{
  // Frags erniedrigen
  iCapture = Max(1, iCapture-1);    
  return(Menu(pClonk, 1));
}

public func OK()
{
  // Nachricht und Variablen umstellen  
  Message("$MsgHostChangedSettings$", 0, iCapture);
  capturelimit = iCapture; 
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("$MsgFrags$", capturelimit), SBRD_Caption);
}

public func Cancel()
{
  // Abbruch: alles wieder auf Anfangswerte stellen	
  iCapture = capturelimit; 
  
  pMenuClonk = 0;
}
