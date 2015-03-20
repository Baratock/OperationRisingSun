//-------------------------------------------- Flash ----------------------------------------------
#strict 2
local pFlashedClonk;
local iModifier;

public func Initialize() 
{
	//Das ganze Feld muss aufgefüllt sein
  SetObjDrawTransform(4000*1000, 0,0, 0, 4000*1000, 0, 0, 0);
  Sound("Piep",1,0,50,GetController(this)+1);   
	ScheduleCall(this,"FadeOut",2*36);//so lange kann der clonk gar nichts sehen
	AddEffect("Sichtbarkeit",this,20,1,this);
	iModifier=10;
  return 1;
}

public func SetModifier(int iMod){iModifier=iMod;}

protected func FxSichtbarkeitTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
SetPosition(GetX(pFlashedClonk),GetY(pFlashedClonk)-30,this);
if(GetCursor(GetController(pFlashedClonk))==pFlashedClonk)SetVisibility(VIS_Owner);
else SetVisibility(VIS_None);
}

protected func FadeOut()
{
	//Ab sofort kommt die Sehkraft zurück
	AddEffect("KommtWieder",this,100,3,this);
}

protected func FxKommtWiederTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	var w = iEffectTime/10*iModifier/10;
	//Der Blitz, der die Sicht behindert, wird immer transparenter
	SetClrModulation(RGBa(255,255,255,2*w),pTarget);
	if (w >= 120) 
	{
		RemoveObject(pTarget);
	}
}
