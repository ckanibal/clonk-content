#strict

protected func Construction() {			// init
  // zuf�llige Grafik
  SetAction("Exist");
  SetDir(Random(4));
}

public func Place(object pTree, int iGrowth) {	// neu entstanden
  // kleben
  SetCategory(1);
  SetActionTargets(pTree);

  // Gr��e
  if(!iGrowth) iGrowth=Random(100)+1;
  SetCon(iGrowth);
}

public func Drop() {				// wird gepfl�ckt (~Gegenteil von Place)
  // wenn nicht Vollst�ndig: n�, l�schen
  if(GetCon()<100) return(RemoveObject());

  SetActionTargets();
  SetCategory(16);
}

protected func RejectEntrance() {		// nicht pfl�cken wenn nicht reif
  if(GetCon()<100) return(1);
}

protected func Entrance() {			// gepfl�ckt
  if(GetCategory()==1) Sound("Grab");
  Drop();
}


public func Activate(object pClonk)		// essen
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

protected func Eat(object pClonk)
{
    pClonk->~Feed(30);
    pClonk->DoBreath(100);
    RemoveObject();
    return(1);
}

public func GetFeedValue() { return(30); }

protected func Hit() { Sound("WoodHit*"); }
