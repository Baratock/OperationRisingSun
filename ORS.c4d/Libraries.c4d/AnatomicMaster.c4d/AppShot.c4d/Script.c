#strict
#appendto SHT1
local iDamage,iPrec;

public func BulletStrike(object pObj) 
{
	if(GetOCF(pObj)&OCF_Living)
	if(pObj->~IsRealHurtable())
	{
		//Kopftreffer
		if(GetY()<= GetYTop(pObj) + GetDefCoreVal("Height",0,GetID(pObj))/3)
		{
		  DoBodyDmg(iDamage,DMG_Projectile,pObj,BP_Head);
		  AlienBlood(20,0,0,RGBa(255,0,0,0));
  	}else 
  	//Brusttreffer
  	if(GetY()> GetYTop(pObj) + GetDefCoreVal("Height",0,GetID(pObj))/3 && GetY()< GetYTop(pObj) + GetDefCoreVal("Height",0,GetID(pObj))/3*2)
  	{
  		var r = RandomX(1,100);
  		if(r > 40)DoBodyDmg(iDamage,DMG_Projectile,pObj,BP_Body);//Oberkörper wird zu 60% getroffen
  		else if(r > 20 && r <= 40)DoBodyDmg(iDamage,DMG_Projectile,pObj,BP_LArm);//Linker Arm wird zu 20% getroffen
  		else if(r <= 20)DoBodyDmg(iDamage,DMG_Projectile,pObj,BP_RArm);//Rechter Arm wird zu 20% getroffen
  	}else
  	//Beintreffer
  	if(GetY()>= GetYTop(pObj) + GetDefCoreVal("Height",0,GetID(pObj))/3*2)
  	{
  		var r = RandomX(1,100);
  		//Die Chancen stehen 50-50
  		if(r <= 50)DoBodyDmg(iDamage,DMG_Projectile,pObj,BP_LLeg);
  		if(r > 50)DoBodyDmg(iDamage,DMG_Projectile,pObj,BP_RLeg);
  	}
  }else Sparks(70*GetCon()/100,Color(GetActTime()));
  DoDmg(iDamage,DMG_Projectile,pObj,iPrec);
}

private func GetYTop(pObj)
{
return GetY(pObj) + GetDefCoreVal("Offset",0,GetID(pObj),1);
}

func Color(){return _inherited();}
