//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ M249 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include ORSW

public func HandSize()      { return(1000); }
public func HandX()         { return(PostureValue(7000,7000,5000)); }
public func HandY()         { return(PostureValue(0000,-500,-1500)); }
public func BarrelXOffset()	{ return(0); }
public func BarrelYOffset()	{ return(-2000); }

public func FMData1(int data)
{
	if(data == FM_Name)      	return "5,56mm NATO"; 
	if(data == FM_AmmoID)     return STAM;
	if(data == FM_AmmoLoad)   return 100;
	if(data == FM_Reload)     return 36*14;
	if(data == FM_Recharge)   return 36/16;
	if(data == FM_Damage)     return 7;
	if(data == FM_HeadDamage)	return 150;
	if(data == FM_Recoil)			return 10;
	if(data == FM_Spreading)	return PostureValue(20,8,3);
	if(data == FM_Auto)       return true;
	if(data == FM_WeaponSound)return "M249_s";
	return Default(data);
}

public func BOTData1(int data)
{
  if(data == BOT_Range)   	return 600;
  if(data == BOT_DmgType) 	return DMG_Projectile;
  return Default(data);
}

public func OnReload(int i)
{
Sound("M249_r");
}

public func Fire1()
{
var user = GetUser();
var trail = 80;
var size = 2; 
var range = 800;
var speed = 300;
var dir = GetDir(user)*2-1;
var angle = user->AimAngle();
var x,y; user->WeaponEnd(x,y);
var ammo = CreateObject (SHOT,x,y,GetController(user));
ammo-> Launch(angle,speed,range,size,trail,GetFMData(FM_Damage));
MuzzleFlash(35,user,x,y,angle);
BulletCasing(dir*6,3,-dir*5,-20,5);
}
