/*-- Spawn --*/

#strict 2

local idID, iSpwnTimer;

protected func Initialize()
{
 SetVisibility(VIS_None,this);
 idID = 0;
 iSpwnTimer = 1000;
}

global func PlaceObjectSpawn(id ID, int iX, int iY, int iTimer)
{
 if(!iTimer)
  iTimer = 1000;
 if(!ID)
  return false;
 var spwn = CreateObject(A_OS,iX,iY,NO_OWNER);
 spwn->Set(ID,iTimer);
 LocalN("iSpwnTimer",spwn) = iTimer;
 AddEffect("Spawner",spwn,1,1,spwn);
 return true;
}
/*
EffectVars
1 - Object
2 - Timer
*/
protected func FxSpawnerStart(object pTarget, int iEffectNumber, int iTemp)
{
 EffectVar(1,pTarget, iEffectNumber) = CreateObject(LocalN("idID",pTarget),0,0,NO_OWNER);
 EffectVar(2,pTarget, iEffectNumber) = LocalN("iSpwnTimer",pTarget);
 //Log("Test1,%i", LocalN("idID",pTarget));
 return true;
}

protected func FxSpawnerTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
 if(!GetLength(FindObjects(Find_Distance(80),Find_NoContainer(),Find_OCF(OCF_Alive))) && !EffectVar(1,pTarget, iEffectNumber))
 {
  if(EffectVar(2,pTarget, iEffectNumber) <= 0)
  {
   EffectVar(2,pTarget, iEffectNumber) = LocalN("iSpwnTimer",pTarget);
   EffectVar(1,pTarget, iEffectNumber) = CreateObject(LocalN("idID",pTarget),0,0,NO_OWNER);
   AddEffect("FadeIn",EffectVar(1,pTarget, iEffectNumber),1,1,0,A_OS,240);
  }
  else
   EffectVar(2,pTarget, iEffectNumber)--;
 }
}

protected func Set(id ID, int iTimer)
{
 idID = ID;
 iSpwnTimer = iTimer;
 return true;
}

protected func FxFadeInStart(object pTarget, int iEffectNumber, int iTemp, int iAlpha)
{
 EffectVar(1,pTarget,iEffectNumber) = iAlpha;
 return true;
}

protected func FxFadeInTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
 SetClrModulation(RGBa(255,255,255,EffectVar(1,pTarget,iEffectNumber)--),pTarget);
 if(0 >= EffectVar(1,pTarget,iEffectNumber))
  return -1;
 return true;
}
