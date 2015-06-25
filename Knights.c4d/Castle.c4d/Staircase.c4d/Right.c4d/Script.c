/*-- Treppenhaus rechts --*/

#strict

#include CPT1 // Burgteil - Treppenhausfunktion

/* Grafische Anpassung in der Burg */

protected func Initialize()
{
  // Regul�re Initialisierung
  _inherited();
  // Burgfunktion pr�fen
  return(CastleChange());
}

protected func UpdateTransferZone()
{
  // Burgfunktion pr�fen. Das ererbte CastleChange wuerde erneut
  // UpdateTransferZone aufrufen, also wird hier auf den Aufruf an
  // die ueberladene Funktion verzichtet.
  CastleChangeImpl();
  // �berladener Aufruf
  return(_inherited());
}

public func CastleChange()
{
  CastleChangeImpl();
  return(_inherited());
}

public func CastleChangeImpl()
{
  // Ist rechts ein Burgteil?
  var cpr;
  if (cpr=FindCastlePart(CastlePartWidth()/2+CastlePartRange()))
  {
    // Immer vor diesem Burgteil
    SetObjectOrder(cpr);
    // Aktivit�t anpassen
    SetAction("InCastle");
  }
  else
  {
    // Sonst idle
    SetAction("Idle");
  }
}

/* Forschung */

public func GetResearchLink() { return(CPWL); }

