/*-- Neues Script --*/

#strict

#appendto XARW

//bisle heruntergedreht, da nun �fter zu haben
protected func Hit()
{
  if (Armed)
    Explode(Random(4) + 6);
}
