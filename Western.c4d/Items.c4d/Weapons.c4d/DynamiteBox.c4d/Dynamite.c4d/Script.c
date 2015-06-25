/*-- Dynamit --*/

#strict 2

/* Aktivierung */

public func Activate(object pCaller)
{
  [$TxtFuse$]
  if(GetMaterialVal("Extinguisher","Material",GetMaterial(0,0))) return(0);
  if(IsFused()) return(0);
  Sound("Fuse");
  SetAction("Fuse");
  // Aktivator wird Besitzer
  SetOwner(GetOwner(pCaller));
  return(1);
}

/* Aktivierung (durch Inhaltssteuerung) */

public func IsFused() { return(GetAction() == "Fuse"); }

public func ControlDigDouble() { return(Activate()); }

// Z�ndschnur-Effekte
private func Effects()
{
  if(GetActTime() >= 100) return(Explode(18));
  if(GetMaterialVal("Extinguisher","Material",GetMaterial(iX,iY))) return(DoExtinguish());

  if(!Contained()) {
    var iX, iY, iAmount;
    iX =  Sin(GetR(), 5);
    iY = -Cos(GetR(), 5);
  }
  else {
    var pObj = this();
    while(pObj = Contained(pObj)) {
      if(GetCategory(pObj) & (C4D_Structure|C4D_Vehicle))
        return(1);
    }
  }
  if(!Random(4))
    for(iAmount = RandomX(1,3); iAmount > 0; iAmount--)
      CreateParticle("DynamiteSpark",iX,iY,iX*3+RandomX(-10,10),iY*3+RandomX(-10,10),20,RGBa(255,255,0,100));
}

// Aktivierung durch Explosion von au�en
protected func Incineration () { return(Explode(14)); }

// Bei Aufprall
protected func Hit() { Sound("WoodHit*"); }

public func MagicExtinguish() {
  // kann durch den Zauber "L�schen" gel�scht werden
  if(GetAction() != "Fuse") return(0);
  DoExtinguish();
  return(1);
}

private func DoExtinguish() {
  Sound("Pshshsh");
  SetAction("Idle");
  return(1);
}

/* Packeigenschaften */

public func PackTo() { return(DYNB); }

public func Entrance(object pContainer)
{
  // Ist dies ein Waffenlager?
  if(pContainer->~ProductCondition() != "IsArmoryProduct") return(0);

  // Nicht wenn unterdr�ckt
  if(IsFused()) return(0);

  // Zu einem Pack dazuf�gen
  return(Pack(pContainer));
}

public func Pack(object pContainer)
{
  // Ist eine freie Dynamitkiste verf�gbar?
  var pObj;
  if(pObj = FindObject2(Find_ID(PackTo()),Find_Container(pContainer),Find_Func("IsNotFull"),Find_Exclude(this()))) {
    // In die Kiste rein damit
    pObj->~DoPackCount(1);
    return(RemoveObject(this()));
  }
  // Keine Kiste vorhanden, selber eine erstellen
  if(pObj = CreateContents(DYNB,pContainer)) {
    pObj->~SetPackCount(1);
    return(RemoveObject(this()));
  }
  // Packen nicht m�glich
  return(0);
}