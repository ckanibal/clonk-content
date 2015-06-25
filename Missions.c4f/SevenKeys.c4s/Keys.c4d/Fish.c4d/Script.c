
#strict

#include FISH
#include _KEY

public func Init(object oWhere)
{
	if(SearchPositionInMatEx(Material("Water"), true)) return(1);
	Message("$TxtPlacementFailed$", oWhere);
}

public func Announce(object oWhere)
{
	Message("$TxtAnnounce$", oWhere);
}

public func Timer()
{
	// Au�erhalb des Wassers? Tot
	if(GetMaterial() != Material("Water"))
	{
		// Sicherheitschecks - der Fisch taucht �fters mal kurzfristig auf (oder klettert aufs Land).
		// Das sollte er besser lassen.
		if(GetMaterial(-2, -2) == Material("Water")) SetPosition(GetX()-2, GetY()-2);
		else if(GetMaterial(2, -2) == Material("Water")) SetPosition(GetX()+2, GetY()-2);
		else if(GetMaterial(-2, 2) == Material("Water")) SetPosition(GetX()-2, GetY()+2);
		else if(GetMaterial(2, 2) == Material("Water")) SetPosition(GetX()+2, GetY()+2);
		else Death();
	}
	// Normale Fischaktivit�t
	if(!(FrameCounter() % 15))
		Activity();
	// Normale Schl�sselaktivit�t
	return(inherited());
}

// Nicht reproduzieren
func Reproduction() { }

public func GetKeyDesc() { return "$TxtAnnounce$"; }

