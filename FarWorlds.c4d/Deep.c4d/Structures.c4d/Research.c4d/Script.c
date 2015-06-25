/*-- Tiefsee-Forschungslabor --*/

#strict
#include RSRC

/* ~#exclude des fundaments */

protected func Construction() {}
protected func Destruction() {}

protected func ContainedDown()
  {
  [$TxtCancelresearch$|Image=RSR2]
  CancelResearch();
  return(0);
  }
  
/* �berladen um alles in einem Men� zu haben und nur Objekte mit IsDeepSeaResearch erforschen zu k�nnen */
  
public func ResearchMenu(pClonk)
{
  // Men� erzeugen
  CreateMenu(RSR1, pClonk, this());
  if (target) AddMenuItem("$TxtContinuedevelopement$", "SelectResearch", target, pClonk, 0, pClonk);
  ResearchObjects(pClonk);
  ResearchVehicles(pClonk);
  ResearchStructures(pClonk);  
}

protected func ResearchObjects(pClonk)    { return(SelectResearchCategory(pClonk, C4D_Object())); }
protected func ResearchVehicles(pClonk)   { return(SelectResearchCategory(pClonk, C4D_Vehicle())); }
protected func ResearchStructures(pClonk) { return(SelectResearchCategory(pClonk, C4D_Structure())); }
  
protected func SelectResearchCategory(pClonk, dwCategory)
  {
  var i, idDef, iPlr = GetOwner(pClonk), link;
  var fResearchAll = 0; // �ber dieses Flag k�nnte sp�ter mal _alles_ erforschbar gemacht werden, sobald ein Spieler eine Superkuppel fertiggestellt hat...
  // Alle geladenen Baupl�ne �berpr�fen
  while (idDef = GetDefinition(i++, dwCategory))
    // Passende Kategorie
    if (GetCategory(0, idDef) & C4D_Knowledge())
      // Dem Spieler noch nicht bekannt
      if (!GetPlrKnowledge(iPlr,idDef))
        // In der Tiefsee erforschbar (hardgecodet f�r einige Originalobjekte, bis diese im Originalpaket erneuert werden)
        if (fResearchAll || DefinitionCall(idDef, "IsDeepSeaResearch") || (idDef == LORY) || (idDef == SUB1))
        {
          // Keine Forschungsgrundlage n�tig oder Forschungsgrundlage vorhanden
          var idBase = DefinitionCall(idDef,"GetResearchBase"), fHasBase, j;
          fHasBase = !idBase || GetPlrKnowledge(iPlr,idBase); j=1;
          // Erweiterte Forschungsgrundlagen
          while (idBase = DefinitionCall(idDef, Format("GetResearchBase%d", ++j)))
            if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=0;
          // Ist schon durch etwas anderes erforschbar? (Zugbr�cke rechts -> Zugbr�cke links etc.)
          if (link = DefinitionCall(idDef, "GetResearchLink"))
            if (GetName(0, link)) // Linkdefinition auch tats�chlich geladen?
              continue;
          // Men�eintrag hinzuf�gen
          if(fHasBase) AddMenuItem( "$TxtResearchs$", "SelectResearch", idDef, pClonk, 0, pClonk );
        }
  return(1); 
  }
  
/* �berladungen */

private func Smoke()
{
  // Raucheffekte des RSRC verhindern...
}  
  
/* Callbacks */

public func IsDeepSeaResearch() { return(1); }

public func GetResearchBase() { return(RSU1); }
