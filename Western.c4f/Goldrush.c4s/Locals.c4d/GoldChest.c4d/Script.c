/*-- Gold-Truhe --*/

#strict

#include CHST

public func Empty()
{
  // Gold verschwindet, der Rest wird ausgesch�ttet	
  while(FindContents(GOLD)) RemoveObject(FindContents(GOLD));
  while(Contents()) Exit(Contents());
}
