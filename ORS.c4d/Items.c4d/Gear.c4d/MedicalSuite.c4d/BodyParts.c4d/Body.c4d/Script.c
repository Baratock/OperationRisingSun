//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Körperteil ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
local pClonk, pMedic;
local pHead, pLeftArm, pRightArm, pLeftLeg, pRightLeg;

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
if(GetCursor(GetController(pMedic))==pMedic && FindObject(RDMG))
	{
	SetVisibility(VIS_Owner,this);
	SetVisibility(VIS_Owner,pHead);
	SetVisibility(VIS_Owner,pLeftArm);
	SetVisibility(VIS_Owner,pRightArm);
	SetVisibility(VIS_Owner,pLeftLeg);
	SetVisibility(VIS_Owner,pRightLeg);
	}else
	{
	SetVisibility(VIS_None,this);
	SetVisibility(VIS_None,pHead);
	SetVisibility(VIS_None,pLeftArm);
	SetVisibility(VIS_None,pRightArm);
	SetVisibility(VIS_None,pLeftLeg);
	SetVisibility(VIS_None,pRightLeg);
	}
var h=100-pClonk->GetBodyStatus(BP_Body);
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

public func SetParts(object Head,object LeftArm,object RightArm,object LeftLeg,object RightLeg)
{
pHead = Head;
pLeftArm = LeftArm;
pRightArm = RightArm;
pLeftLeg = LeftLeg;
pRightLeg = RightLeg;
}

public func Destruction()
{
if(pHead)RemoveObject(pHead);
if(pLeftArm)RemoveObject(pLeftArm);
if(pRightArm)RemoveObject(pRightArm);
if(pLeftLeg)RemoveObject(pLeftLeg);
if(pRightLeg)RemoveObject(pRightLeg);
}

public func Entrance(){RemoveObject();}
