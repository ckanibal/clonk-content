/*-- Verbrannter Totempfahl --*/

#strict 2

protected func Destruction()
{
  // Der Clan bekommt den jeweiligen Bauplan zur�ck
  if(!TTPL->~RejectResearch(this(),-1)) SetClanKnowledge(GetClan(),GetDefConstructTo(GetID()),0);
}