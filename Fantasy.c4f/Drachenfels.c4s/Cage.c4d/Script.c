/*-- K�fig --*/

#strict

/* �ffnung */

protected func ControlThrow(object pClonk)
  { [$CtrlOpen$|Image=_KEY]
  // Schon offen?
  if (GetAction() eq "Open") return();
  // Schl�ssel dabei?
  var pKey = FindContents(_KEY, pClonk);
  if (!pKey) return(Message("$MsgNoKey$", pClonk, GetName(pClonk)), Sound("CommandFailure*"));
  // Aufschlie�en
  Sound("Connect", this());
  Sound("ChestOpening", this());
  SetAction("Open");
  SetSolidMask();
  GameCall("OnCageOpened", this());
  }
