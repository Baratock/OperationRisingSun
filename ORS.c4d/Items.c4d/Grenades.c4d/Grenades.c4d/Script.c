/*-- Grundfunktion --*/

#strict 2

/*--Locals*/
local used;
local pThrower;
local iTime;
//local iStrengh;

public func HandSize()  { return 800; }
public func HandX()     { return 4000; }
public func HandY()     { return 0; }
public func IsDrawable() { return true; }


protected func Initialize()
{
iTime = 36*3;
//iStrengh = 4;
used = false;
}


public func Departure(object pContainer)
{
 if(IsUsed())
  pThrower = pContainer;
 return true;
}

public func ControlThrow(object pBy)
{
 if(!IsUsed())
 {
 	Active(pBy);
 	return true;
 }
 if(!WildcardMatch(GetAction(pBy),"Walk*"))
  return false;
 var dir = -1+2*GetDir(pBy);
 Exit(this,dir*4+GetXDir(pBy),-2,RandomX(-10,-10),dir*5,-3,dir*30);
 Schedule("SetAction(\"Throw\")",1,1,pBy);
 return true;
}

public func Hit()
{
SetXDir(GetXDir()/3*2);
}

public func ControlDigDouble(object pClnk)
{
	if(IsUsed())
		return 1;
	var menu = CreateQuickMenu(this, pClnk);
	menu->Add(0, SNB1, 0, "Timer", 1);
	menu->Add(1, SNB2, 0, "Timer", 2);
	menu->Add(2, SNB3, 0, "Timer", 3);
	menu->Add(3, SNB4, 0, "Timer", 4);
	menu->Add(4, SNB5, 0, "Timer", 5);
	return true;
}

protected func Timer(int Time) { iTime=36*Time; }

public func Active(object pClnk)
{
AddEffect("GrenExplode",this,1,1,this,0,iTime,TypeColor()); 
used = true;
pThrower = Contained();
Sound("activate_g");
}

protected func FxGrenExplodeStart(object pTarget, int iEffectNumber, int iTemp,int iTimer,int iColor) {
 EffectVar(1,pTarget,iEffectNumber) = iTimer;
 EffectVar(2,pTarget,iEffectNumber) = iColor;
 return true;
}

protected func FxGrenExplodeTimer(object pTarget, int iEffectNumber, int iEffectTime) {
 if(iEffectTime >= EffectVar(1,pTarget,iEffectNumber))
 {
  pTarget->SetController(GetController(LocalN("pThrower",pTarget)));
  pTarget->~Explosion();
  return -1;
 }
 if(EffectVar(2,pTarget,iEffectNumber) && !Contained(pTarget) && !InLiquid(pTarget) && (iEffectTime%4 == 2))
 {
  pTarget->CreateParticle("SignalSmoke",0,0,RandomX(-3,3),-14,55,(Random(30)&255)<<24 |EffectVar(2,pTarget,iEffectNumber));
 }
 if(GetLength(pTarget->FindObjects(Find_AtPoint(),Find_NoContainer(),Find_OCF(OCF_Alive))))
 {
  SetXDir(GetXDir(pTarget)*5/6,pTarget);
  SetYDir(GetYDir(pTarget)*5/6,pTarget);
 }

 return true;
}
protected func TypeColor() {return 0;}
public func IsGrenade() { return GetID() != RSGR; }
public func Explosion() {}

public func CSName() {return "None";}
protected func CSSymbol() {return GetID(this);}
public func CustomSettings(object pClnk) {}


/*NaczhfrageCallbacks*/

//Benutzt??
protected func IsUsed() { return used;}
/*
//Wurfstärke
protected func GetGrenadeStrengh() {
return iStrengh;
}
*/
