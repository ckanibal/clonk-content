/*-- K�nigsmord --*/

#strict

/* Lokale Variablen */

local iKingCount;           // Anzahl der K�nige
local iKingLocals;          // Anzahl der lokalen Variablen, die f�r K�nige reserviert sind
// Local(0..iKingLocals-1)             // K�nige (Pointer)
// Local(iKingLocals..iKingLocals*3-1) // X- und Y-Startposition der K�nigreiche (XPos immer != 0 f�r K�nigreiche; 0 f�r tote K�nigreiche)
// Local(iKingLocals*3..oo)            // Index des Spielerteams



/* Initialisierung */

protected func Initialize()
  {
  // Immer nach oben links f�r einfachere Offsets
  SetPosition();
  // Das Ziel muss vor den Spielern aktiviert werden
  if (GetPlayerCount()) return (Log("$ErrActivateAfterPlrJoin$"), RemoveObject());
  // K�nige suchen
  var pKing;
  while (pKing = FindObject(KING, 0,0,0,0, 0, 0,0, 0, pKing))
    // Neuer K�nig gefunden - setzen
    King(++iKingCount) = pKing;
  // Ausreichen K�nige da?
  if (iKingCount < 2) return (Log("$ErrNeedsTwoKings$"), RemoveObject());
  // Local-Listen initialisieren
  iKingLocals = iKingCount;
  // Startpositionen setzen
  var iKing=iKingLocals;
  while (iKing)
    {
    StartPosX(iKing) = Max(GetX(pKing= King(iKing)), 1);
    StartPosY(iKing) = GetY(pKing);
    --iKing;
    }
  // Pr�fung starten
  SetAction("Checking");
  // Fertig
  return(1);
  }



/* Spielerbeitritt */

protected func InitializePlayer(iPlr)
  {
  // Spiel schon zu Ende?
  if (!IsGameRunning()) return(EliminatePlayer(iPlr));
  // Jeder Spieler kann sich sein K�nigreich aussuchen
  CreateObject(_KSL, 10,10, iPlr);
  // Das Auswahlobjekt wird alle weiteren Schritte �bernehmen
  return(1);
  }

public func ChooseKingdom(iPlr, iKingdom)
  {
  // Spiel schon zu Ende?
  if (!IsGameRunning()) return(EliminatePlayer(iPlr));
  // Gew�hltes K�nigreich pr�fen
  var pKing = King(iKingdom);
  if (!pKing) return();
  // Spieler pr�fen (Schon in einem Team?)
  if (PlayerTeam(iPlr)) return();
  // Spieler in dieses K�nigreich versetzen
  PlayerTeam(iPlr) = iKingdom;
  // Wenn der K�nig noch nicht zugewiesen ist: K�nigreich �bertragen
  // Beitrittsnachricht
  var szJoinMessage;
  var szKDName, idKD, szKDDesc;
  GetKingdomData(iKingdom, szKDName, idKD, szKDDesc);
  if (GetOwner(pKing)<0)
    {
    AssignKing2Plr(iKingdom, iPlr, -1);
    szJoinMessage = RandomJoinLeadMessage(GetPlayerName(iPlr), szKDName);
    }
  else
    szJoinMessage = RandomJoinMessage(GetPlayerName(iPlr), szKDName);
  PlayerMessage(iPlr, szJoinMessage);
  Log(szJoinMessage);
  // Verfeindung mit allen anderen Spielern anpassen
  for (var i,j=GetPlayerCount(); j>1; ++i)
    if (GetPlayerName(i) && i!=iPlr)
      {
      --j;
      var fHostile = PlayerTeam(i) != iKingdom;
      SetHostility(i, iPlr, fHostile, 1);
      SetHostility(iPlr, i, fHostile, 1);
      }
  // Spieler starten lassen
  LaunchPlayer(iPlr);
  // Fertig
  return(1);
  }

public func LaunchPlayer(iPlr, fRelaunch)
  {
  // Startposition ermitteln
  var iKingdom = PlayerTeam(iPlr);
  if (!iKingdom) return();
  var sx, sy; GetKingdomPos(iKingdom, sx, sy);
  // Vorerst einfach die gesamte Spielercrew plazieren
  var iCrew, pCrew;
  while (pCrew = GetCrew(iPlr, iCrew++))
    {
    DoEnergy(10000, pCrew);
    while (DoMagicEnergy(1, pCrew)) ;
    if (Contained(pCrew)) if (GetID(pCrew) != KING) Exit(pCrew);
    SetPosition(sx + Random(21)-10, sy - Random(20) - 10, pCrew);
    SetDir(Random(2), pCrew);
    }
  // Flagge gibts gratis
  var pFlagClonk = GetCursor(iPlr); if (!pFlagClonk) pFlagClonk = GetHiRank(iPlr);
  if (pFlagClonk) CreateContents(FLAG, pFlagClonk);
  // OK, fertig
  return(1);
  }

private func AssignKing2Plr(iKingdom, iPlr, iFromPlayer)
  {
  // Keine Besitzer�nderung?
  if (iPlr == iFromPlayer) return(1);
  // Kein Besitzer: K�nigreich annehmen
  var obj;
  var iKDX, iKDY, iKDWdt, iKDHgt;
  if (iFromPlayer<0)
    // Nur Objekte im Bereich des K�nigreiches
    if (!GetKingdomExtents(iKingdom, iKDX, iKDY, iKDWdt, iKDHgt)) return();
  // Ansonsten: Alle Objekte mit entsprechendem Besitzer
  while (obj = FindObjectOwner(0, iFromPlayer, iKDX, iKDY, iKDWdt, iKDHgt, 0, 0,0, obj))
    if (~GetOCF(obj) & OCF_Living())
      SetOwner(iPlr, obj);
  // Kein Spieler: K�nig �bernehmen
  var pKing;
  pKing = King(iKingdom);
  if (!pKing) return();
  if (GetOwner(pKing)>=0)
    {
    // K�nigsbesitzer vorhanden: Info l�schen
    var pDelKing = CreateObject(GetID(pKing), 0,0, GetOwner(pKing));
    GrabObjectInfo(pKing, pDelKing);
    RemoveObject(pDelKing);
    }
  // K�nig in die eigene Crew
  SetOwner(iPlr, pKing); MakeCrewMember(pKing, iPlr);
  SetOwner(iPlr, pKing); 
  // K�nig ausw�hlen
  UnselectCrew(iPlr); 
  SetCursor(iPlr, pKing);
  SelectCrew(iPlr, pKing, 1);
  // Fertig
  return(1);
  }



/* Spieleraustritt */

protected func RemovePlayer(iPlr)
  {
  // Spiel schon zu Ende? Keine weiteren Checks n�tig (normalerweise sollte iPlrTeam==0 sein; aber es k�nnten auch in der Endphase noch Spieler aus dem Gewinnerteam austreten!)
  if (!IsGameRunning()) return(1);
  // Spielerteam zugewiesen? (Schl�gt fehl, falls der Spieler in EliminateTeam() eliminiert wird!)
  var iPlrTeam = PlayerTeam(iPlr);
  if (!iPlrTeam) return(1);
  // Team l�schen
  PlayerTeam(iPlr) = 0;
  // Hat das Team noch einen K�nig?
  var pKing = King(iPlrTeam);
  if (!pKing) return(1);
  // Geh�rt der K�nig diesem Spieler?
  if (GetOwner(pKing) == iPlr)
    {
    // Existiert noch ein anderer Spieler in diesem Team?
    var iOtherTeamPlr = FindPlayerOfTeam(iPlrTeam);
    if (iOtherTeamPlr < 0)
      // Anderer Spieler nicht gefunden: Team eliminieren
      EliminateTeam(iPlrTeam);
    else
      // Anderer Spieler gefunden: K�nig(reich) an diesen Spieler �bergeben
      AssignKing2Plr(iPlrTeam, iOtherTeamPlr, iPlr);
    return(1);
    }
  // OK, eine unterst�tzende Kraft f�llt weg
  return(1);
  }

protected func EliminateTeam(iKingdom)
  {
  // K�nigreich schon weg?
  if (!StartPosX(iKingdom)) return();
  // Versager-Nachricht
  var szKDName, idKD, szKDDesc;
  if (!GetKingdomData(iKingdom, szKDName, idKD, szKDDesc)) return();
  Log(RandomEliminationMsg(szKDName));
  // Alle Spieler aus dem K�nigreich vernichten
  for (var i,j=GetPlayerCount(); j; ++i) if (GetPlayerName(i))
    {
    --j;
    if (PlayerTeam(i) == iKingdom)
      {
      PlayerTeam(i) = 0;
      EliminatePlayer(i);
      }
    }
  // Der K�nig k�nnte noch leben, wenn zum Beispiel der Thron erobert wurde
  var pKing = King(iKingdom);
  if (GetAlive(pKing)) Kill(pKing);
  // Ein K�nigreich weniger
  King(iKingdom) = StartPosX(iKingdom) = StartPosY(iKingdom) = 0;
  --iKingCount;
  // Fertig
  return(1);
  }

public func PlayerForfeit(iPlr, iByPlayer)
  {
  // Ein Spieler hat das Leben seines K�nigreiches verwirkt
  // G�ltiger Spieler?
  if (iPlr<0) return();
  // K�nigreich zugewiesen?
  var iKingdom = PlayerTeam(iPlr);
  // Ansonsten haftet der Spieler pers�nlich :)
  if (!iKingdom) return(EliminatePlayer(iPlr));
  // K�nigreich vernichten
  return(EliminateTeam(iKingdom));
  }

public func PlayerEliminated(iPlr)
  {
  // Spiel schon zu Ende?
  if (!IsGameRunning()) return();
  // Geh�rt der Spieler einem K�nigreich an?
  var iKingdom = PlayerTeam(iPlr);
  if (!iKingdom) return();
  // Ist er selber nicht der K�nig?
  var pKing = King(iKingdom);
  if (!pKing) return();
  if (GetOwner(pKing) == iPlr) return();
  // Dann kann ihn der K�nig wieder zum Leben erwecken
  // Thron suchen
  var pThrone = FindObjectOwner(THRN, GetOwner(pKing));
  if (!pThrone) return();
  // Relaunch im Thron erm�glichen
  return(pThrone->PlayerEliminated(iPlr));
  }




/* Spielziel */

protected func Checking()
  {
  // Timer: Pr�fen, ob ein K�nigreich eliminiert ist
  var i=iKingLocals, pKing;
  while (i--)
    if (pKing = Local(i))
      if (!GetAlive(pKing))
        // K�nig tot: Team eliminieren
        EliminateTeam(i+1);
      else; // spart einen Block :)
    else if (Local(iKingLocals + i*2))
      // Kein K�nig mehr da, aber dennoch ein K�nigreich zugewiesen (XPos != 0): Team auch eliminieren
      EliminateTeam(i+1);
  // Ende?
  if (iKingCount <= 1) EndGame();
  }

private func EndGame()
  {
  // Nur, wenn noch laufend
  if (!IsGameRunning()) return();
  // Sieger ermitteln
  var iWinner = FindWinner();
  // Siegernachricht
  Sound("Trumpet", 1);
  var szWinMsg, szKDName, idKD, szKDDesc;
  if (iWinner) if (GetKingdomData(iWinner, szKDName, idKD, szKDDesc)) szWinMsg = RandomWinningMsg(szKDName);
  // Anonyme Nachricht
  if (!szWinMsg) szWinMsg = RandomWinningMsg2();
  // Nachricht ausgeben
  Message("@%s", 0, szWinMsg);
  Log(szWinMsg);
  // Alle Spieler eliminieren, die nicht im Team sind (z.B. noch kein K�nigreich gew�hlt)
  for (var i,j=GetPlayerCount(); j; ++i) if (GetPlayerName(i))
    {
    --j;
    if (PlayerTeam(i) != iWinner)
      {
      PlayerTeam(i) = 0;
      EliminatePlayer(i);
      }
    }
  // Neue Endaktivit�t
  SetAction("Ending");
  // Fertig
  return(1);
  }

private func FindWinner()
  {
  // Einen Gewinner in allen K�nigreichen suchen
  var i = iKingLocals, pKing;
  while (i)
    if (pKing = King(i--))
      // K�nig vorhanden und lebt: Gewinner gefunden!
      if (GetAlive(pKing)) return(i+1);
  // Kein Gewinner
  return();     
  }

protected func EndingDone()
  {
  // Ende Gel�nde
  GameOver();
  return(RemoveObject());
  }



/* K�nigreichverwaltung */

private func & King(iIndex) { return(Local(iIndex - 1)); }
private func & StartPosX(iTeam) { return(Local(iKingLocals + iTeam*2 - 2)); }
private func & StartPosY(iTeam) { return(Local(iKingLocals + iTeam*2 - 1)); }
private func & PlayerTeam(iPlr) { return (Local(iKingLocals*3 + iPlr)); }

public func GetIndexedKingdom(iIndex)
  {
  // Nur zugewiesene K�nige z�hlen
  var i=0;
  while (i < iKingLocals) if (Local(i++)) if (!iIndex--) return(i);
  // Index au�erhalb der Grenzen: 0 (Kein K�nigreich)
  return();
  }

public func GetKingdomData(iKingdom, &rszKDName, &ridKD, &rszKDDesc)
  {
  // K�nig suchen
  var pKing = King(iKingdom);
  if (!pKing) return();
  // Namen setzen
  rszKDName = pKing->~GetKingdomName();
  if (!GetChar(rszKDName)) rszKDName = Format("$MsgUnnamed$ #%d", iKingdom);
  // ID f�rs Symbol setzen
  ridKD = GetID(pKing);
  // Desc setzen
  rszKDDesc = "$MsgAKingdom$";
  // Fertig
  return(1);
  }

public func GetKingdomPos(iKingdom, &riX, &riY)
  {
  // K�nigreich zugewiesen?
  if (!King(iKingdom)) return();
  // Position setzen
  riX = StartPosX(iKingdom);
  riY = StartPosY(iKingdom);
  // Fertig
  return(1);
  }

private func FindPlayerOfTeam(iKingdom, iIndex)
  {
  // Alle Spieler durchgehen
  for (var i,j=GetPlayerCount(); j; ++i)
    if (GetPlayerName(i))
      {
      --j;
      // Spieler geh�rt dem Team an?
      if (PlayerTeam(i) == iKingdom)
        // Ergebnis gefunden!
        if (!iIndex--) return(i);
      }
  // Kein Ergebnis
  return(-1);
  }



/* Hilfsfunktionen */

private func LooksLikeKingdom(iX, iY)
  {
	// Au�erhalb der Landschaftsgrenzen?
	if (iX<0 || iX>=LandscapeWidth() || iY<0 || iY>=LandscapeHeight()) return();
  // Zur Sicherheit, damit nicht an 0/0 gesucht wird...
  if (!iX && !iY) ++iY;
  // K�nigreich == Burg
  var obj;
  while (obj = FindObject(0, iX,iY,0,0, 0, 0,0, NoContainer(), obj))
    if (obj->~CastlePartWidth()) return(1);
  // Kein Burgteil - Kein K�nigreich :(
  return();
  }

private func GetKingdomExtents(iKingdom, &rX, &rY, &rWdt, &rHgt)
  {
  // Sicherheit
  if (!King(iKingdom)) return();
  // Szenarienspezifisch?
  if (GameCall("KingdomExtents", King(iKingdom)))
    {
    rX = GameCall("KingdomExtentsX", King(iKingdom));
    rY = GameCall("KingdomExtentsY", King(iKingdom));
    rWdt = GameCall("KingdomExtentsWdt", King(iKingdom));
    rHgt = GameCall("KingdomExtentsHgt", King(iKingdom));
    return(1);
    }
  // Startposition suchen
  var sx = StartPosX(iKingdom), sy = StartPosY(iKingdom);
  // Von dort nach links nach einem Ende des K�nigreiches suchen
  rX=sx; while (LooksLikeKingdom(rX, sy)) rX -= 40;
  // Suche nach oben
  rY=sy; while (LooksLikeKingdom(sx, rY)) rY -= 40;
  // Suche nach rechts
  rWdt = sx-rX; while (LooksLikeKingdom(rX+rWdt, sy)) rWdt += 40;
  // Suche nach unten
  rHgt = sy-rY; while (LooksLikeKingdom(sx, rY+rHgt)) rHgt += 40;
  // Fertig; K�nigreich bestimmt
  return(1);
  }

private func RandomEliminationMsg(szTeamName)
  {
  var iMsg = Random($MsgEliminatedCnt$);
  if (!iMsg--) return(Format("$MsgEliminated0$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated1$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated2$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated3$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated4$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated5$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated6$", szTeamName));
  if (!iMsg--) return(Format("$MsgEliminated7$", szTeamName));
  if (!iMsg  ) return(Format("$MsgEliminated8$", szTeamName));
               return(Format("$MsgEliminated9$", szTeamName));
  }

private func RandomWinningMsg(szTeamName)
  {
  var iMsg = Random($MsgWonCnt$);
  if (!iMsg--) return(Format("$MsgWon0$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon1$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon2$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon3$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon4$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon5$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon6$", szTeamName));
  if (!iMsg--) return(Format("$MsgWon7$", szTeamName));
  if (!iMsg  ) return(Format("$MsgWon8$", szTeamName));
               return(Format("$MsgWon9$", szTeamName));
  }

private func RandomWinningMsg2()
  {
  var iMsg = Random($MsgWonACnt$);
  if (!iMsg--) return("$MsgWonA0$");
  if (!iMsg--) return("$MsgWonA1$");
  if (!iMsg--) return("$MsgWonA2$");
  if (!iMsg--) return("$MsgWonA3$");
  if (!iMsg--) return("$MsgWonA4$");
  if (!iMsg--) return("$MsgWonA5$");
  if (!iMsg--) return("$MsgWonA6$");
  if (!iMsg--) return("$MsgWonA7$");
  if (!iMsg  ) return("$MsgWonA8$");
               return("$MsgWonA9$");
  }

private func RandomJoinLeadMessage(szPlayerName, szTeamName)
  {
  var iMsg = Random($MsgKingCnt$);
  if (!iMsg--) return(Format("$MsgKing0$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing1$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing2$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing3$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing4$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing5$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing6$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing7$", szPlayerName, szTeamName));
  if (!iMsg--) return(Format("$MsgKing8$", szPlayerName, szTeamName));
               return(Format("$MsgKing9$", szPlayerName, szTeamName));
  }

private func RandomJoinMessage(szPlayerName, szTeamName)
  {
  var iMsg = Random($MsgTeamJoinCnt$);
  if (!iMsg--) return(Format("$MsgTeamJoin0$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin1$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin2$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin3$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin4$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin5$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin6$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin7$", szTeamName, szPlayerName));
  if (!iMsg--) return(Format("$MsgTeamJoin8$", szTeamName, szPlayerName));
               return(Format("$MsgTeamJoin9$", szTeamName, szPlayerName));
  }



/* Status */

private func IsGameRunning() { return(GetAction() eq "Checking"); }

public func IsFulfilled()
  {
  // Nur ein K�nig kann �berleben!
  return (iKingCount < 2);
  }

protected func Activate(iPlr)
  {
  var iTeam = PlayerTeam(iPlr), szTeamMsg;
  var szKDName, idKD, szKDDesc;
  if (iTeam) if (GetKingdomData(iTeam, szKDName, idKD, szKDDesc)) szTeamMsg = Format("$MsgTeamInfo$", szKDName);
  if (!szTeamMsg) szTeamMsg = "$MsgNoTeam$";
  var szKingdomMsg;
  if (iTeam)
    // Selber im Team
    if (iKingCount>1)
      if (iKingCount == 2)
        // Nur ein weiteres Team
        szKingdomMsg = "$MsgOneOtherTeams$";
      else
        // Mehr weitere Teams
        szKingdomMsg = Format("$MsgOtherTeams$", iKingCount-1);
    else
      // Keine weiteren Teams
      szKingdomMsg = "$MsgNoOtherTeams$";
  else
    // Kein eigenes Team
    if (iKingCount)
      if (iKingCount == 1)
        // Nur ein weiteres Team
        szKingdomMsg = "$MsgOneTeam$";
      else
        // Es gibt andere Teams
        szKingdomMsg = Format("$MsgTeams$", iKingCount);
    else
      // Es gibt keine K�nigreiche
      szKingdomMsg = "$MsgNoTeams$";
  // Nachricht zusammensetzen und ausgeben
  return(MessageWindow(Format("%s|%s|%s", GetDesc(), szTeamMsg, szKingdomMsg), iPlr));
  }

public func IsUnjoinedPlayer()
  {
  // Zur�ckgeben, ob noch Spieler ohne Team vorhanden sind
  for (var i,j=GetPlayerCount(); j; i++)
    if (GetPlayerName(i))
      {
      // Noch in der Teamauswahl?
      if (FindObjectOwner(_KSL, i)) return(1);
      --j;
      }
  // Alle OK
  return();
  }


public func GetUnjoinedPlayers()
  {
  // Namensstring aus nicht beigetretenen Spielern zusammensetzen
  var sResult, iNumPlrs, sName;
  for (var i,j=GetPlayerCount(); j; i++)
    if (sName = GetPlayerName(i))
      {
      // Noch in der Teamauswahl?
      if (FindObjectOwner(_KSL, i))
        if (iNumPlrs++)
          sResult = Format("%s, %s", sResult, sName);
        else
          sResult = sName;
      --j;
      }  
  // Fertig, Stirng zur�ckgeben
  return (sResult);
  }

public func IsSinglePlayerVsTeam(&riSinglePlrKingdom)
  {
  // Gibt es ein Einzelk�mpferk�nigreich und ein Mehrspielerk�nigreich?
  var fSinglePlrKingdom, fMultiPlrKingdom;
  // Alle Spieler auf ihre K�nigreiche anrechnen (Var())
  for (var i,j=GetPlayerCount(); j; i++)
    if (GetPlayerName(i))
      {
      ++Var(PlayerTeam(i));
      --j;
      }
  // K�nigreiche �berpr�fen
  var iTeam=iKingLocals+1;
  while (--iTeam)
    if (Var(iTeam) && StartPosX(iTeam))
      if (Var(iTeam) == 1)
        {
        fSinglePlrKingdom = 1;
        riSinglePlrKingdom = iTeam;
        }
      else
        fMultiPlrKingdom = 1;
  // Bedingung nur erf�llt, wenn beide Arten von K�nigreichen existieren
  return(fSinglePlrKingdom && fMultiPlrKingdom);
  }

public func GetSinglePlayer()
  {
  var iSingleTeam;
  // Einzelspielerk�nigreich vorhanden?
  if (!IsSinglePlayerVsTeam(iSingleTeam)) return(-1);
  // K�nigsspieler zur�ckgeben
  return(GetOwner(King(iSingleTeam)));
  }
