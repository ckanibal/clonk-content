/*-- Turm mit Zugbr�cke (rechts) --*/

#strict

#include CPT4 // Burgteil - Treppenhaus - Turm - Zugbr�cke

/* Zugbr�cke erzeugen */

private func InitializeTower()
{
  CreateObject(CPBR, 40, 35, GetOwner());
}

/* Transfer */

public func UpdateTransferZone()
{
  // Normale Transferzone
  var x = -14;
  var w = 33;
  // Zugbr�cke geschlossen: breitere Transferzone
  if (FindDrawbridgeUp())
  	{ x = -14; w = 39; }
  // Mit dar�berliegendem Niedergang
  if (FindStaircaseAbove())
    if (GetOCF() & OCF_Fullcon())
      return(SetTransferZone(x, -47, w, 61));
  // Ohne dar�berliegenden Niedergang (kein Auf- oder Abstieg m�glich)
  return(SetTransferZone(x, 10, w, 4));
}

/* Forschung */

public func GetResearchLink() { return(CPT4); }
