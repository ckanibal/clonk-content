#strict
#appendto EXTG

private func OnFire(o)
  {
  if (GetCategory(o) & 3) return(); // Burgteile nicht l�schen, da man sie neu aufbauen k�nnte
  return (_inherited(o));
  }