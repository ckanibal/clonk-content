#strict

#appendto DRST

protected func ContainedDig(object pClonk)
{
  [$SellMenu$]
  Sound("Error");
  PlayerMessage(GetOwner(pClonk),"$MsgNoSell$",this());
}

// Geldgesch�fte sollen nicht durch Vandalismus gest�rt werden
global func GetBank(object pMessage, bool silent)
{
  return(1);
}
