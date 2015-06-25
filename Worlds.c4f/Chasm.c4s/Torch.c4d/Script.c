/*--- Fackel ---*/

#strict

public func Activate()
{
  var gbaeude, obj;
  //Nich im Himmel (au�er an Geb�uden)
  if(GetMaterial()!=Material("Tunnel") && !(gbaeude=FindObject(0,1,0,0,0,OCF_Exclusive())))
      return(Sound("CommandFailure1.wav"),Message("Anbringen hier nicht m�glich",this()));
  //Nich im Entrancebereich von Geb�uden
  if(GetOCF(gbaeude)&OCF_Entrance()) if(InEntrance(gbaeude))
      return(Sound("CommandFailure1.wav"),Message("Anbringen hier nicht m�glich",this()));
  //Nich in Fl�ssigkeiten
  if(InLiquid(Par()))
      return(Sound("CommandFailure1.wav"),Message("Anbringen hier nicht m�glich",this()));
  Sound("Connect");
  ObjectSetAction(obj=CreateObject(TOR2,0,12,GetOwner()),"Burning",gbaeude);
  SetDir(Random(2)); if(gbaeude) SetDir(GetX(obj)>GetX(gbaeude),obj);
  RemoveObject();
  return(1);
}

public func IsAnvilProduct()
{
  return(1);
}

private func InEntrance(gbaeude) 
{
  if(Inside(GetX(),
    GetX(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),0),
    GetX(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),2)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),0) ))
      if(Inside(GetY(),
        GetY(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),1),
        GetY(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),3)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),1) ))
          return(1);
}
