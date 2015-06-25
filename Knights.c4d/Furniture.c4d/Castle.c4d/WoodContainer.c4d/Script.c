/*-- Holzbeh�lter --*/

#strict

/* Initialisierung */

protected func Initialize()
  {
  // Holzanzeigeaktivit�t setzen
  SetAction("ShowWood");
  return(1);
  }


/* Statusanzeige */

// Aktualisierung bei Aufnahme und Herausnahme
protected func Collection2() { return(CountContents()); }
protected func Ejection() { return(CountContents()); }

private func CountContents()
  {
  // Z�hlt eingelegtes Holz (maximal 9 Animationsphasen)
  SetPhase(Min(ContentsCount(WOOD),9));
  return(1);
  }


/* Aufnahme */

// Effekt
protected func Collection() { return(Sound("WoodContainer")); }

// Aufnahmebeschr�nkung
protected func RejectCollect(idObj, pObj)
  {
  // Verhindert Aufnahme aller nichtpassender Objekte
  if( idObj != WOOD) return(1);
  // Maximal 25 St�ck
  if (ContentsCount(WOOD) < 25) return();
  // Aufnahme ablehnen
  Message("$MsgContainerFull$",this());
  return(1);
  }


/* Abgabe */

protected func ControlDig(pClonk)
  {
  [$CtrlTake$|Image=WOOD]
  // Gegenstand nehmen
  var pItem = Contents();
  if (!pItem) return(Message("$MsgContainerEmpty$", this()));
  if (Collect(pItem, pClonk)) Sound("Grab");
  return(1);
  }
