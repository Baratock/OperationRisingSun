//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Körperteil ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
local pClonk;
local iDamage;

public func Construction (object creator)
{
iDamage=0;
pClonk=creator;
SetOwner(GetController(pClonk),this);
SetVisibility(VIS_Owner,this);
AddEffect("BodyPart",this,1,1,this);
}

private func FxBodyPartTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
if(!pClonk)return RemoveObject();
if(!GetAlive(pClonk))return RemoveObject();
if(GetCursor(GetController(pClonk),0)!=pClonk)return SetVisibility(VIS_None,this);
if(!FindObject(RDMG))return SetVisibility(VIS_None,this);
SetVisibility(VIS_Owner,this);
iDamage=LocalN("iHead",pClonk);
var h=100-iDamage;
BoundBy(h,0,100);
SetClrModulation(HSL(h,200,128));
}
