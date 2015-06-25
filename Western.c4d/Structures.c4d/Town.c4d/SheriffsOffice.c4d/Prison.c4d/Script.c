/*-- Gef�ngnis --*/

#strict

local office;

public func Office(object pOffice)
{
  office = pOffice;
}

/* Einsperren */

public func Imprison(object pBandit)
{
  // Wir sperren mal heiter einen Banditen ein!

  // Bandit kommt hier rein
  Enter(this(), pBandit);
  SetCrewEnabled(0, pBandit);
  // Alle Gegenst�nde werden weggenommen
  var pCon;
  while(pCon = Contents(0, pBandit))
    {
    // Ab ins B�ro damit
    Exit(Contents(0, pBandit));
    Enter(office, pCon);
    }
  // Ger�usch?! :(
}

/* Ausbrechen */

public func Jailbreak()
{
  // Alle Banditen werden frei
  var living, counter;
  while(living = FindObject(0, 0,0,0,0, OCF_Alive(), 0,0, this()))
    {
    SetCrewEnabled(1, living);
    Exit(living, AbsX(GetX(office))+RandomX(-20,20), AbsY(GetY(office))+20);
    counter++;
    }
  if(counter) Message("$MsgBanditsFree$");
  return(counter);
}

// Ist ein staatlich gepr�ftes Gef�ngnis
public func IsPrison() { return(1); }
