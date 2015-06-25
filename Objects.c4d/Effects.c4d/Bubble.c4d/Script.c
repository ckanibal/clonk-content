/* Luftblase */

#strict

protected func Shiver()
  {
  // Bewegung
  SetXDir(Random(21) - 10);
  return(1);
	}

protected func Initialize()
  {
  // Aktion setzen
  SetAction("Bubble");
  SetComDir(COMD_Up());
  // Nicht in Fl�ssigkeit gestartet: gleich weg
  if (!GBackLiquid(0,0)) 
    Remove();
  return(1);
  }

protected func LiquidCheck()
  {
  if(!InLiquid())
    Remove();
  }
protected func Remove()
  {
  RemoveObject();
  return(1);
  }
