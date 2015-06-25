
#strict

// So gut wie alle relevanten Basen haben eine T�r. 
#appendto DOOR

// Ist eine Basis? Die aktuelle Fahne wird zwischengespeichert
local pMyFlag;
private func GetBase() {
	// Keine Flagge?
	if(!pMyFlag) return(-1);
	// Flagge weht noch?
	if(pMyFlag->GetAction() eq "FlyBase" && 
		 pMyFlag->GetActionTarget() == this())
	  return(pMyFlag->GetOwner());
	// Fahne weg...
	pMyFlag = 0;
	return(-1);
}
private func IsBase() { return GetBase() >= 0; }

// Wird von Flaggen aufgerufen
public func CheckFlag(object obj) {
	// Bereits eine Basis?
	if(GetBase() >= 0) return();
	// Kann eine Basis sein?
	if(!GetDefCoreVal("Base")) return();
	// Kommt in Frage?
	if(GetPlayerName(obj->GetOwner())) {
		pMyFlag = obj;
		pMyFlag->Exit();
		pMyFlag->SetAction("FlyBase", this());
		Sound("Trumpet");
		return();
	}
}

// Wird von Flagge aufgerufen
local iEnergyAvailable;
public func BaseTimer(object pFlag) {
	// Nicht unsere Flagge?
	if(pFlag != pMyFlag) return();
	// Inhalt pr�fen
	for(var i = 0, obj; obj = Contents(i); i++)
		// Lore?
		if(obj->~IsLorry())
			// Inhalt holen
			GrabContents(obj);
		// Lebewesen?
		else if(obj->GetOCF() & OCF_Alive) {
			// L�schen
			if(obj->OnFire())
				obj->Extinguish();
			// Heilen
			if(obj->GetEnergy() < obj->GetPhysical("Energy") / 1000) {
				// Keine Energie da?
				if(!iEnergyAvailable) {
					// Kein Geld da?
					if(GetWealth(GetBase()) < 5)
						continue;
					// Kaufen
					Sound("Cash", true, 0, 100, GetBase() + 1);
					DoWealth(GetBase(), -5);
					iEnergyAvailable += 50;
				}
				// Auff�llen
				var iFillup = Min(Min(3, obj->GetPhysical("Energy") / 1000 - obj->GetEnergy()), iEnergyAvailable);				
				obj->DoEnergy(iFillup); iEnergyAvailable -= iFillup;
			}
		}
}

// Verkaufbare Objekttypen
private func IsSellable(id idItem) {
  // Nur alles womit die Zivilisation noch was anfangen kann, also z.B. seltene Rohstoffe
  return(idItem == GOLD || idItem == OBRL || idItem == CRYS ||
         idItem == WIPF || // als Haustier beliebt
         idItem == FMEG || idItem == MEGG || // Werden f�r geheime Klonierungssexperimente ben�tigt
         idItem == GIDL || idItem == IDOL || // Der Schlager auf jedem Flohmarkt
         idItem == SHEL); // shiny
}

// Preis, zu dem ein bestimmtes Gut gekauft werden kann (-1 = nicht kaufbar)
private func GetBuyPrice(id idItem, int iPlayer) {
  // Was verkaufbar ist, ist automatisch nicht kaufbar
  if(IsSellable(idItem)) return(-1);
  // Standardpreis
  var iValue = GetValue(0, idItem, 0, iPlayer);
  // Noch vorr�tig?
  var fHomebase = (GetHomebaseMaterial(iPlayer, idItem) > 0);
  // Preis verdoppeln, wenn nicht vorr�tig
  if(!fHomebase) iValue *= 2;
  return(iValue);
}

// Preis, zu dem ein bestimmtes Objekt verkauft werden kann (-1 = nicht verkaufbar)
private func GetSellPrice(object pObj, int iPlayer) {
  var idItem = pObj->GetID();
  // Verkaufbar?
  if(!IsSellable(idItem)) return(-1);
  // Standardpreis
  var iValue = GetValue(0, idItem, this(), iPlayer);
  // Inhalt dazuz�hlen
  for(var pCnt, i = 0; pCnt = pObj->Contents(i); i++)
    iValue += GetSellPrice(pCnt, iPlayer);
  // Und dabei bleibt es. Vielleicht mal Preis bei viel Verkaufsvolumen senken? W�re m�chtig fies :)
  return(iValue);
}

// Erzeuge Kaufmen�
private func OpenBuyMenu(object pClonk, id idSelect) {
  // Men� erzeugen
  CreateMenu(GetID(), pClonk, 0, 2, "$TxtNothingToBuy$");
  // Homebase-Materialien durchgehen
  var idItem, j = 0;
  for(var i = 0; idItem = GetHomebaseMaterial(GetBase(), 0, i, C4D_All()); i++) {
    // Kaufpreis bestimmen
    var iValue = GetBuyPrice(idItem, GetBase());
    if(iValue < 0) continue;
    // Men�eintrag erzeugen
    AddMenuItem("$TxtBuyIt$", "BuyIt", idItem, pClonk, 
      GetHomebaseMaterial(GetBase(), idItem), pClonk, 0,
      128, 0, iValue);
    j++;
    // Selektieren?
    if(idSelect == idItem) SelectMenuItem(j-1, pClonk);
  }
}

protected func BuyIt(id idItem, object pClonk, bool fRight, int iValue) {
  // Noch Basis?
  if(GetBase() < 0) return();
  // Verfeindete d�rfen nichts kaufen
  var iPlayer = GetBase(), iBuyPlayer = pClonk->GetOwner();
  if(Hostile(iPlayer, iBuyPlayer)) return();
  // Wert neu berechnen, nur zur Sicherheit (da k�nnte wer tricksen...)
  iValue = GetBuyPrice(idItem, iPlayer);
  // Heimatbasismaterial vorhanden?
  var iHomeBase = GetHomebaseMaterial(iPlayer, idItem);
  // Anzahl der zu kaufenden Elemente
  var iCount = 1;
  if(fRight) {
    // So viele wie auf Lager, bzw. wenn kein Lager vorhanden als Standard 5 St�ck.
    if(iHomeBase) iCount = iHomeBase; else iCount = 5;
  }
  // Entsprechende Anzahl kaufen
  for(var i = 0; i < iCount; i++) {
    // Differenzbetrag entfernen
    var iDiff = Min(iValue - GetValue(0, idItem, 0, iPlayer), GetWealth(iPlayer));
    DoWealth(iPlayer, -iDiff);
    // Sicherstellen, dass das Objekt kaufbar ist
    if(!iHomeBase) DoHomebaseMaterial(iPlayer, idItem, 1);
    // Kaufen!
    var pObj = Buy(idItem, iBuyPlayer, GetBase(), this(), i == 0);
    // Nicht kaufbar?
    if(!pObj) {
      // Differenz wieder gutschreiben
      DoWealth(GetBase(), iDiff);
      // Heimatbasismaterial entfernen
      if(!iHomeBase) DoHomebaseMaterial(iPlayer, idItem, -1);
      break;
    }
  }
  // Kaufmen� wieder �ffnen
  OpenBuyMenu(pClonk, idItem);
}

protected func ContainedUp(object pClonk) {
  // Keine Basis?
  if(GetBase() < 0) return(_inherited());
  // Kaufmen� �ffnen
  OpenBuyMenu(pClonk);
  return(1);
}

protected func ContextBuy(object pClonk) {
  [$TxtBuy$|Condition=IsBase]
  // Keine Basis?
  if(GetBase() < 0) return(_inherited());
  // Kaufmen� �ffnen
  OpenBuyMenu(pClonk);
  return(1);    
}

// Erzeuge Verkaufmen�
private func OpenSellMenu(object pClonk, id idSelect) {
  // Men� erzeugen
  CreateMenu(GetID(), pClonk, 0, 2, "$TxtNothingToSell$");
  // Alle Inhaltsobjekte durchgehen
  var iIDCnt = 0, k = 0;
  for(var pObj, i = 0; pObj = Contents(i); i++) {
    var idItem = pObj->GetID();
    // Bereits in Liste?
    for(var j = 0; j < iIDCnt; j++)
      if(Var(j) == idItem)
        break;
    if(j < iIDCnt) continue;
    // Speichern
    Var(iIDCnt++) = idItem;
    // Preis bestimmen
    var iValue = GetSellPrice(pObj, GetBase());
    if(iValue < 0) continue;
    // Inhalt?
    var strPrompt = "$TxtSellIt$";
    if(pObj->Contents()) strPrompt = "$TxtSellItWithContents$";
    // Men�eintrag erzeugen
    AddMenuItem(strPrompt, "SellIt", idItem, pClonk, 
      ContentsCount(idItem), pClonk, 0,
      128, 0, iValue);
    k++;
    // Selektieren?
    if(idSelect == idItem) SelectMenuItem(k-1, pClonk);
  }
}

protected func SellIt(id idItem, object pClonk, bool fRight, int iValue) {
  // Noch Basis?
  if(GetBase() < 0) return();
  // Verfeindete d�rfen nichts verkaufen (wer wei� was sich damit f�r Schabernack treiben lie�e....?)
  var iPlayer = GetBase();
  if(Hostile(iPlayer, pClonk->GetOwner())) return();
  // Solange verkaufen bis nichts mehr da ist
  while(true) {
    // Erstbestes Objekt
    var pObj = FindContents(idItem);
    if(!pObj) break;
    // Nochmal Verkaufspreis bestimmen (kann sich durchaus von Objekt zu Objekt �ndern...)
    iValue = GetSellPrice(pObj, iPlayer);
    if(iValue < 0) break;
    // Alten Geldwert speichern, f�r Korrekturen
    var iOldWealth = GetWealth(iPlayer);
    // Verkaufen
    if(!Sell(iPlayer, pObj)) break;
    // Geldwert ggf. korrigieren
    SetWealth(iPlayer, iOldWealth + iValue);
    // Nur wiederholen falls gew�nscht
    if(!fRight) break;
  }
  // Verkaufsmen� wieder �ffnen
  OpenSellMenu(pClonk, idItem);
}

protected func ContainedDig(object pClonk) {
  [$TxtSell$]
  // Keine Basis?
  if(GetBase() < 0) return(_inherited());
  // Kaufmen� �ffnen
  OpenSellMenu(pClonk);
  return(1);
}

protected func ContextSell(object pClonk) {
  [$TxtSell$|Condition=IsBase]
  // Keine Basis?
  if(GetBase() < 0) return(_inherited());
  // Kaufmen� �ffnen
  OpenSellMenu(pClonk);
  return(1);    
}
