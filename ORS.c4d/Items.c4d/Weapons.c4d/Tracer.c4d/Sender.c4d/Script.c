//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  GPS-Sender  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
local target;

public func Initialize()
{	AddEffect("Life1",this,2,1,this); }

public func IsGPS() { return true; }

public func AttachTo(object pBy) 
{
	SetAction("Attach",pBy);
}

public func SetTarget(object pTarget)
{
	target=pTarget;
	AttachTo(pTarget);
	AddEffect("Life2",this,2,1,this);
}

public func Solid()
{
SetCategory(GetCategory() | C4D_StaticBack, this);
}

protected func FxLife1Timer(object pTarget, int iEffectNumber, int iEffectTime)
{
	SetXDir(0);
	SetYDir(0);
	if(iEffectTime >= 60*36)return RemoveObject(this);
}

protected func FxLife2Timer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if(!target)return RemoveObject(this);
}
