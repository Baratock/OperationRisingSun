/*-- Flaggenmast --*/

#strict 2

local iFlaggers,iProcess, iStarterFlagg, iIndex,guarded;
local pConnected1,pConnected3,pConnected2,pConnected4,pConnected5;
local szConquerFunc;
protected func Initialize() {
 iProcess = 120;
 iFlaggers = 0;
 iIndex = 1;
 iStarterFlagg = 0;
 guarded=0;
 szConquerFunc = 0;
 AddEffect("ConnectingDots",this,1,16,this,0);
 var Flag = CreateObject(V_FL,15*(-1+2*GetDir()),-25,NO_OWNER);
 LocalN("pFlagPole",Flag) = this;
 Flag->Flying();
 return true;
}

//Effect für die Connecting Dots

protected func FxConnectingDotsTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
 var Flagger = LocalN("iFlaggers",pTarget);
 var clr;
 if(Flagger)
  clr = GetTeamColor(LocalN("iFlaggers",pTarget));
 else
  clr = RGB(130,130,130);
 for(var i = 1;i <= 5; i++)
 {
  var pCFlag = LocalN(Format("pConnected%d",i),pTarget);
  if(!pCFlag)
   continue;
  var x = GetX(pCFlag)-GetX(pTarget);
  var y = GetY(pCFlag)-GetY(pTarget);
  //Log("%d",i);
  for(var j = 1;j <= 4; j++)
  {
   //Log("Dot%d",i);
   CreateParticle("Dot",x*j/9,y*j/9,0,0,45,clr,pTarget);
  }
 }
}

 //Timer fürs Verschieben der Flagge und verändern Einnahme Prozesses
protected func Timer()
{
 var aGrabbers = FindObjects(Find_Distance(60, 0, 30),Find_Func("IsClonk"));
 var obj,i,c;
 while(obj = aGrabbers[i++])
 {
  if(GetPlayerTeam(GetOwner(obj)) == iFlaggers)
   if(iProcess < 120)
   {
    iProcess++;
    if(iProcess == 120)
     Conquered(iFlaggers);
    
   }
  if(GetPlayerTeam(GetOwner(obj)) != iFlaggers)
   if(!guarded)
    for(var j=1;c = eval(Format("pConnected%d",j));j++)
    {
     if((LocalN("iFlaggers",c) == GetPlayerTeam(GetOwner(obj)) && LocalN("iProcess",c) == 120)|| iStarterFlagg == GetPlayerTeam(GetOwner(obj)))
     {
      if(iProcess > 0)
       iProcess--;
      if(iProcess == 0)
       iFlaggers = GetPlayerTeam(GetOwner(obj));
      if(iStarterFlagg == GetPlayerTeam(GetOwner(obj)))
       break;
     }
    }
  
 }
 return true;
}

protected func Conquered(iTeam) {
 guarded = true;
 Schedule("UnGuard()",3*36);
 if(szConquerFunc)
  GameCall(Format("%s",szConquerFunc),iTeam);
 return true;
}

protected func UnGuard() {return guarded = false;}

global func PlaceCnquFlag(int iX,int iY,int iTeam, bool fStart,string szFunc) {
 var pFlag = CreateObject(V_FP,iX,iY,NO_OWNER);
 LocalN("iFlaggers",pFlag) = iTeam;
 LocalN("szConquerFunc",pFlag) = szFunc;
 if(fStart)
  LocalN("iStarterFlagg",pFlag) = iTeam;
 return pFlag;
}

global func ConnectFlags(object pFlag1, object pFlag2) {
 //Überprüfen ob es sich überhaupt um Fahnenmasten handelt
 if(GetID(pFlag1) != V_FP && GetID(pFlag2) != V_FP)
  return false;
 //Überprüfen ob noch Connections frei sind
 if(LocalN("iIndex",pFlag1) > 5 && LocalN("iIndex",pFlag1) > 5)
  return false;
 //WEnn alles Happy dann "verbinden"
 LocalN(Format("pConnected%d",LocalN("iIndex",pFlag1)),pFlag1) = pFlag2;
 LocalN("iIndex",pFlag1)++;
 LocalN(Format("pConnected%d",LocalN("iIndex",pFlag2)),pFlag2) = pFlag1;
 LocalN("iIndex",pFlag2)++;
 return true;
}
