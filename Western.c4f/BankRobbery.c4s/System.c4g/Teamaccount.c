/*-- Konto --*/

#strict
#appendto ACNT

/* Initialisierung */

protected func Initialize()
{
  // Anfangsgeldwert
  value=GetWealth(GetOwner());
  if(Local(CheckTeam(GetOwner()), FindObject(TACC))) value = Local(CheckTeam(GetOwner()), FindObject(TACC));
  else Local(CheckTeam(GetOwner()), FindObject(TACC)) = value;
  SetWealth(GetOwner(), value);
}

/* �berpr�fung */

private func Execute()
{
  // Geh�rt niemandem (Spieler eliminiert?) - l�schen
  if (GetOwner() < 0) return(RemoveObject() );

  // Geldwert�nderung?
  if (GetWealth(GetOwner()) != value)
  {
    // �nderung berechnen und anrechenen
    var d=GetWealth(GetOwner())-value;
    value+=d;
    // �nderung auf verb�ndete Konten anrechnen
    Local(CheckTeam(GetOwner()), FindObject(TACC)) += d;
    var obj;
    while(obj = FindObject(ACNT, 0, 0, 0, 0, 0, 0, 0,0, obj)) if(obj!=this()) if(CheckTeam(GetOwner(obj))==CheckTeam(GetOwner())) obj->UpdateValue(d);
  }
}

/* Team�nderung */

public func TeamChanged()
{
  Initialize();    	
}
