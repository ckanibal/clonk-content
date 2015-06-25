/*--- Schie�pulver ---*/

#strict

protected func Hit()
{
  Sound("WoodHit*");
  return(1);
}  
  
protected func Damage()
{
  // Explodiert nicht, wenn er von einem Clonk getragen wird (Sonderregel gegen T-Flint-Superjumps)
  if (Contained())
    if (Contained()->~IsClonk())
    {
      Extinguish();
      return(0);
    }
  // Explosion - verz�gert, um Endlosrekursion zu verhindern
  Schedule("Explode(30)", 1);
  return(1);
}

// Entz�ndet sich bei Explosionstreffer
public func OnShockwaveHit(level, x, y)
{
 Incinerate();
}

/* Kann in der Chemiefabrik hergestellt werden */
public func IsChemicalProduct() { return(1); }

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }
