/*-- Verbranntes Burgteil --*/

#strict

/* Entz�ndung */

protected func Incineration()
{
  return(FCastleChange());
}

// Burgteile auf ihre Daseinsberechtigung �berpr�fen (Brustwehren inmitten der Burg, etc.)
public func FCastleChange()
{
  // Alle Teile benachrichtigen
  var pPart;
  while(pPart = FindObject(0, 0, 0, 0, 0, OCF_Fullcon(), 0,0, NoContainer(), pPart))
    pPart->~CastleChange();
  return(1);
}