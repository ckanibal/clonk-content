/*-- Gemeinschaftsiglu --*/

#strict

#include WRKS
#include IGLO

private func MaxOccupant() { return(6); }

/* ~#exclude f�r das fundament */

protected func Construction() {}
protected func Destruction() {}

/* Eingangssteuerung */

protected func ActivateEntrance(pByObj) { // (koordinaten ge�ndert)
  // Raus geht's immer
  if (Contained(pByObj)==this())
    return(Exit(pByObj,22,14));
  // Weitere Clonks k�nnen nur rein, bis die Maximalzahl erreicht ist
  if (GetOCF(pByObj)&OCF_CrewMember())
    if (IsOccupied())
      return(0,Message("$Full$",this()));
  // Sonst Objekt aufnehmen
  return(Enter(this(),pByObj));
}

/* TimerCall */

protected func Activity() { // (komponenten ge�ndert)
  // Es ist wohl zu warm
  if (GetTemperature()>10) Melt();
  // Zeit als minimale Baustelle berechnen
  if (!(GetComponent(SWBL)||GetComponent(WOOD)||GetComponent(LTHR)))
    Local(9)++;
  // Baustelle errichtet, aber nicht weitergebaut: abbrechen
  if (Local(9)>35) RemoveObject();  
  CheckBuild();
  return(1);
}

/* W�rme */

public func GetWarmth() {
  // Gro�es Iglu ist etwas w�rmer
  return(60);
}

/* Konstruktion */

private func MenuProduction(pCaller) {
  // Men� erzeugen und mit Baupl�nen des Spielers f�llen
  CreateMenu(IGL2,pCaller,this(),1,"$TxtNoconstructionplans$");
  for(var i=0,idKnowledge;idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i,C4D_Vehicle());++i)
    AddMenuItem( "$TxtProductions$", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  return(1);
}

private func Building()
{
  if (!Random(2)) Sound("Build*");
}