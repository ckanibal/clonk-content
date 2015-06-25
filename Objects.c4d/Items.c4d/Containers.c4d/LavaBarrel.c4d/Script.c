/*-- Lavafass --*/

#strict

#include BARL

// Fassdaten
public func BarrelMaterial()
  {
  // Wenn m�glich tats�chliches F�llmaterial ausgeben
  if (iFillMaterial) return (iFillMaterial-1);
  // iFillMaterial nicht gesetzt bei F�ssern aus alten Spielst�nden, oder z.B. bei gekauften F�ssern
  // Dann ist das Fass mit normaler Lava gef�llt
  return(Material("Lava"));
  }
  
public func BarrelMaterialName()
  {
  // Wenn m�glich tats�chliches F�llmaterial ausgeben
  if (iFillMaterial) return (MaterialName(iFillMaterial-1));
  return("Lava");
  // iFillMaterial nicht gesetzt bei F�ssern aus alten Spielst�nden, oder z.B. bei gekauften F�ssern
  // Dann ist das Fass mit normaler Lava gef�llt
  }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (iMaterial == Material("Lava")) return(LBRL);
  if (iMaterial == Material("DuroLava")) return(LBRL);
  return(_inherited(iMaterial));
}