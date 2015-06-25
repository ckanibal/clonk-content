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
  // Mana-Regeneration (nur wenn der K�nig im Freien ist)
  if (!Random(20))
    if (Contained())
      if (!Contained(Contained()))
        if (DoMagicEnergy(1, Contained())) CreateParticle("PxSpark",0,0,0,0,10,RGB(0,0,255));
  // Effekt
  return(SetAction(Format("Glitter%d", Random(5))));
  }

public func IsMagicProduct() { return(true); }
