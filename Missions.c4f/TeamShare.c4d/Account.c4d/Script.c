/*-- Konto --*/

#strict 2

local aAlliancePlayers;
local aAllianceAccounts;

local iValue; // Geld auf dem Konto
local iScore; // Punkte
local iTick;  // Z�hler
local fSkipNextScore;
local iMyValue;

local fFirstAlliance;

local iMaxPlayerCount; // Wie viele Spieler maximal im Team waren

/* Zerst�rung */

protected func Destruction()
{
  var iPoints = GetPlayerValue(GetOwner());
  // Befreundete Accounts benachrichtigen
  for(var pAccount in aAllianceAccounts)
  {
    pAccount->UpdateValue(TACC_TypeScore,-iPoints);
    pAccount->AllianceDie(this);
  }
}

/* Initialisierung */

protected func Initialize()
{
  // Anfangsgeldwert
  iValue = GetWealth(GetOwner());
  iScore = GetPlrValueGain(GetOwner());

  // Anfangsbasismaterial
  var id;
  for(var i=0; id = GetHomebaseMaterial (GetOwner(), 0, i, C4D_All); i++)
    SetComponent(id, GetHomebaseMaterial(GetOwner(), id));

  // Z�hler und Array initialisieren
  iTick = 1;
  aAlliancePlayers  = [];
  aAllianceAccounts = [];
}

private func GetPlayerValue(iPlr)
{
  var iPoints;
  // Die Punkte des Spielers z�hlen (nur die Objekte, die nach seinem Tod auch weitergegeben werden)
  for(var pObj in FindObjects(Find_Owner(iPlr), Find_Not(Find_Category(C4D_StaticBack)), Find_Not(Find_OCF(OCF_CrewMember))))
    iPoints += pObj->GetValue();
  return iPoints;
}

/* �berpr�fung */
// Mit der Funktion TACC_BlockTeamExchange(int iType) im Szenarioscript kann der Austausch einzelner Tpyen geblockt werden

private func Execute()
{
  // Geh�rt niemandem (Spieler eliminiert?) - l�schen
  if (GetOwner() < 0) return RemoveObject();

  var iChange;
  // Geldwert�nderung?
  if (GetWealth(GetOwner()) != iValue && !GameCall("TACC_BlockTeamExchange", TACC_TypeValue))
  {
    // �nderung berechnen und anrechenen
    iChange = GetWealth(GetOwner())-iValue;
    iValue += iChange;
    // �nderung auf verb�ndete Konten anrechnen
    for(var pAccount in aAllianceAccounts) pAccount->UpdateValue(TACC_TypeValue, iChange);
  }
  // Punkte�nderung?
  if(fSkipNextScore == 0)
   if (GetPlrValueGain(GetOwner()) != iScore && !GameCall("TACC_BlockTeamExchange", TACC_TypeScore))
  {
    // �nderung berechnen und anrechenen
    iChange = GetPlrValueGain(GetOwner())-iScore;
    iScore += iChange;
    fSkipNextScore = 1;
    // �nderung auf verb�ndete Konten anrechnen
    for(var pAccount in aAllianceAccounts) pAccount->UpdateValue(TACC_TypeScore, iChange);
  }
  // Basismaterial
  var id, i;
  if(!GameCall("TACC_BlockTeamExchange", TACC_TypeBaseMat))
  while(id = GetHomebaseMaterial (GetOwner(), 0, i++, C4D_All))
  {
    // Basismaterial�nderung?
    if (GetHomebaseMaterial(GetOwner(), id) != GetComponent(id))
    {
      // �nderung berechnen und anrechenen
      iChange = GetHomebaseMaterial(GetOwner(), id)-GetComponent(id);
      SetComponent(id, GetComponent(id)+iChange);
      // �nderung auf verb�ndete Konten anrechnen
      for(var pAccount in aAllianceAccounts) pAccount->UpdateValue(TACC_TypeBaseMat, iChange, id);
    }
  }
  // Alle 35 Aufrufe CheckAlliances durchf�hren
  if (!--iTick) AllChk();
}

/* Wert �ndern */

static const TACC_TypeValue   = 1;
static const TACC_TypeScore   = 2;
static const TACC_TypeBaseMat = 3;
static const TACC_TypePlans   = 4;
static const TACC_TypeSpells  = 5;

public func UpdateValue(iType, iByAmount, idTypeID)
{
  // Geld
  if(iType == TACC_TypeValue && !GameCall("TACC_BlockTeamExchange", TACC_TypeValue))
  {
    iValue+=iByAmount;
    iMyValue -= iByAmount;
    SetWealth(GetOwner(), GetWealth(GetOwner()) + iByAmount);
    return;
  }
  // Punkte
  if(iType == TACC_TypeScore && !GameCall("TACC_BlockTeamExchange", TACC_TypeScore))
  {
    iScore+=iByAmount;
    iMyValue += iByAmount;
    fSkipNextScore = 1;
    return;
  }
  // Basismaterial
  if(iType == TACC_TypeBaseMat && !GameCall("TACC_BlockTeamExchange", TACC_TypeBaseMat))
  {
    if(!idTypeID) return;
    SetComponent(idTypeID, GetComponent(idTypeID)+iByAmount);
    DoHomebaseMaterial(GetOwner(), idTypeID, iByAmount);
    return;
  }
}

public func GetValue(iType, idTypeID)
{
  // Geld
  if(iType == TACC_TypeValue)
    return iValue;
  // Punkte
  if(iType == TACC_TypeScore)
    return iScore;
  // Basismaterial
  if(iType == TACC_TypeBaseMat)
    return GetComponent(idTypeID);
}

protected func CalcValue()
{
  fSkipNextScore = 0;
  return iMyValue;
}


/* Allianz�nderungen pr�fen und Konten entsprechend umkonfigurieren */

private func AllChk()
{
  // Z�hler zur�cksetzen
  iTick=35;
  // Pr�fen, ob Allianzen gebrochen wurden
  var i,iOwner = GetOwner(), iPlr;
  // Alle Konten der Teammitglieder durchgehen
  for (iPlr in aAlliancePlayers)
    // Konto vorhanden und verfeindet?
    if(Hostile(iOwner,iPlr))
      // Konto l�schen
      BreakAlliance(iPlr);

  // Pr�fen, ob neue Allianzen geschlossen wurden
  for(i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spieler ermitteln
    iPlr = GetPlayerByIndex(i);
    // Nicht verfeindet und noch kein Konto reserviert?
    if(!Hostile(iOwner,iPlr) && GetIndexOf(iPlr, aAlliancePlayers)==-1)
      // Neues Konto erstellen
      CreateAlliance(iPlr);
  }
}

/* Konto aus den Teamkonten l�schen */

private func BreakAlliance(iPlr)
{
  // Alle Allianzen brechen, dabei alles aufteilen
  var iCount = GetLength(aAlliancePlayers)+1, id;

  for(var pAccount in aAllianceAccounts)
  {
    // Geld anpassen
    pAccount->UpdateValue(TACC_TypeValue, -iValue/iCount);
    // Punkte anpassen
    pAccount->UpdateValue(TACC_TypeScore, -iScore/iCount);
    // Material anpassen
    for(var i=0; id = GetHomebaseMaterial (GetOwner(), 0, i, C4D_All); i++)
      pAccount->UpdateValue(TACC_TypeBaseMat, -GetComponent(id)/iCount, id);

    // Und diesen Spieler aus der Allianz l�schen
    pAccount->RemoveAlliance(this);
    // Uns selbst nat�rlich auch l�schen :)
    RemoveAlliance(pAccount);
  }
  UpdateValue(TACC_TypeValue, iValue/iCount-iValue);
  UpdateValue(TACC_TypeScore, iScore/iCount-iScore);
  for(var i=0; id = GetHomebaseMaterial (GetOwner(), 0, i, C4D_All); i++)
      UpdateValue(TACC_TypeBaseMat, GetComponent(id)/iCount - GetComponent(id), id);
}

/* Neuen Spieler in die Allianz aufnehmen */
// Wenn TACC_BlockFirstAllianceAdd im Szenarioscript 1 zur�ckgibt,
// wird das Anfangsguthaben des Teams als das vom ersten Mitglied genommen,
// statt aller Mitglieder zusammen

private func CreateAlliance(iWithPlr)
{
  // Verb�ndete Spieler �berpr�fen: Alle m�ssen verb�ndet sein
  var pAccount = FindObjectOwner(ACNT, iWithPlr);
  if(!pAccount) return;
  if(!CheckAlliances(pAccount)) return;
  // Werte anpassen
  var iNewValue;
  if(!GameCall("TACC_BlockFirstAllianceAdd")) fFirstAlliance = 1;
  // Geld
  iNewValue = pAccount->AddAllianceValue(TACC_TypeValue, iValue, 0, !fFirstAlliance);
  AddAllianceValue(TACC_TypeValue, iNewValue-iValue);
  // Punkte
  iNewValue = pAccount->AddAllianceValue(TACC_TypeScore, iScore, 0, !fFirstAlliance);
  AddAllianceValue(TACC_TypeScore, iNewValue-iScore);
  // Basismaterial
  var id;
  for(var i=0; id = GetHomebaseMaterial (GetOwner(), 0, i, C4D_All); i++)
  {  
    iNewValue = pAccount->AddAllianceValue(TACC_TypeBaseMat, GetComponent(id), id, !fFirstAlliance);
    AddAllianceValue(TACC_TypeBaseMat, iNewValue-GetComponent(id), id);
  }
  for(var i=0; id = GetHomebaseMaterial (iWithPlr, 0, i, C4D_All); i++)
  {
    if(GetHomebaseMaterial(GetOwner(), id)) continue; // Wurde schon behandelt
    iNewValue = pAccount->AddAllianceValue(TACC_TypeBaseMat, GetComponent(id), id, !fFirstAlliance);
    AddAllianceValue(TACC_TypeBaseMat, iNewValue-GetComponent(id), id);
  }
  fFirstAlliance = 1;
  // Allianz hinzuf�hen
  pAccount->AddAlliances(this);
  AddAlliances(pAccount);

  return 1;
}

/* Pr�fen, ob dieser Spieler mit der Allianzgruppe pWith verb�ndet ist */

private func CheckAlliances(pWith)
{
  // Allianzen f�r alle verb�ndeten Konten pr�fen...
  for (var pAccount in aAllianceAccounts)
    if (!CheckAlliance(pAccount, pWith)) return false;
  // ...und f�r sich selbst
  return CheckAlliance(this, pWith);
}

/* Pr�fen, ob der Account pAcc mit pWith verb�ndet ist */

public func CheckAlliance(pAcc, pWith)
{
  // Besitzer von pAcc ermitteln 
  var iOwner = GetOwner(pAcc);
  // Jeden Spieler durchgehn
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spielernummer ermitteln
    var iPlr = GetPlayerByIndex(i);
    // Konto f�r diesen Spieler in pWith vorhanden?
    if( pWith->CheckPlayer() )
      // Verfeindet?
      if(Hostile(iOwner, iPlr) )
        // Dann war's wohl nix.
        return 0;
  }
  // Zuletzt noch pr�fen, ob der Spieler mit pWith selbst verb�ndet ist.
  return !Hostile(iOwner, GetOwner(pWith));
}

public func CheckPlayer(iPlr)
{
  // Pr�ft, ob ein Spieler schon in der Allianz ist
  if(GetIndexOf(iPlr, aAlliancePlayers) == -1) return false;
  return true;
}

public func GetAlliancePlayerCount(fMaximalCount)
{
  if(fMaximalCount) return iMaxPlayerCount+1;
  return GetLength(aAlliancePlayers)+1;
}

public func GetPlayerAccount(iPlr)
{
  // Sucht das Konto eines Spielers
  for (var pAccount in aAllianceAccounts)
    if(GetOwner(pAccount) == iPlr)
      return pAccount;
}

// F�gt einer ganzen Allianz einen Wert von einem bestimmten Typen zu
public func AddAllianceValue(iType, iAddValue, idTypeID, fSet)
{
  if(fSet) iAddValue -= GetValue(iType, idTypeID);
  // Bei Verb�ndeten hinzuf�gen...
  for (var pAccount in aAllianceAccounts)
    pAccount->UpdateValue(iType, iAddValue, idTypeID);
  // ...und bei sich selbst
  UpdateValue(iType, iAddValue, idTypeID);
  return GetValue(iType, idTypeID);
}

public func AddAlliances(pGroup)
{
  // Bei Verb�ndeten hinzuf�gen...
  for (var pAccount in aAllianceAccounts)
    // Jo, also hinzuf�gen
    pAccount->AddAlliance(pGroup);
  // ...und bei sich selbst
  AddAlliance(pGroup);
}

public func AddAlliance(pGroup)
{
  var o = GetOwner(pGroup);
  // Jeden Spieler durchgehn
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spielernummer ermitteln
    var iPlr = GetPlayerByIndex(i);
    // Verb�ndet mit dem neuen, aber nicht mit uns?
    if(pGroup->CheckPlayer(iPlr) && iPlr != GetOwner() && !CheckPlayer(iPlr))
    {
      // Dazuf�gen
      aAllianceAccounts[GetLength(aAllianceAccounts)] = pGroup->GetPlayerAccount(iPlr);
      aAlliancePlayers[GetLength(aAlliancePlayers)] = iPlr;
    }
  }
  
  // Auch hier neue Allianz hinzuf�gen
  aAllianceAccounts[GetLength(aAllianceAccounts)] = pGroup;
  aAlliancePlayers[GetLength(aAlliancePlayers)] = GetOwner(pGroup);
  // Maximale Spielerzahl aktualisieren
  if(iMaxPlayerCount < GetLength(aAllianceAccounts)) iMaxPlayerCount = GetLength(aAllianceAccounts);
}

/*  Allianz stirbt: Geldwert belassen und Z�hler verringern */

public func AllianceDie(pDeadAccount)
{
  var iIndex = GetIndexOf(pDeadAccount, aAllianceAccounts);
  DeleteArrayItem(iIndex, aAlliancePlayers);
  DeleteArrayItem(iIndex, aAllianceAccounts);
}

public func RemoveAlliance(pDeadAccount)
{
  var iIndex = GetIndexOf(pDeadAccount, aAllianceAccounts);
  DeleteArrayItem(iIndex, aAlliancePlayers);
  DeleteArrayItem(iIndex, aAllianceAccounts);
}


/* Info */

public func GetTeam()
{
  // Kein Team? :'(
  if (!GetLength(aAlliancePlayers)) return "$NoAccount$";

  // Alle Allianzmitglieder speichern
  var szResult, i = GetLength(aAlliancePlayers)-1;
  for(var iPlr in aAlliancePlayers)
  {
    if(!szResult) szResult = Format("$AccountWith$ %s", GetPlayerName(iPlr));
    else if(!i) szResult = Format("%s $And$ %s", szResult, GetPlayerName(iPlr));
    else szResult = Format("%s, %s", szResult, GetPlayerName(iPlr));
    i--;
  }
      
  return szResult;
}

// L�scht ein Item aus einem Array
private func DeleteArrayItem(iNumber, &aArray)
{
 var temp=[];
 for(var cnt;cnt<GetLength(aArray);cnt++)
 {
  if(cnt==iNumber)continue;
  var dif=0;
  if(cnt>iNumber)dif=-1;
  temp[cnt+dif]=aArray[cnt];
 } 
 
 aArray=temp;
 return aArray;
}
