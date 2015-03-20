//------------------------------------------  40MM Granate  ---------------------------------------
#strict 2

local iDamage,pClonk;

func Construction(object byObj)
{
	iDamage=50;
	pClonk=byObj->~GetUser();
	return 1;
}

public func Launch(int iAngle, int iSpeed,int iDmg)
{
  if(iDmg)iDamage = iDmg;
	SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  
  AddEffect("Angle",this,2,1,this);
  //Hitüberprüfung jeden frame
  AddEffect("HitCheck",this,1,1,0,SHT1,pClonk,true);
}

private func Hit()
{
  HitObject();
}

private func HitObject(object pObject) 
{	
	if(pObject)
	if(GetOCF(pObject)&OCF_Living)
	if(pObject->~IsRealHurtable())
	{
		//Kopftreffer
		if(GetY()<= GetYTop(pObject) + GetDefCoreVal("Height",0,GetID(pObject))/3)
		{
		  DoBodyDmg(100,DMG_Projectile,pObject,BP_Head,GetController(pClonk));
  	}else 
  	//Brusttreffer
  	if(GetY()> GetYTop(pObject) + GetDefCoreVal("Height",0,GetID(pObject))/3 && GetY()< GetYTop(pObject) + GetDefCoreVal("Height",0,GetID(pObject))/3*2)
  	{
  		var r = RandomX(1,100);
  		if(r > 40)DoBodyDmg(100,DMG_Projectile,pObject,BP_Body,GetController(pClonk));//Oberkörper wird zu 60% getroffen
  		else if(r > 20 && r <= 40)DoBodyDmg(100,DMG_Projectile,pObject,BP_LArm,GetController(pClonk));//Linker Arm wird zu 20% getroffen
  		else if(r <= 20)DoBodyDmg(100,DMG_Projectile,pObject,BP_RArm,GetController(pClonk));//Rechter Arm wird zu 20% getroffen
  	}else
  	//Beintreffer
  	if(GetY()>= GetYTop(pObject) + GetDefCoreVal("Height",0,GetID(pObject))/3*2)
  	{
  		var r = RandomX(1,100);
  		//Die Chancen stehen 50-50
  		if(r <= 50)DoBodyDmg(100,DMG_Projectile,pObject,BP_LLeg,GetController(pClonk));
  		if(r > 50)DoBodyDmg(100,DMG_Projectile,pObject,BP_RLeg,GetController(pClonk));
  	}
  }
  BlastObjects(GetX(), GetY(), iDamage*3/2);
  Explode(iDamage,0,0,0,1,Format("handgrenade%d",RandomX(1,2)));
}

private func GetYTop(pObj)
{
return GetY(pObj) + GetDefCoreVal("Offset",0,GetID(pObj),1);
}

public func FxAngleTimer (object pTarget, int iEffectNumber, int iEffectTime)
{
if(iEffectTime>=36*20)return RemoveObject();
var iAngle=Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir(),1);
SetR(iAngle-90);
SetYDir(GetYDir()+GetGravity()/20);
}
