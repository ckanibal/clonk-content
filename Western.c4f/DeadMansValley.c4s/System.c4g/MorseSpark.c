/*-- F�r alle seh und h�rbar --*/

#strict
#appendto TGTS

protected func Initialize()
{
  SetVisibility(VIS_All());
  SetPlrView(GetOwner(), this());
  SetAction("Fly");
  Sound("Morse", 0,this(),100,0, 1);
}
