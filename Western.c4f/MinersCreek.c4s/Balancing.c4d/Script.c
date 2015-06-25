/*-- Balancing --*/

#strict

local iRelaunchesCVRM, iRelaunchesBNDT, iBuildings, iUtilities;
local pMenuClonk;

protected func Initialize()
{
  // Standart Einstellungen hohlen
  iRelaunchesBNDT = relright; 
  iRelaunchesCVRM = relleft;
  iBuildings = maxright;
  iUtilities = maxutil;
}

protected func Activate(iByPlayer)
{
  // Nicht der Host?
  if(iByPlayer!=GetPlayerByIndex())
  {
    PlayerMessage(iByPlayer, "$MsgOnlyHost$", 0, GetPlayerName(GetPlayerByIndex()));    	  
    return(1);
  }
  // Nur ein Clonk des Hosts soll hier rumpfuschen
  if(pMenuClonk && pMenuClonk!=GetCursor(iByPlayer))
  {
    CloseMenu(pMenuClonk);
    Cancel();
  }
  // Schon Geb�ude zerst�rt oder Gruppen gestorben?
  if(iRelaunchesBNDT==relright && iRelaunchesCVRM==relleft && !destleft && !destright)
  {
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
  // Relaunches
  AddMenuItem ("$MsgRelaunchesCVRM$", "AddRelaunches1", BALA, pClonk, iRelaunchesCVRM, pClonk, "$DescRelaunchesCVRM$",2, 1);
  AddMenuItem ("$MsgRelaunchesBNDT$", "AddRelaunches2", BALA, pClonk, iRelaunchesBNDT, pClonk, "$DescRelaunchesBNDT$",2,2);
  // Ziele
  AddMenuItem ("$MsgBuildings$", "AddBuildings", BALA, pClonk, iBuildings, pClonk, "$DescBuildings$",2,3);
  AddMenuItem ("$MsgUtilities$", "AddUtilities", BALA, pClonk, iUtilities, pClonk, "$DescUtilities$",2,4);
  // Leerstelle, damit Best�tigen und Abbrechen sch�n abgegrenzt in die n�chste Zeile kommen
  AddMenuItem ("", "", BALA, pClonk, 0, pClonk, " ",2,7);
  // Best�tigen und Abbrechen
  AddMenuItem ("$MsgOK$", "OK", BALA, pClonk, 0, pClonk, "$DescOk$",2,5); // CXCN
  AddMenuItem ("$MsgCancel$", "Cancel", BALA, pClonk, 0, pClonk, "$DescCancel$",2,6); // MCMX
  SelectMenuItem(iSelection, pClonk);
  return(1);
}

public func OnMenuSelection(int iItemIndex, object pMenuObject)
{
  // F�r nicht Net2, �berspringen der Leerstelle
  if(iItemIndex==4) SelectMenuItem(5, pMenuObject);	
}

public func AddRelaunches1(idType, pClonk)
{
  // Kavallerie erh�hen
  iRelaunchesCVRM = Min(10, iRelaunchesCVRM+1);  	
  return(Menu(pClonk, 0));
}

public func AddRelaunches2(idType, pClonk)
{
  // Banditen erh�hen
  iRelaunchesBNDT = Min(10, iRelaunchesBNDT+1);  	
  return(Menu(pClonk, 1));
}

public func AddBuildings(idType, pClonk)
{
  // Mehr Geb�ude
  iBuildings = Min(7, iBuildings+1);  	
  return(Menu(pClonk, 2));
}

public func AddUtilities(idType, pClonk)
{
  // Mehr Versorgungseinrichtungen
  iUtilities = Min(3, iUtilities+1);  	
  return(Menu(pClonk, 3));
}

public func MenuQueryCancel(int MenuItem, object pClonk)
{
  // Passende Zahlen verringern
  if(MenuItem==0)
  {
    iRelaunchesCVRM = Max(1, iRelaunchesCVRM-1);	  
    return(Menu(pClonk, MenuItem));
  }
  else if(MenuItem==1)
  {
    iRelaunchesBNDT = Max(1, iRelaunchesBNDT-1);	  
    return(Menu(pClonk, MenuItem));
  }
  if(MenuItem==2)
  {
    iBuildings = Max(2, iBuildings-1);	  
    return(Menu(pClonk, MenuItem));
  }
  else if(MenuItem==3)
  {
    iUtilities = Max(0, iUtilities-1);	  
    return(Menu(pClonk, MenuItem));
  }  
}

public func Cancel()
{
  // Abbruch: alles wieder auf Anfangswerte stellen	
  iRelaunchesBNDT = relright; 
  iRelaunchesCVRM = relleft;
  iBuildings = maxright;
  iUtilities = maxutil;	
  
  pMenuClonk = 0;
}

public func OK()
{
  // Nachricht und Variablen umstellen	
  Message("$MsgHostChangedSettings$");
  relright = iRelaunchesBNDT; 
  relleft = iRelaunchesCVRM;
  maxright = iBuildings;
  // Scorebord anpassen
  if(iUtilities && !maxutil)
  {
    SetScoreboardData(SBRD_Caption, ScoreboardCol(UTIL), "{{PUMW}}", ScoreboardCol(UTIL));
  }
  else if(!iUtilities && maxutil)
  {
    SetScoreboardData(ScoreboardCol(BNDT), ScoreboardCol(UTIL)); 
  }
  maxutil = iUtilities;
  // Scorebord aktualisieren und anzeigen
  ActualizeScoreboard();
}
