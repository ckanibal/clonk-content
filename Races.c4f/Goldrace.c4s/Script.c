/* Rennen */

#strict

// WICHTIG: damit man das Rennen auf Liga spielen kann
// muss Wettstreit(RVLR) aktiviert sein

func Initialize()
{
  // Statue erzeugen
  CreateObject(GIDL, LandscapeWidth()-20, 280, -1);
  return();
}

// -- Callbacks des Rennen-Spielziels --
// wenn diese Funktionen nicht im Szenarioscript sind
// oder 0 zur�ck geben, wird der Default-Wert verwendet

// Richtung:
// 1: links -> rechts
// 2: rechts -> links
// 3: untern -> oben
// 4: oben -> unten
// default: links -> rechts
func GetRACEDirection() { return(1); }

// Start: Anzahl an Pixeln, ab dem Rand, von dort beginnt die Z�hlung
// default: 50 px
func GetRACEStartOffset() { return(20); }

// Ende: Anzahl an Pixeln, ab dem Rand, bis dorthin geht die Z�hlung
// default: 50 px
func GetRACEEndOffset() { return(30); }

// Gewinnbedingung
//  1: Spieler hat gewonnen
//  0: gewinnen erlaubt wenn am Rand
// -1: kein gewinnen m�gliche
func CheckRACEGoal(iPlr)
{
  // In diesem Fall m�ssen die Spieler die Statue anfassen
  if(GetAction(GetCursor(iPlr)) S= "Push")
    if(GetActionTarget(0, GetCursor(iPlr)) == FindObject(GIDL))
      return(1); // Dann hat der Spieler gewonnen
  // Sonst muss er noch weiterspielen
  return(-1);
}

// Spielerbeitritt
func InitializePlayer(iPlr)
{
  return(JoinPlayer(iPlr));
}

// Relaunch
protected func RelaunchPlayer(int iPlr)
{
 // Verk�nden
 Log("$MsgRestart$", GetPlayerName(iPlr));

 // Ein neuer Clonk f�r den Spieler mit voller Energie
 var pClonk = CreateObject(CLNK, 10, 10, iPlr);
 MakeCrewMember(pClonk, iPlr);
 DoEnergy(100, pClonk);

 // Den Spieler neu starten lassen
 return(JoinPlayer(iPlr));
}

func JoinPlayer(iPlr)
{ 
  // Anw�hlen und in Position bringen
  SetCursor(iPlr,GetHiRank(iPlr));
  SelectCrew(iPlr, GetHiRank(iPlr), 1);
  SetPosition(10,260, GetCursor(iPlr)); // Hier die Koordinaten f�r die Relaunchposition anpassen!
}
