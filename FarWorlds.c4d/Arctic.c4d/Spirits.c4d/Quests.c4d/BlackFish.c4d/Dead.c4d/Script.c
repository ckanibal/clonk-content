/*-- Toter Fisch --*/

#strict
#include DFSH

/* Aktionen von au�en */  
  
public func Eat(pClonk) {
  pClonk->~Feed(-30);
  RemoveObject();
  return(1);
}