//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Lebensbalken  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2

local pClonk;
local pBulk;
local pMedic;

public func Initialize() 
{
AddEffect("Transform",this,8,1,this);
return 1;
}

public func FxTransformTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
if(!pClonk || !pMedic)Remove();
if(GetID(pMedic->~GetGear(1))!=G_MS)Remove();
if(GetCursor(GetController(pMedic))==pMedic)
{
	SetVisibility(VIS_Owner,pBulk);
	SetVisibility(VIS_Owner,this);
}else
{
	SetVisibility(VIS_None,pBulk);
	SetVisibility(VIS_None,this);
}
var laenge=(GetEnergy(pClonk)*10)/(GetPhysical("Energy",0,pClonk)/10000)/2;
if(laenge<=0)Remove();
SetObjDrawTransform(-1000*laenge,0,0,0,1000,0);
}

private func Remove()
{
RemoveObject(pBulk);	
RemoveObject(this);
}

public func Entrance(){Remove();}
