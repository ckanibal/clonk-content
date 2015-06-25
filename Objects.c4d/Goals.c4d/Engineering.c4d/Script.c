/*-- Siedlung --*/

#strict
#include GOAL

public func IsFulfilled () {
  var cnt = GetPlayerCount(), plr;
  if (!cnt) return(false); // Kein Spieler
  // Baupl�ne aller Spieler auf Vorhandensein pr�fen
  for (var j=0; j<cnt; ++j) if (GetPlayerName(plr = GetPlayerByIndex(j)))
    {
    var def, i = 0;
    //Alle Baupl�ne durchgehen
    while (def = GetPlrKnowledge (plr, 0, i++, C4D_All))
      {
      // Schon gecheckt?
      var k;
      for (k=0; k<j; ++k) if (GetPlrKnowledge(GetPlayerByIndex(k), def)) break;
      if (k < j) continue;
      // Objekt ist gebaut - egal von welchem Spieler
      if(FindObject(def, 0,0,0,0, OCF_Fullcon)) continue;
      // oder Stellvertreterobjekt ist gebaut (z.B. bei Gesch�tzt�rmen)
      if(def->~GetAlternativeType())
        if(FindObject(def->~GetAlternativeType(), 0,0,0,0, OCF_Fullcon)) continue;
      //Wenn es kein passendes Objekt gibt, ist das Spielziel nicht erf�llt
      return false;
      }
    }
  // Kein Bauplan nicht umgesetzt -> erf�llt
  return true;
}

public func IsFulfilledforPlr (plr) {
  var def, i;
  //Alle Baupl�ne durchgehen
  while (def = GetPlrKnowledge (plr, 0, i++, C4D_All))
  {
    // Objekt ist gebaut
    if(FindObjectOwner(def, plr, 0, 0, 0, 0, OCF_Fullcon)) continue;
    // oder Stellvertreterobjekt ist gebaut (z.B. bei Gesch�tzt�rmen)
    if(def->~GetAlternativeType())
      if(FindObjectOwner(def->~GetAlternativeType(), plr, 0, 0, 0, 0, OCF_Fullcon)) continue;
    //Wenn es kein passendes Objekt gibt, ist das Spielziel nicht erf�llt
    return(false);
  }
  return (true);
}

public func Activate(act_plr) 
{
  // Alle Baupl�ne durchgehen...
  var show_all_player_plans = !ObjectCount(RVLR), done, def, i, plr;
  // Geschafft
  if (show_all_player_plans) done = IsFulfilled(); else done = IsFulfilledforPlr(act_plr);
  if (done) return MessageWindow("$done$", act_plr);
  var obj = GetCursor(act_plr), name = GetName();
  CreateMenu(GetID(), obj, this(), 0, name);
  // Im Wettkampfmodus nur nicht umgesetzte Baupl�ne des eigenen Spielers anzeigen; ansonsten alle Spielerbaupl�ne pr�fen
  if (show_all_player_plans)
    {
    var cnt = GetPlayerCount();
    // Alle nicht umgesetzten Baupl�ne anzeigen
    for (var j=0; j<cnt; ++j) if (GetPlayerName(plr = GetPlayerByIndex(j)))
      {    
      i = 0;
      //Alle Baupl�ne durchgehen
      while (def = GetPlrKnowledge (plr, 0, i++, C4D_All))
        {
        // Schon gecheckt?
        var k;
        for (k=0; k<j; ++k) if (GetPlrKnowledge(GetPlayerByIndex(k), def)) break;
        if (k < j) continue;
        // Objekt ist gebaut - egal von welchem Spieler
        if(FindObject(def, 0,0,0,0, OCF_Fullcon)) continue;
        // oder Stellvertreterobjekt ist gebaut (z.B. bei Gesch�tzt�rmen)
        if(def->~GetAlternativeType())
          if(FindObject(def->~GetAlternativeType(), 0,0,0,0, OCF_Fullcon)) continue;
        // Ansonsten dieses ins Men�
        AddMenuItem(name, "And()", def, obj, 1, 0, Format("$MsgGoalTodo$", GetName(0, def)));
        }
      }
    }
  else
    {
    // Wettkampfmodus
    i = 0;
    while (def = GetPlrKnowledge(act_plr, 0, i++, C4D_All))
      {
      //...und ins Men� tun, wenn sie noch fehlen
      if(FindObjectOwner(def, act_plr, 0, 0, 0, 0, OCF_Fullcon)) continue;
      if(def->~GetAlternativeType())
        if(FindObjectOwner(def->~GetAlternativeType(), act_plr, 0, 0, 0, 0, OCF_Fullcon)) continue;
    
      AddMenuItem(GetName(), "And()", def, obj, 1, 0, Format("$MsgGoalTodo$", GetName(0, def)));
      }
    }
  return true;
}
