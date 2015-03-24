//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ AK-47 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include ORSW

public func HandSize()      { return(900); }
public func HandX()         { return(PostureValue(5000,5000,3000)); }
public func HandY()         { return(PostureValue(1000,1000,-500)); }
public func BarrelXOffset()	{ return(0); }
public func BarrelYOffset()	{ return(-2000); }

public func FMData1(int data)
{
	if(data == FM_Name)      	return "M43"; 
	if(data == FM_AmmoID)     return STAM;
	if(data == FM_AmmoLoad)   return 30;
	if(data == FM_Reload)     return 36*4;
	if(data == FM_Recharge)   return 36/9;
	if(data == FM_Damage)     return 10;
	if(data == FM_HeadDamage)	return 150;
	if(data == FM_Recoil)			return 16;
	if(data == FM_Spreading)	return PostureValue(4,3,2);
	if(data == FM_Auto)       return true;
	if(data == FM_WeaponSound)return "ak47_s";
	return Default(data);
}

public func FMData2(int data)
{
	if(data == FM_Name)      	return "M43 Single Shot"; 
	if(data == FM_AmmoID)		return STAM;
	if(data == FM_AmmoLoad)		return 30;
	if(data == FM_AmmoUsage)	return 1;
	if(data == FM_Reload)		return 36*4;
	if(data == FM_Recharge)		return 36/8;
	if(data == FM_Damage)		return 13;
	if(data == FM_HeadDamage)	return 150;
	if(data == FM_Recoil)		return 16;
	if(data == FM_Spreading)	return PostureValue(3,2,1);
	if(data == FM_Auto)       	return false;
	if(data == FM_WeaponSound)	return "ak47_s";
	return Default(data);
}

public func BOTData1(int data)
{
  if(data == BOT_Range)   	return 300;
  if(data == BOT_DmgType) 	return DMG_Projectile;
  return Default(data);
}

public func Fire1()
{
var user = GetUser();
var trail = 80;
var size = 2; 
var range = 600;
var speed = 300;
var dir = GetDir(user)*2-1;
var angle = user->AimAngle();
var x,y; user->WeaponEnd(x,y);
var ammo = CreateObject (SHOT,x,y,GetController(user));
ammo-> Launch(angle,speed,range,size,trail,GetFMData(FM_Damage));
MuzzleFlash(35,user,x,y,angle);
BulletCasing(dir*6,3,-dir*5,-20,5);
}

public func Fire2()
{
	Fire1();
}
