/*-- Metall --*/

#strict
#appendto METL

// Man soll nicht mit unzerst�rbarem Granit bauen k�nnen
public func BridgeMaterial() {
  return(Material("Rock"));
}
