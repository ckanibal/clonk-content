/* Keine Aufwertung f�r den K�nig */

#strict

#appendto RVLT

private func NoRevaluation(t, t1) { return(_inherited(t, t1)); }

public func Revaluation(target, target1)
  {
  if (GetID(target) == KING) return(NoRevaluation(target, target1));
  return(_inherited(target, target1));
  }
