/*-- Schie�pulver --*/

#strict 2

#appendto GUNP

protected func Damage()
{
  // Explodiert nicht, wenn er von einem Clonk getragen wird (Sonderregel gegen T-Flint-Superjumps)
  if(Contained())
    if(Contained()->~IsClonk()) {
      Extinguish();
      return(0);
    }
  // Explosion - verz�gert, um Endlosrekursion zu verhindern
  // H�ngt von der Gr��e ab
  if(GetCon() < 4) return(RemoveObject(this()));
  Schedule(Format("Explode(%d)",GetCon()*3/10),1);
  return(1);
}