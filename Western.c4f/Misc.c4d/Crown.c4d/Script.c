#strict

protected func Initialize()
  {
  // Blinkaktivit�t setzen
  SetAction("Glitter0");
  return(1);
  }

// Auftreffsound
protected func Hit() { return(Sound("Crystal*")); }

// Aktivit�t
protected func Glittering()
  {
  // Effekt
  return(SetAction(Format("Glitter%d", Random(5))));
  }
