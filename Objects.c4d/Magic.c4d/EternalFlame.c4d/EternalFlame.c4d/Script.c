/* Ewige Flamme */

#strict

protected func BurnProcess()
  {
  // Brennvorgang: Ende wenn im Erdreich oder gel�scht
  if (GBackSolid()) { Sound("Fire"); RemoveObject(); return(); }
  if (!OnFire()) { RemoveObject(); return(); }
  return(1);
  }

protected func Completion()
  {
  // Flamme entz�nden
  Incinerate();
  SetAction("Burn");
  SetComDir(COMD_Down());
  // Effekt anh�ngen, damit Entzauberung die Flamme entfernt
  if (AddEffect("MFireNSpell", this(), 75, 0, this()) <= 0) return(RemoveObject());
  return(1);
  }

protected func Damage()
  {
  // Flamme entfernen wenn sie Schaden nimmt
  Sound("Fire");
  RemoveObject();
  return(1);
  }

protected func FxMFireNSpellStop(object pTarget, int iEffectNumber, int iReason)
  {
  // Effekt per Script entfernt: Flamme l�schen (entfernt sich dann selber)
  if (iReason) return(1);
  Extinguish(pTarget);
  return(1);
  }
