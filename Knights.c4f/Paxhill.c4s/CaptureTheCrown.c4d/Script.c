/*-- Krone stehlen --*/

#strict
#include GOAL

static winner; // Wenn gesetzt, ID des Gewinnerteams

public func IsFulfilled()
{
  // Erf�llt wenn es schon einen Gewinner gibt
  if (winner) return true;
  
  // Wird auch erf�llt falls eine Seite nur �ber das Ziel "Teammelee" gewinnt.
  var melee = FindObject2(Find_ID(MELE));
  if(melee) return melee->IsFulfilled(...);
  
  // Nicht erf�llt
  return false;
}

protected func Activate(int plr)
{
  // Beschreibung ausgeben
  return MessageWindow(GetDesc(), plr);
}

public func OnCrownLost(object crown)
{
  // Schon vorbei?
  if (winner) return;
  // Das Gewinnerteam ist das letzte, das noch die Krone hat
  var crowns = FindObjects(Find_ID(_CRN), Find_Exclude(crown));
  if (GetLength(crowns) > 1 || !GetLength(crowns)) return;
  winner = GetPlayerTeam(GetOwner(crowns[0]));
  // Die anderen eliminieren
  for (var i=GetPlayerCount(); i; --i)
    {
    var plr = GetPlayerByIndex(i-1);
    if (GetPlayerName(plr))
      if (GetPlayerTeam(plr) != winner)
        EliminatePlayer(plr);
    }
}
