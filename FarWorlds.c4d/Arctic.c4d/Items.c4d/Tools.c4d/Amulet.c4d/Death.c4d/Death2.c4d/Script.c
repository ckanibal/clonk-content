/*-- Todesamulett 2 --*/

#strict

#include AMD1

public func Activate(pClonk) {
  [$TxtMagic$]
  if(GetPhysical("Magic",2,pClonk)<100000) return(0,Message("$TxtsisnoAngakok$",this(),GetName(pClonk)));
  // Clonk soll anhalten
  SetComDir(COMD_Stop(),pClonk);
  // Men� �ffnen
  var iExtra;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) iExtra=1; else iExtra=5; }
  else                  { if(ObjectCount(NMGE)) iExtra=0; else iExtra=3; }
  CreateMenu(MCMS,pClonk,this(),iExtra,"$TxtNoMagic$",GetMagicEnergy(pClonk));
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", LGT2, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", MGSW, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", DTFH, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", DTSL, pClonk,0,pClonk );
  AddMenuItem( "$TxtMagic$: %s", "DoMagic", EXTM, pClonk,0,pClonk );
  return(1);
}