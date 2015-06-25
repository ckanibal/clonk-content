/*-- Totes Pferd --*/

#strict

local iGraphics;

/* Verwesung */

private func Decaying()
{
  DoCon(-4);
}

/* Zerlegen */

public func ControlDigDouble(pClonk) {
  [$TxtEmbowel$|Image=KNFE|Condition=GetAlive]
  // Der Clonk soll dieses Objekt zerlegen, wenn er kann
  return(ObjectSetAction(pClonk,"Embowel",this()));
}

/* Wird zerlegt */

public func Embowel(object pByClonk) {
  var pObj, iID, i, iCount, pContainer;
  pContainer = Contained();
  for(i = 0; iID = GetComponent(0,i,this()); i++) {
    if(!FindDefinition(iID)) continue;
    for(iCount = GetComponent(iID,0,this()); iCount > 0; iCount--) {
      pObj = CreateObject(iID,0,10,GetOwner());
      if(iID == SKIN)
        pObj->~SetSkinGraphics(Format("%d", LocalN("iGraphics",this())));
      if(iID == BBON)
        SetName("$NameHorseBone$", pObj);
      if(pContainer) { Enter(pContainer, pObj); continue; }
      SetXDir(RandomX(-1,1)*10, pObj);
      SetYDir(RandomX(-1,1)*10, pObj);
      SetRDir(RandomX(-1,1)*10, pObj);
      SetR(Random(360), pObj);
    }
  }
  RemoveObject();
  return(1);
}