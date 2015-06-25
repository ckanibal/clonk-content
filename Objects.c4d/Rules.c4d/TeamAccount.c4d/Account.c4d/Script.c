/*-- Konto --*/

#strict

local value; // Geld auf dem Konto
local tick;  // Z�hler
local lastA; // Gr��te Spielernummer der Verb�ndeten
local cnt;   // Anzahl Verb�ndeter

// Local(n): Konto von Spieler n, wenn befreundet

/* Zerst�rung */

protected func Destruction()
{
  // Befreundete Accounts benachrichtigen
  var i,acc;
  while (i<=lastA)
    if (acc=Local(i++))
      acc->AllianceDie(this() );
}

/* Initialisierung */

protected func Initialize()
{
  // Anfangsgeldwert
  value=GetWealth(GetOwner());

  // Z�hler initialisieren
  tick=35;
  lastA=-1;
}

/* �berpr�fung */

private func Execute()
{
  // Geh�rt niemandem (Spieler eliminiert?) - l�schen
  if (GetOwner() < 0) return(RemoveObject() );

  // Geldwert�nderung?
  if (GetWealth(GetOwner()) != value)
  {
    // �nderung berechnen und anrechenen
    var d=GetWealth(GetOwner())-value;
    value+=d;
    // �nderung auf verb�ndete Konten anrechnen
    var i,acc;
    while (i<=lastA) if (acc=Local(i++)) acc->UpdateValue(d);
  }
  // Alle 35 Aufrufe CheckAlliances durchf�hren
  if (!--tick) AllChk();
}

/* Kontobestand �ndern */

public func UpdateValue(iByAmount)
{
  // �nderung auf value und Wealth getrennt anrechnen, da beide verschieden sein k�nnen,
  // wenn zwei Konten im gleichen Frame ge�ndert wurden
  value+=iByAmount;
  SetWealth(GetOwner(), GetWealth(GetOwner()) + iByAmount);
}

/* Allianz�nderungen pr�fen und Konten entsprechend umkonfigurieren */

private func AllChk()
{
  // Z�hler zur�cksetzen
  tick=35;
  // Pr�fen, ob Allianzen gebrochen wurden
  var i,o = GetOwner();
  // Alle Konten der Teammitglieder durchgehen
  for (;i<=lastA;i++)
    // Konto vorhanden und verfeindet?
    if(Local(i) && Hostile(o,i))
      // Konto l�schen
      BreakAlliance(i);

  // Pr�fen, ob neue Allianzen geschlossen wurden
  for(i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spieler ermitteln
    var plr = GetPlayerByIndex(i);
    // Nicht verfeindet und noch kein Konto reserviert?
    if(!Hostile(o,plr) && !Local(plr))
      // Neues Konto erstellen
      CreateAlliance(plr);
  }
}

/* Konto aus den Teamkonten l�schen */

private func BreakAlliance(iPlr)
{
  // Alle Allianzen brechen, dabei Geld aufteilen
  var v1=value/(cnt+1);
  UpdateValue(v1-value);
  var o = GetOwner();

  // Alle Konten durchgehen
  for (var i = 0;i<=lastA;i++) if (Local(i))
  {
    // Auch hier Geld anpassen
    Local(i)->UpdateValue(-v1);
    // Und diesen Spieler aus der Allianz l�schen
    Local(i)->RemoveAlliance(o);
    // Uns selbst nat�rlich auch l�schen :)
    RemoveAlliance(i);
  }

  cnt=0;
  lastA=-1;
}

/* Neuen Spieler in die Allianz aufnehmen */

private func CreateAlliance(iWithPlr)
{
  // Verb�ndete Spieler �berpr�fen: Alle m�ssen verb�ndet sein
  var acc=FindObjectOwner(ACNT,iWithPlr);
  if(!acc) return(0);
  if (!CheckAlliances(acc)) return(0);
  // Neuen letzten Verb�ndeten berechnen
  lastA=Max(lastA, iWithPlr);
  // Allianz hinzuf�gen; Locals setzen
  var v2 = ObjectCall(acc,"AddAlliances",this(),value); //acc->AddAlliances(this(), value);
  AddAlliances(acc, v2-value);

  return(1);
}

/* Pr�fen, ob dieser Spieler mit der Allianzgruppe pWith verb�ndet ist */

private func CheckAlliances(pWith)
{
  // Allianzen f�r alle verb�ndeten Konten pr�fen...
  for (var i = 0;i<=lastA;i++) if (Local(i))
    if (!CheckAlliance(Local(i), pWith)) return(0);
  // ...und f�r sich selbst
  return(CheckAlliance(this(), pWith));
}

/* Pr�fen, ob der Account pAcc mit pWith verb�ndet ist */

public func CheckAlliance(pAcc, pWith)
{
  // Besitzer von pAcc ermitteln 
  var o = GetOwner(pAcc);
  // Jeden Spieler durchgehn
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spielernummer ermitteln
    var plr = GetPlayerByIndex(i);
    // Konto f�r diesen Spieler in pWith vorhanden?
    if(Local(plr, pWith) )
      // Verfeindet?
      if(Hostile(o, plr) )
        // Dann war's wohl nix.
        return(0);
  }
  // Zuletzt noch pr�fen, ob der Spieler mit pWith selbst verb�ndet ist.
  return(!Hostile(o, GetOwner(pWith)) );
}

public func AddAlliances(pGroup, iAddValue)
{
  // Bei Verb�ndeten hinzuf�gen...
  for (var i = 0;i<=lastA;i++)
    // Verb�ndet?
    if (Local(i))
      // Jo, also hinzuf�gen
      Local(i)->AddAlliance(pGroup, iAddValue);
  // ...und bei sich selbst
  AddAlliance(pGroup, iAddValue);
  return(value);
}

public func AddAlliance(pGroup, iAddValue)
{
  // Locals setzen
  var o = GetOwner(pGroup);
  // Jeden SPieler durchgehn
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spielernummer ermitteln
    var plr = GetPlayerByIndex(i);
    // Verb�ndet?
    if(Local(plr, pGroup) )
    {
      // Sind wie das selbst?
      if(Local(plr) || plr == GetOwner() ) continue;
      // Konto hinzuf�gen
      Local(plr) = Local(plr, pGroup);
      // Neue h�chste Spielernummer in diesem Team ermitteln
      lastA = Max(lastA, plr);
      // Diese Allianz hat ein Mitglied mehr
      ++cnt;
    }
  }
  
  // Auch hier neue Allianz hinzuf�gen
  Local(o)=pGroup;
  // Und neue h�chste Spielernummer setzen
  lastA=Max(lastA, o);
  ++cnt;
  // Geldwert anpassen
  UpdateValue(iAddValue);
}

/*  Allianz stirbt: Geldwert belassen und Z�hler verringern */

public func AllianceDie()
{
  return(--cnt);
}

public func RemoveAlliance(iAlliance)
{
  // Allianz entfernen; Z�hler verringern
  if (!Local(iAlliance)) return(0);
  Local(iAlliance) = 0;
  --cnt;
  while (!Local(lastA) && lastA) --lastA;
  return(1);
}


/* Info */

public func GetTeam()
{
  // Kein Team? :'(
  if (!cnt) return();
  // Alle Allianzmitglieder speichern
  var j,k;
  for (var i = 0; i <= lastA; i++)
    if (Local(i))
      Var(j++) = GetPlayerName(i);

  // Erstes Mitglied holen
  var rs = Var(0);

  // Und alle weiteren mit Komma getrennt dranh�ngen
  while (++k < j)
    if (Var(k))
      rs = Format("%s, %s", rs, Var(k));
      
  return(rs);
}
