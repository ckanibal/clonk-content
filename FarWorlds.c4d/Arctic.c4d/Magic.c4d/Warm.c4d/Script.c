/* W�rmen */

#strict

public func Activate(oCaller, oClonk)
{                            
  if(!oClonk) oClonk = oCaller;
  // Effekt pr�fen
  var iChkEff;
  if (iChkEff = CheckEffect("WarmPSpell", oExtiObj, 180)) return(iChkEff!=-1 && RemoveObject());
  // N�chstes gerforenes Objekt finden
  var oExtiObj, iExtiCount, pObj;
  // Den Clonk selbst?
  if(Frozen(oClonk)) 
    iExtiCount += DoUnfreeze(oClonk, oCaller);
  else if(oClonk->~MagicWarm(1800))
    iExtiCount += DoUnfreeze(oClonk, oCaller);
  else if(pObj = Contents(0,oClonk))
    if(GetID(pObj)==METL) if(GetDefHeight(ANML))
    {
      RemoveObject(pObj);
      CreateContents(ANML, oClonk);
      return(RemoveObject());
    }
  // N�chstliegende Objekte
  SetPosition(GetX(oCaller), GetY(oCaller));
  // Suchradius ist abh�ngig von H�he des Aufrufenden - ein Zauberturm hat einen gr��eren Radius
  var iSearchRadius = GetObjHeight(oCaller) * 3;
  while(oExtiObj = FindObject(0, 0, 0, -1, -1, 0, 0, 0, 0, oExtiObj))
    if(ObjectDistance(oCaller, oExtiObj) > iSearchRadius)
      break;
    else if(Frozen(oExtiObj))
      iExtiCount += DoUnfreeze(oExtiObj, oCaller);
    else if (oExtiObj->~MagicUnfreeze(oCaller))
      iExtiCount += DoUnfreeze(oExtiObj, oCaller);
  // Irgendwas erw�rmt?
  if (!iExtiCount)
  {
    Message("$NoUnfreeze$", oClonk);
    RemoveObject();
    return 0;
  }
  // Einmaliger Effekt pro Zauber
  Sound("Inflame");
  return(true);
}

private func DoUnfreeze(object oExtiObj, object oCaller)
  {
  // Nicht wenn gegnerischer CrewMember
  if (Hostile(GetOwner(oExtiObj), GetOwner(oCaller)))
    if (GetOCF(oExtiObj) & OCF_CrewMember())
      if (!ObjectCount(NTMG)) return(0);
  // Erw�rme es
  Unfreeze(oExtiObj);
  // Eine paar Effekte...
  CreateParticle("PxSpark",RandomX(-5,5),RandomX(-5,5),0,0,RandomX(100,200),RGB(255,128,0));
  // 1 Objekt erw�rmt
  return(1);
  }
  
// Sichterheit, wenn das Fantasypack nicht aktiv ist
global func Frozen() { return _inherited(...); }
global func Unfreeze() { return _inherited(...); }

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("666"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
