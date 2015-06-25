/*-- Altes Zeug, das nicht mehr in die Engine muss --*/

#strict

// Abgel�st durch SetPosition
global func ForcePosition(object obj, int x, int y) { return(SetPosition(x, y, obj)); }

// Abgel�st durch RemoveObject
global func AssignRemoval(object obj) { return(RemoveObject(obj)); }

// F�r Szenarien ohne Objects.c4d...
global func EmptyBarrelID() { return(BARL); }

// F�gt das Material in ein Fass im Objekt ein
global func ObjectInsertMaterial(int imat, object pTarget)
{
  if (!pTarget || imat == -1) return(); // Kein Zielobjekt / Material?
  // Fasstyp ermitteln
  var idBarl;
  if (idBarl = GetBarrelType(imat))
  {
    // Fass suchen
    var pBarl = FindFillBarrel(pTarget, idBarl);
    if (pBarl)
      // Fass auff�llen
      return(pBarl->BarrelDoFill(1, imat+1));
  }
  // Kein Fass? Dann Objekt �berlaufen lassen
  return(InsertMaterial(imat, GetX(pTarget)-GetX(), GetY(pTarget)-GetY()));
}
  
// Auff�llbares Fass im Objekt suchen
global func FindFillBarrel(object pInObj, id type)
{
  // Alle Inhaltsobjekte durchlaufen
  var pObj;
  for(var i = 0; pObj = Contents(i, pInObj); i++)
    // ID stimmt?
    if(pObj->GetID() == type)
      // Fass nicht voll?
      if (!pObj->~BarrelIsFull())
        // Nehmen wir doch das
        return(pObj);
  // Nix? Dann halt ein leeres Fass suchen und f�llen
  if (!(pObj=FindContents(EmptyBarrelID(), pInObj))) return();
  ChangeDef(type, pObj);
  return(pObj);
}

// Fl�ssigkeit aus F�ssern im Objekt extrahieren
global func ObjectExtractLiquid(object pFrom)
{
  // Alle Inhaltsobjekte durchlaufen
  var pObj;
  for(var i = 0; pObj = Contents(i, pFrom); i++)
  {
    // Extrahieren
    var iRet = pObj->~BarrelExtractLiquid();
    if(iRet != -1) return(iRet);
  }
  // Extrahieren nicht m�glich
  return(-1);
}
  
global func ShowNeededMaterial(object pOfObject)
{
  MessageWindow(GetNeededMatStr(pOfObject), GetOwner(),CXCN,GetName(pOfObject));
  return(1);
}

global func SetOnlyVisibleToOwner(bool fVisible, object pObj)
{
  var oldVal=GetOnlyVisibleToOwner(pObj);
  if (fVisible) 
    SetVisibility(VIS_Owner() | VIS_God(), pObj);
  else
    SetVisibility(VIS_All(), pObj);
  return(oldVal);
}
  
global func GetOnlyVisibleToOwner(object pObj)
{
  return (GetVisibility(pObj) == VIS_Owner() | VIS_God());
}

global func MessageBoard(string msg, par0, par1, par2, par3, par4, par5, par6, par7, par8)
{
  return(Log(msg, par0, par1, par2, par3, par4, par5, par6, par7, par8));
}

// Fasskonfiguration
// Kann z.B. durch eine Spielregel �berladen werden (Shamino)
// Bit 0 (1): Wasserf�sser sind auch im Verkauf 8 Clunker wert
// Bit 1 (2): F�sser werden beim Verkaufen nicht entleert (sind wieder voll kaufbar)
// Bit 2 (4): Nur Wasserf�sser werden beim Verkaufen nicht entleert (sind wieder voll kaufbar)
global func BarrelConfiguration() { return(5); }
