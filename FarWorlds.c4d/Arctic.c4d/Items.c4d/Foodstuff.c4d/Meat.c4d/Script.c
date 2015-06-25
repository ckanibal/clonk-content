/*-- Fleisch --*/

#strict

protected func Hit() { Sound("WoodHit*"); }

public func Eat(pEater) {
  if(pEater) pEater->~Feed(80);
  RemoveObject();
}

public func Activate(pClonk) {
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

/* K�derqualit�t */  
  
public func IsBait() { return(70); }

/* Action-Call */

private func Baiting() {
  // Wurde eingesammelt
  if (Contained())
    return(SetAction("Idle"));
  // Clonk-Aktion �berpr�fen
  if (GetAction(GetActionTarget())ne"Fish")
    return(SetAction("Idle"));
  // Sollte hier noch die Position des Clonks
  // �berpr�fen, falls dieser verrutscht ist.
}
