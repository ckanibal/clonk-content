/*-- Gruselschloss --*/

#strict

protected func Initialize()
{
  // Schloss platzieren
  CreateContructionNear(CST3, 680, 200, NO_OWNER);
  // Dorf platzieren
  CreateContructionNear(HUT3, 250, 670, NO_OWNER);
  CreateContructionNear(HUT2, 250, 670, NO_OWNER);
  CreateContructionNear(HUT2, 250, 670, NO_OWNER);
  CreateContructionNear(WMIL, 270, 620, NO_OWNER);
}

func InitializePlayer (plr,x,y,bas,team) {
  // Geb�ude in den Besitz
  for (var obj in FindObjects(Find_Category(C4D_Structure), Find_Owner(NO_OWNER), Find_Distance(300, x,y)))
    SetOwner(plr, obj);
  // H�rx ist ein K�nig!
  if (team == 2) {
    var obj = GetHiRank (plr);
    obj->CLNK::Redefine2 (_HRX);
    SetPortrait ("random", obj, _HRX);
    SetName ("$Herx$", obj);
  }
  // Alle ins Haus!
  var home;
  if (team == 2) home = FindObject2(Find_ID(CST3)); else home = FindObject2(Find_ID(HUT3));
  for (var mat in FindObjects(Find_Category(C4D_Object | C4D_Vehicle | C4D_Living), Find_Not(Find_ID(CSTE)), Find_Owner(plr)))
    Enter(home, mat);
  // Den besten Clonks ausw�hlen und vor die T�r
  SetCursor(plr, GetHiRank(plr));
  SetCommand(GetCursor(plr), "Exit");
  // Und auf den Gegner schauen
  SetDir(2-team, GetCursor(plr));
}


/* Bauplatz suchen und Geb�ude platzieren */

func CreateContructionNear(id def, int x, int y, int owner)
{
  Var(0)=x; Var(1)=y;
  if (!FindConstructionSite(def, 0,1)) return 0;
  x=Var(0); y=Var(1);
  return CreateConstruction(def, x,y, owner, 100, true);
}