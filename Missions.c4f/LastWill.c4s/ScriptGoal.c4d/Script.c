/*-- Scriptdefiniertes Spielziel --*/

#strict
#include GOAL

/* Lokale Variablen */

local goalFulfilled;
local messageFulfilled;
local messageUnfulfilled;
        
protected func Initialize()
{
  messageFulfilled = "$MsgGoalFulfilled$";
  messageUnfulfilled = "$MsgGoalUnfulfilled$";
  return(_inherited());
}          
        
public func IsFulfilled()
{
  return(goalFulfilled);
}

public func Fulfill()
{
  goalFulfilled = 1;
}

public func SetMessages(strFulfilled, strUnfulfilled)
{
  messageFulfilled = strFulfilled;
  messageUnfulfilled = strUnfulfilled;  
}

protected func Activate(iPlayer)
{
  // Erf�llt
  if (IsFulfilled()) 
    return(MessageWindow(Format(messageFulfilled), iPlayer));
  // Noch nicht erf�llt
  else
    return(MessageWindow(Format(messageUnfulfilled), iPlayer));
}
