/* K�nigsmord - K�nigreichauswahl */

#strict

local pRule; // Regel, die das Men� steuert
// Locals: Zuordnung Men�index -> K�nigsindex

/* Initialisierung */

public func Initialize()
  {
  // K�nigsmord-Regel suchen
  pRule = FindObject(_KTK);
  if (!pRule) return(RemoveObject());
  // K�nigreich direkt nach Team bestimmen
  var iKingdom = iKingdom = pRule->_KTK::GetIndexedKingdom(GetPlayerTeam(GetOwner()) - 1);
  var szKDName, idKD, szKDDesc;
  if (!pRule->_KTK::GetKingdomData(iKingdom, szKDName, idKD, szKDDesc)) return(RemoveObject());
  MnuChooseKingdom(idKD, iKingdom);
    
  /*// Alle Clonks des Besitzers einfangen
  var iCrew, pCrew;
  while (pCrew = GetCrew(GetOwner(), iCrew++)) Enter(this(), pCrew);
  // Men�(s) �ffnen
  Timer();*/
  // Fertig
  return(1);
  }


/* Timer */

/*protected func Timer()
  {
  // Regel noch da?
  if (!pRule) return(RemoveObject());
  // Men� f�r alle Clonks von der Regel erstellen lassen (k�nnte z.B. durch LAufzeitbeitritt geschlossen worden sein
  var i, pClonk;
  while (pClonk = Contents(i++))
    if (!GetMenu(pClonk))
      OpenMenu(pClonk);
  }*/


/* Men�steuerung */

/*
private func OpenMenu(pClonk)
  {
  // Keine Clonkangabe: Aufruf f�r alle enthaltenen Clonks
  if (!pClonk)
    {
    var j, pClonk2;
    //while (pClonk2 = Contents(j++)) if (!OpenMenu(pClonk2)) return();
    return(1);
    }
  // Vorheriges Men� schlie�en
  if (GetMenu(pClonk)) CloseMenu(pClonk);
  // Men� �ffnen
  CreateMenu(_KTK, pClonk, this(), 0, "$MnuKingdomChoice$", 0, 1);
  // Eintr�ge hinzuf�gen
  var i, k, pKing, iKingdom;
  while (iKingdom = pRule->_KTK::GetIndexedKingdom(i++))
    {
    var szKDName, idKD, szKDDesc;
    if (!pRule->_KTK::GetKingdomData(iKingdom, szKDName, idKD, szKDDesc)) continue;
    AddMenuItem(szKDName, "MnuChooseKingdom", idKD, pClonk, 0, iKingdom, szKDDesc);
    Local(k++) = iKingdom;
    }
  // Kein K�nigreich zur Auswahl? Objekt mit allen Clonks l�schen (Spieler eliminiert)
  if (!k)
    {
    Log("$MsgNoKingdoms$");
    EliminatePlayer(GetOwner());
    RemoveObject();
    return();
    }
  // Men�auswahl
  OnMenuSelection(GetMenuSelection(pClonk), pClonk);
  // OK, Erfolg
  return(1);
  }*/

/*protected func MenuQueryCancel() { return(); } // Kein Men�abbruch */

/*protected func OnMenuSelection(iMenuIndex, pClonk)
  {
  // Regel noch da?
  if (!pRule) return(RemoveObject());
  // Position des ausgew�hlten K�nigreiches ermitteln (Bei Fehler (K�nigreich tot) Men�s neu erstellen)
  var kx,ky;
  if (!pRule->_KTK::GetKingdomPos(Local(iMenuIndex), kx, ky)) return(OpenMenu());
  // Sichtposition danach setzen
  SetPosition(kx, ky); SetPlrView(GetOwner(), pClonk);
  // Fertig
  return(1);
  }*/

protected func MnuChooseKingdom(idKD, iKingdom)
  {
  // Sicherheit
  if (!pRule) return(RemoveObject());
  // T�r�� - K�nigreich ausgew�hlt!
  // Alle Men�s schlie�en
  /*var i, pClonk;
  while (pClonk = Contents(i++)) CloseMenu(pClonk);*/
  // Benachrichtigung an den K�nigsmord
  if (!pRule->_KTK::ChooseKingdom(GetOwner(), iKingdom)) return();
  // Erfolg: Wir sind �berfl�ssig :(
  return(RemoveObject());
  }


/* Steuerung */

/*protected func ContainedThrow(pClonk) { [Burgauswahl] return(OpenMenu(pClonk)); } // Men� ablehnen
protected func RejectCollect() { return(1); } // Gegenst�nde ablehnen */
