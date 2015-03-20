//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Körperteil ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
local pClonk, pMedic;

public func Initialize()
{
AddEffect("BodyPart",this,2,1,this);
SetObjDrawTransform(600,0,0,0,600,0,this);
}

private func FxBodyPartTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
if(!pClonk || !pMedic)RemoveObject();
if(!GetAlive(pClonk))RemoveObject();
if(GetID(pMedic->~GetGear(1))!=G_MS)RemoveObject();
var h=100-pClonk->GetBodyStatus(BP_RLeg);
BoundBy(h,0,100);
SetClrModulation(HSLa(h,200,128,128));
}

public func SetClonk(object clonk,object medic)
{
pClonk=clonk;
pMedic=medic;
AttachTo(pClonk);
return SetOwner(GetController(pMedic),this);
}

public func Entrance(){RemoveObject();}
