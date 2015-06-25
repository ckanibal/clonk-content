#strict

/* Checkt ab ob genug magische Energie f�r den Zauber vorhanden ist */

static const MCLK_ManaTrainRate = 10; // Zauber geben Wert/x Magiephysicaltraining
static const MCLK_UnlimitedCast = 100; // Endloszauberei

global func CheckMana(idMagic,pCaster)
  {
  var iMagicReq = Value(idMagic);
  // Keine Zauberenergie ben�tigt: Kann immer zaubern
  if (!iMagicReq || ObjectCount(NMGE)) return(MCLK_UnlimitedCast);
  // Ansonsten zur�ckgeben, wie oft gezaubert werden kann
  return (GetMagicEnergy(pCaster) / iMagicReq);
  }

// Globaler Aufruf, wenn ein Zauber erfolgreich war: Magie trainieren
global func OnClonkSucceededSpell(id idSpell, object pClonk)
  {
  if (!pClonk) if (!(pClonk = this())) return(_inherited(idSpell));
  // Zauberkraft trainieren (plus 1/10 des Zauberwerts)
  if (!ObjectCount(NMGE))
    {
    var iMaxTrain, idInfo;
    // Maximales Training bei Infoobjekten: Nach Ursprungsdefinition richten
    if (idInfo = GetObjectInfoCoreVal("id", "ObjectInfo", pClonk)) iMaxTrain = idInfo->~MaxMagicPhysical();
    // Infodefinition hat kein MaxMagicPhysical (d.h., kein Zauberclonk) oder kein Infoobjekt vorhanden:
    // Training wird eh nicht permanent gesichert; nach aktueller Definition richten
    if (!iMaxTrain) iMaxTrain = pClonk->~MaxMagicPhysical();
    // Trainieren
    TrainPhysical("Magic", Value(idSpell)/MCLK_ManaTrainRate, iMaxTrain, pClonk);
    }
  // Gegebenenfalls weitere Hooks erlauben
  return (_inherited(idSpell, pClonk));
  }


/* Zaubermen� erzeugen und mit verf�gbaren Zaubern eines Spielers f�llen */

global func OpenSpellMenu(object clonk, object command_target, string itemtext, string cbfunc, int player, object magic_source, string emptytext, id select_spell)
  {
  OpenEmptySpellMenu(MCMS, clonk, command_target, magic_source, emptytext);
  var idMagic, i = 0;
  while (idMagic = GetPlrMagic(player, 0, i++)) 
    {
    AddMenuItem(itemtext, cbfunc, idMagic, clonk,0,clonk);
    if(select_spell == idMagic) SelectMenuItem(i-1);
    }
  return true;
  }
  
/* Leeres Zaubermen� erstellen */

global func OpenEmptySpellMenu(id symbol, object clonk, object command_target, object magic_source, string emptytext)
  {
  var extra;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) extra=C4MN_Extra_Components; else extra=C4MN_Extra_ComponentsLiveMagic; }
  else                  { if(ObjectCount(NMGE)) extra=C4MN_Extra_None; else extra=C4MN_Extra_LiveMagicValue; }
  return CreateMenu(symbol, clonk, command_target, extra, emptytext, ObjectNumber(magic_source));
  }


/* Checkt ob genug Zutaten f�r dne Zauber da sind; wenn ja, wird zur�ckgegeben wie oft er gezaubert werden kann */

global func CheckAlchem(id idMagic) { // Par(1)-Par(x): Zaubernde, [Zauberturm], [weitere...]

	// Regel aktiviert?
	if(!ObjectCount(ALCO)) return(MCLK_UnlimitedCast);

	// Par(1) = this() wenn =0
	if(!Par(1)) Par(1)=this();

	var idAlchem,iDesire,iHave, iMin=MCLK_UnlimitedCast, i, m;

	// Components in der Defcore einmal durchgehen...
	for(i=0;GetComponent(0,i,0,idMagic);++i) {

	  // Von jedem Eintrag ID und Anzahl merken
	  idAlchem = GetComponent(0,i,0,idMagic);
	  iDesire = GetComponent(idAlchem,0,0,idMagic);
	  if (!iDesire) continue;
	  iHave = 0;

	  // Alle Objekte der Parameterliste nach der gesuchten Zutat durchsuchen...
	  for(m=1;Par(m);++m) {
	    iHave += GetAlchem(idAlchem,0,Par(m));
	  }
	  
	  // Es kann nur so oft gezaubert werden, wie es die wenigstvorhandene Zutat vorgibt
	  iMin = Min(iMin, iHave / iDesire);
	}
	return(iMin);
}

/* Gibt einen String zur�ck, was zum Zaubern eines Zaubers noch fehlt */

global func GetNeededAlcStr(id idMagic) { // Par(1)-Par(x): Zaubernde, [Zauberturm], [weitere...]

	// Par(1) = this() wenn =0
	if(!Par(1)) Par(1)=this();

	var idAlchem,iDesire, i, m, szString, szSplit;
	szString = "";

	// Components in der Defcore einmal durchgehen...
	for(i=0; GetComponent(0,i,0,idMagic); ++i) {

	  // Von jedem Eintrag ID und Anzahl merken
	  idAlchem = GetComponent(0,i,0,idMagic);
	  iDesire = GetComponent(idAlchem,0,0,idMagic);

	  // Alle Objekte der Parameterliste nach der gesuchten Zutat durchsuchen...
	  for(m=1;Par(m);++m) {
	    iDesire -= GetAlchem(idAlchem,0,Par(m));
	    if(iDesire <= 0) break;
	  }
          // Alle Objekte durchsucht aber nicht genug da? String anh�ngen
	  if(iDesire > 0) {
	    // erstes Mal keinen Seperator
	    if(szString ne "") szSplit = ",|";
	    else szSplit = "";
	    szString = Format("%s%s%dx{{%i}}%s",szString,szSplit,iDesire,idAlchem,GetName(0,idAlchem));
	  }

	}
	return(szString);
}


/* L�scht die Zutaten aus den Objekten ben�tigt f�r den Zauber: Checkt nicht ob genug da sind! */

global func ReduceAlchem(id idMagic) { // Par(1)-Par(x): Zaubernde, [Zauberturm], [weitere...]

	// Regel aktiviert?
	if(!ObjectCount(ALCO)) return(true);

	// Par(1) = this() wenn =0
	if(!Par(1)) Par(1)=this();

	var idAlchem,iDesire, i, j, m, iAlchemCount;

	// Components in der Defcore einmal durchgehen...
	for(i=0;GetComponent(0,i,0,idMagic);++i) {

	  // Von jedem Eintrag ID und Anzahl merken
	  idAlchem = GetComponent(0,i,0,idMagic);
	  iDesire = GetComponent(idAlchem,0,0,idMagic);

	  // Alle Objekte der Parameterliste nach der gesuchten Zutat durchsuchen...
	  for(m=1;Par(m);++m) {

            iAlchemCount = GetAlchem(idAlchem,0,Par(m));
            DoAlchem(idAlchem,-iDesire,Par(m));
            iDesire -= iAlchemCount;

            if(iDesire <= 0) break;
	  }
          // Alle Objekte durchsucht aber nicht genug da? Entwickler sollte das doch pr�fen!
          if(iDesire > 0) return(false, Log("Warning: Some required alchemic components could not be found!"));
	}
	return(true);
}


/* �bergibt die Zutaten f�r den Zauber an das Objekt */

global func IncreaseAlchem(id idMagic, object pTarget) {

	// Regel aktiviert?
	if(!ObjectCount(ALCO)) return(true);

	// Local call/safety
	if (!pTarget) if (!(pTarget = this())) return();

	// Components in der Defcore einmal durchgehen...
	for(var i=0,idAlchem;idAlchem=GetComponent(0,i,0,idMagic);++i)
	  // ...und als Alchemieobjekte dem Target �bergeben
	  DoAlchem(GetComponent(0,i,0,idMagic), GetComponent(idAlchem,0,0,idMagic), pTarget);
	return(true);
}

/* ++++ Alchemiebeh�lter ++++ */

global func AlchemContainerCount(object pContainer) {			// Anzahl der Alchemiebeh�lter im Objekt

  var pAlchem,j;
  for(var i;pAlchem=Contents(i,pContainer);++i)
    if(pAlchem->~IsAlchemContainer())
      ++j;
  return(j);
}


global func AlchemContainer(int iIndex, object pContainer) {		// iIndex'ter Alchemiebeh�lter im Objekt

  // f�r dumme Entwickler
  iIndex = Max(iIndex,0);
  var pAlchem;
  for(var i;pAlchem=Contents(i,pContainer);++i)
    if(pAlchem->~IsAlchemContainer()) {
      if(!iIndex) return(pAlchem);
      --iIndex;
    }
  return(false);
}

global func TransferAlchem(object pStart, object pEnd) {		// transferiert alles alchemische aus Start nach End

  if(!AlchemBag(pStart)) return(false);
  if(!AlchemBag(pEnd,true)) return(false);

  var idAlchem, iAlchem;
  // alle Komponenten, deren Index h�her 0 ist, �bergeben
  while(idAlchem=GetAlchem(0,0,pStart)) {
    iAlchem = GetAlchem(idAlchem,0,pStart);

    // tats�chlich gar keine Zutat sondern ein weiterer Beh�lter?
    if(idAlchem->~IsAlchemContainer()) {
      for(var i=0; i<iAlchem; ++i) {
        var pObj = CreateObject(idAlchem,0,0,-1);
        TransferAlchem(pObj,pEnd);
      }
    }
    // ansonsten normal transferieren
    else {
      DoAlchem(idAlchem,+iAlchem,pEnd);
    }
    // Container wurde gel�scht, abbrechen
    if(!DoAlchem(idAlchem,-iAlchem,pStart)) break;
  }
}


/* ++++ Alchemische Komponenten ++++ */

global func GetAlchem(id idType, int iIndex, object pContainer) {	// idType anggb: Anzahl dieser Zutat
									// iIndex anggb: Id der Zutat dieses Indexes
  if(!AlchemBag(pContainer)) return(false);

  // �berspringt C4ID==0 F�lle
  var iIndexAfter=0, idContent;
  if(!idType)
    while(idContent=GetComponent(0,iIndexAfter,pContainer)) {
      if(!GetComponent(idContent,0,pContainer)) { ++iIndexAfter; continue; }
      // C4ID>0 und iIndex erreicht, fertig
      if(!iIndex) break;
      // ansonsten an den gew�nschten Index ranr�cken
      --iIndex;
      ++iIndexAfter;
      }

  return(GetComponent(idType,iIndexAfter,pContainer));
}

global func DoAlchem(id idType, int iNumber, object pContainer) {	// �ndert die Anzahl von idType um iNumber

  if(!AlchemBag(pContainer,true)) return(false);
  SetComponent(idType,Max(0,GetComponent(idType,0,pContainer)+iNumber),pContainer);
  // wenn Beutel und leer: l�schen
  if(!AlchemCount(pContainer)) {
    if(!(pContainer->~Exhaustion()))
      RemoveObject(pContainer);
    return(false);
  }
  return(true);
}

global func AlchemCount(object pContainer) {				// z�hlt alle Zutaten

  if(!AlchemBag(pContainer)) return(0);
  var idAlchem, iCount;
  for(var i;idAlchem=GetAlchem(0,i,pContainer);++i)
    iCount += GetAlchem(idAlchem,0,pContainer);
  return(iCount);
}

/* ++++ Alchemiebeutel ++++ */

global func AlchemBag(&pObject, bool fCreate) {			// Alchemiebeutel, fCreate=true f�r Erstellen wenn n�tig

  if(!pObject) pObject=this();
  // brauch keinen
  if(pObject->~IsAlchemContainer()) return(true);

  pObject = GetAlchemBag(pObject);
  // nicht gefunden
  if(!pObject) {
    // abbrechen
    if(!fCreate) return(false);
    // neu erzeugen
    pObject = CreateAlchemBag(pObject);
    if(!(pObject->~IsAlchemContainer())) return(false); 
  }
  return(true);
}

global func GetAlchemBag(object pObject) {			// Alchemiebeutel
  var pBag;
  if(!pObject) pObject = this();
  if(pBag=FindObject(ALC_,0,0,0,0,0,0,pObject)) return(pBag);
  // FindObject findet nicht das Kontextobjekt
  if(GetID()==ALC_) if(GetActionTarget()==pObject) return(this());
  return(false);
}

global func CreateAlchemBag(object pObject) {			// Alchemiebeutel erzeugen
  if(!pObject) pObject = this();
  var pBag;
  pBag = CreateObject(ALC_,AbsX(GetX(pObject)),AbsY(GetY(pObject)),-1);
  if(!pBag) return(pObject);
  pBag->~BelongTo(pObject);
  return(pBag);
}