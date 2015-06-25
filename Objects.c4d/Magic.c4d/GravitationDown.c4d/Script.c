/*-- Gravitation runter --*/

#strict

public func Activate(pCaster, pCaster2)
  {
  // G�ltigkeit pr�fen
  if(GetGravity()<=30) { Message("$MsgGravityMinimum$",pCaster); return(0, RemoveObject()); }
  // Magie kann man h�ren, ganz klar ;)
  Sound("Magic*");
  // Effekt global starten
  if (!AddEffect("GravChangeUSpell", 0, 150, 80, 0, GetID(), -20))
    return(0, RemoveObject());
  // Gravitation anzeigen
  Message("$MsgGravityDec$",pCaster,GetGravity());
  // Fertig - das Zauberobjekt wird nun nicht mehr gebraucht
  return(RemoveObject());
  }


/*-- Gravitationseffekt --*/
// Wird auch vom anderen Gravitationszauber verwendet

// EffectVars: 0 - Vorherige Gravitation
//             1 - �nderung durch den Zauber

protected func FxGravChangeUSpellStart(pTarget, iEffectNumber, iTemp, iChange)
  {
  // Effektstart
  if (!iTemp) EffectCall(pTarget, iEffectNumber, "DoSparks", iChange);
  // Vorherige Gravitation sichern
  var iOldGrav = EffectVar(0, pTarget, iEffectNumber) = GetGravity();
  // F�r nichttempor�re Aufrufe wird iChange �bergeben, und auf den �nderungswert aufgerechnet
  if (iChange) EffectVar(1, pTarget, iEffectNumber) += iChange;
  // Gravitations�nderung setzen
  // Die �nderung kann in tempor�ren Aufrufen auch ungleich iChange sein
  SetGravity(iOldGrav + EffectVar(1, pTarget, iEffectNumber));
  // Fertig
  return(1);
  }

protected func FxGravChangeUSpellTimer(pTarget, iEffectNumber)
  {
  // Gravitation in Richtung Normalwert schrauben
  var iGravChange = EffectVar(1, pTarget, iEffectNumber);
  // Fertig?
  if (Inside(iGravChange, -1, 1)) return(-1);
  // Anpassen
  var iDir = -iGravChange/Abs(iGravChange);
  if (Abs(iGravChange) > 40) iDir*=2;
  EffectVar(1, pTarget, iEffectNumber) += iDir;
  SetGravity(GetGravity() + iDir);
  return(1);
  }

protected func FxGravChangeUSpellStop(pTarget, iEffectNumber)
  {
  // Gravitation Wiederherstellen
  SetGravity(EffectVar(0, pTarget, iEffectNumber));
  // Effekt entfernen
  return(1);
  }

protected func FxGravChangeUSpellEffect(szNewEffect, pTarget, iEffectNumber)
  {
  // Falls der neu hinzugef�gte Effekt auch eine Gravitations�nderung ist, Interesse am �bernehmen anmelden
  if (SEqual(szNewEffect, "GravChangeUSpell")) return (-3);
  // Ansonsten ignorieren
  return();
  }

protected func FxGravChangeUSpellAdd(pTarget, iEffectNumber, szNewEffect, pTarget, iNewTimer, iChange)
  {
  // Aufruf erfolgt, wenn der Effekt �bernommen werden konnte
  // Gravitations�nderung auf diesen Effekt aufrechnen
  EffectVar(1, pTarget, iEffectNumber) += iChange;
  SetGravity(GetGravity() + iChange);  
  // Grafikeffekt
  EffectCall(pTarget, iEffectNumber, "DoSparks", iChange);
  // Fertig
  return(1);
  }

protected func FxGravChangeUSpellDoSparks(pTarget, iEffectNumber, iMovement)
  {
  // Graphischer Effekt
  var lwdt=LandscapeWidth(),lhgt=LandscapeHeight(),x=GetX(),y=GetY();
  var i=LandscapeWidth()*LandscapeHeight()/10000;
  while (i--) CreateParticle("MSpark", Random(lwdt)-x,Random(lhgt)-y, 0,iMovement*2, 30+Random(50), RGBa(255-Random(50),255-Random(50),0,128));
  }

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("222"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
