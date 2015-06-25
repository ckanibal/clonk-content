/*-- Fraglimit --*/

#strict

local iFrags;
local fNoMoreSettings;
local pMenuClonk;

protected func Initialize()
{
  // Standart Einstellungen hohlen
  iFrags = FragLimit;
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
  }
  // Schon Geb�ude zerst�rt oder Gruppen gestorben?
  if(!fNoMoreSettings)
  {
    iFrags = FragLimit;
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
  AddMenuItem (Format("$MsgFrags$", iFrags), "AddFrag", FRAG, pClonk, 0, pClonk, "$DescAddFrag$",2, 1);
  AddMenuItem (Format("$MsgFrags$", iFrags), "RemFrag", FRAG, pClonk, 0, pClonk, "$DescRemFrag$",2,2);
  // Hauptmann
  if(fDontUseChief)
  {
    var pIcon = CreateObject(CHIF);
    SetClrModulation(RGB(255,80,80), pIcon);
    AddMenuItem ("$MsgNoChief$", "SwitchChief", CHIF, pClonk, 0, pClonk, 0,4,pIcon);
    RemoveObject(pIcon);
  }
  else 
    AddMenuItem ("$MsgChief$", "SwitchChief", CHIF, pClonk, 0, pClonk, 0);
  // Fertig
  AddMenuItem ("$MsgOK$", "OK", FRAG, pClonk, 0, pClonk, "$DescOk$",2,3); // CXCN
  // 
  SelectMenuItem(iSelection, pClonk);
  return(1);
}

public func AddFrag(idType, pClonk)
{
  // Frags erh�hen
  iFrags = Min(100, iFrags+1);
  return(Menu(pClonk, 0));
}

public func RemFrag(idType, pClonk)
{
  // Frags erniedrigen
  iFrags = Max(1, iFrags-1);  	
  return(Menu(pClonk, 1));
}

public func OK()
{
  // Nachricht und Variablen umstellen	
  if(!fDontUseChief) Message("$MsgHostChangedSettings$|$MsgChief$.", 0, iFrags);
  else Message("$MsgHostChangedSettings$|$MsgNoChief$.", 0, iFrags);
  FragLimit = iFrags; 
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("$MsgFrags$", FragLimit),                    SBRD_Caption);
}

public func SwitchChief(idType, pClonk)
{
  // Frags erniedrigen
  fDontUseChief = !fDontUseChief;  	
  return(Menu(pClonk, 2));
}
