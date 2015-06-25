/*-- CaptureTheFlag --*/

#strict

private func Execute()
{
  // Alle Spieler �berpr�fen
  var iPlrCnt, iPlr = 0;
  for (iPlrCnt = GetPlayerCount(); iPlrCnt; iPlr++)
    if (GetPlayerName(iPlr))
      {
      iPlrCnt--;
      if (GetPlayerTeam(iPlr) != -1) if (!FindObjectOwner(FLAG, iPlr))
        EliminatePlayer(iPlr);
      }
}    

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}
