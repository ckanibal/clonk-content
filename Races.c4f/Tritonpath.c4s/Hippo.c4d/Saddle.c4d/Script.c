/*-- Sattel --*/

#strict

private func ID_Horse() { return(HICP); }
private func ID_HorseSaddled() { return(HISD); }

public func Activate(object pClonk)
{
  [$TxtPuton$]
  // Kein Pferd
  var pHorse;
  if (!(pHorse = FindObject(ID_Horse(), 0, 1))) return(0);
  // Pferd satteln
  if (!pHorse->Saddle(ID_HorseSaddled(), GetID())) return(0);
  pHorse->SetOwner(GetOwner(pClonk));
  // Sattel entfernen
  RemoveObject();
  // Fertig
  return(1);
}

/* Eigenschaften */

public func IsAnvilProduct() { return(1); }

public func IsSaddle() { return(1); }