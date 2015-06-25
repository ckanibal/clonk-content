/*-- Replikatzauber --*/

#strict

func Activate(pCaller, pTrueCaller)
{
  // Zaubernden Clonk ermitteln
  if(pTrueCaller) pCaller=pTrueCaller;

  if(Contained(pCaller)) { PlayerMessage(GetOwner(pCaller), "$NotInFree$", pCaller); return(0); }  

  // Ben�tigte Variablen:
  var iEnergy, pReplication, pWipf;

  if(pWipf=FindContents(WIPF, pCaller))  // Wipf dabei? Superkombo!
  {
    // Energie bestimmen
    iEnergy=GetEnergy(pCaller)/10;
    for(var i=6; i>0; --i)
    {
      pReplication=CreateObject(GetID(pCaller), AbsX(GetX(pCaller)), AbsY(GetY(pCaller))+9, GetOwner(pCaller));
      // Den gleichen Namen geben
      SetName(GetName(pCaller), pReplication);
      // Kopie mit Effekt zur Steuerung ausstatten
      AddEffect("ReplicationSpell", pReplication, 200, 1, 0, GetID(), 1750+RandomX(-100,100));
      // Energie zuweisen
      DoEnergy(iEnergy-GetEnergy(pReplication), pReplication);
      // Kampfphysicals runterdrehen
      SetPhysical("Fight", GetPhysical("Fight", 0, pCaller)/3, 2, pReplication);
      // Ein bisschen Platz muss sein
      Fling(pReplication,RandomX(-5,5), RandomX(-2,-1));
    }
    Fling(pCaller,RandomX(-5,5), RandomX(-2,-1));
    CastParticles("MSpark", 15, 25, AbsX(GetX(pCaller)), AbsY(GetY(pCaller)), 50, 75, GetColorDw(pCaller), GetColorDw(pCaller));
    RemoveObject(pWipf);
  }

  else  // Normal - Hier sollte nicht so sehr auffallen, dass es eine Kopie ist.
  {
    // Energie bestimmen
    var iEnergy=GetEnergy(pCaller)/3;

    // Zauberer Anhalten
    SetComDir(COMD_Stop(), pCaller);

    // Kopie erstellen
    var pReplication=CreateObject(GetID(pCaller), AbsX(GetX(pCaller)), AbsY(GetY(pCaller))+10, GetOwner(pCaller));

    // Richtung anpassen
    SetDir(GetDir(pCaller), pReplication);

    // Geschwindigkeit anpassen
    SetXDir(GetXDir(pCaller, 1000), pReplication, 1000);
    SetYDir(GetYDir(pCaller, 1000), pReplication, 1000);

    // Aktion anpassen
    if(!pReplication->SetAction(GetAction(pCaller)) )
/*    pReplication->SetAction("Walk");
    else*/
      pReplication->SetPhase(GetPhase(pCaller));
 
    // Energie zuweisen
    DoEnergy(iEnergy-GetEnergy(pReplication), pReplication);

    // Physical runtersetzen
    SetPhysical("Fight", GetPhysical("Fight", 0, pCaller)*3/4, 2, pReplication);

    // Interner Effekt zum gucken ob der Zauberer schon eine Replikation hat
    var iIndex=AddEffect("IntHasCopy", pCaller, 1);
    EffectVar(0, pCaller, iIndex)=pReplication;

    // Effect f�r Steuerung der Kopie zuweisen
    AddEffect("ReplicationSpell", pReplication, 200, 1, 0, GetID(), 2000+RandomX(-100,100));

    // Den gleichen Namen geben
    SetName(GetName(pCaller), pReplication);
  }

  // Zauberobjekt ist fertig.
  RemoveObject();
  return(1);
}

func FxReplicationSpellStart(pTarget, iEffectNumber, iTemp, iTime)
{
  if(iTemp) return();
  EffectVar(0, pTarget, iEffectNumber)=iTime;
}

func FxReplicationSpellTimer(pTarget, iEffectNumber, iEffectTime)
{
  // Die Action kann kaputtgehen, wenn der Magier eine Ride-Action oder sowas
  // hatte und eine Kopie castete, die die urspruengliche Action mangels
  // ActionTarget nicht uebernehmen konnte. Dummerweise merkt die Engine das
  // beim SetAction-Aufruf noch nicht...
  if(pTarget->GetAction() S= "Idle") pTarget->SetAction("Walk");
  // Zeit abgelaufen?
  if(iEffectTime>=EffectVar(0, pTarget, iEffectNumber))
    // Beenden
    return(-1);
  // Am Anfang erstmal nichts tun:
  if(iEffectTime<=50) return(0);
  // Habe schon was zu tun: Beenden
  if(GetCommand(pTarget) && Random(10)) return();
  // Hochintellente Ki
  var pObj;
  // Gegnersuche
  while(pObj=FindObject(0, GetX(pTarget), GetY(pTarget), -1, -1, OCF_CrewMember(), 0, 0, NoContainer(), pObj))
  {
    if(ObjectDistance(pTarget, pObj) > 1500) continue;
    if(!Hostile(GetOwner(pObj), GetOwner(pTarget))) continue;
    var pEnemy=pObj;
    break;
  }
  //Attacke
  if(pEnemy) SetCommand(pTarget, "Attack", pEnemy);
}

func FxReplicationSpellStop(pTarget, iEffectNumber, iReason, fTemp)
{
  // Tempor�re Aufrufe: Nichts tun
  if(fTemp) return(0);
  // Funken und Rauch verwirren, wenn pl�tzlich einen Clonk ersetzend.
  Sound("Poff", 0, pTarget, 100);
  CastParticles("PSpark", 10, 8, GetX(pTarget), GetY(pTarget), 20, 180, GetColorDw(pTarget), GetColorDw(pTarget));
  CastParticles("FSpark", 3, 50, GetX(pTarget), GetY(pTarget), 20, 80);
  CastParticles("Smoke", 3, 50, GetX(pTarget), GetY(pTarget), 20, 180);
  // Kopie noch l�schen - fertig.
  return(RemoveObject(pTarget, 1));
}

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("45"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
