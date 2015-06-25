/*-- Stalaktit --*/

#strict 2

local iPhase;
local pGrow;
local pChild;

func Initialize() {
  // Ein zuf�lliges Aussehen
  SetAction("Hanging");
  SetPhase(iPhase=Random(4));
}

// Ein neuer Stalaktit wird erzeugt
func Grow()
{
  SetObjDrawTransform(1000, 0, 0, 0, 0);
  pGrow=100;
}

func Check() {
  // Noch im Wachsen?	
  if(pGrow)
  {
    // In der Wachstumsphase wird er immer Gr��er	  
    pGrow-=5;
    SetObjDrawTransform(1000, 0, 0, 0, 10*(100-pGrow), -pGrow*190);
    return true;
  }

  // Im Fallen?
  if(GetAction() == "Falling") return(Falling());
  
  var clnk;
  // Ausgewachsene h�ngende Stalaktite fallen vorzugsweise auf Clonks runter
  for (clnk in FindObjects(Find_InRect(-25,0,50,100), Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember))) 
    if(!Random(2)) {
      // Und wenn wir noch keinen Nachwuchs haben einen neuen Stalaktit heranwachsen lassen	    
      if(!pChild) (pChild = CreateObject(GetID(), 0, 19))->Grow();
      Sound("Earthquake");
      SetAction("Falling");
      SetPhase(iPhase);
      RemoveVertex(0);
      RemoveVertex(0);
    }
}

// Im Fallen Schaden machen
func Falling() {
  var clnk;
  for (clnk in FindObjects(Find_InRect(-7,-19,14,50), Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember))) 
    DoEnergy(-1-Random(5),clnk);
}

// Am Boden in die Einzelteile zerfallen
func Hit() {
  var obj;
  Sound("RockBreak*");
  Sound("Blast2",0,0,50);
  // links oben
  ObjectSetAction(obj=CreateObject(_STP,-4,-4,GetOwner()),"Exist");
  SetPhase(iPhase*2,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(30)-15,-Random(35)+10,obj);
  // rechts oben
  ObjectSetAction(obj=CreateObject(_STP,3,-4,GetOwner()),"Exist");
  SetPhase(iPhase*2+1,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(30)-15,-Random(35)+10,obj);
  // unten
  ObjectSetAction(obj=CreateObject(_STP,0,12,GetOwner()),"LameExist");
  SetPhase(iPhase,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(10)-15,-Random(35)+10,obj);
	
  RemoveObject();
}
