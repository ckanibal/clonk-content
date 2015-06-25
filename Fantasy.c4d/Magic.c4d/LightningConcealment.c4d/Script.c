/*-- Verblendung --*/

#strict

protected func Activate(object pCaster, object pRealCaster)
{
  // Zaubergeb�ude: Ein wenig st�rker
  var iLevel;
  if(GetCategory(pCaster) & C4D_Structure())
    iLevel=160;
  else
    iLevel=120;
  AddEffect("LightningConcealment", this(), 300, 1, 0, GetID(), iLevel);
  return(1);
}

func FxLightningConcealmentStart(pTarget, iEffectNumber, iTemp, var1)
{
  if(iTemp) return(0);
  EffectVar(0, pTarget, iEffectNumber)=var1;
  return(1);
}

func FxLightningConcealmentTimer(pTarget, iEffectNumber, iEffectTime)
{
  // St�rke abfragen
  var iLevel=EffectVar(0, pTarget, iEffectNumber);
  if(iLevel<=0)
  {
    RemoveObject(pTarget);
    return(-1);
  }
  for(var i=1; i>0; --i)
  {
  // Funken bauen
  var iX, iY, iAngle, iRad;
  // Zuf�llige Position innerhalb des Radius finden
  iAngle=Random(360);
  iRad=Random(iLevel);
  iX=GetX(pTarget)+Sin(iAngle, iRad);
  iY=GetY(pTarget)+Cos(iAngle, iRad);
  CreateParticle("Flash", iX, iY, 0, 0, RandomX(iLevel*6, iLevel*10), RGBa(255,255,255,Random(120)));
  }
  // St�rke langsam runtersetzen
  if(iEffectTime > iLevel*6)
    --EffectVar(0, pTarget, iEffectNumber);
  return(1);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("644"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
