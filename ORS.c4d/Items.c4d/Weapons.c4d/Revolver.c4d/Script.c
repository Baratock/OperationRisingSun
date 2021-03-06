//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Revolver ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include ORSW

public func HandSize() 			{ return 1000; }
public func HandX()    			{ return PostureValue(7000,6000,4000); }
public func HandY()    			{ return PostureValue(0,-2500,-1000); }
public func BarrelXOffset()	{ return 0; 	 }
public func BarrelYOffset()	{ return -5000;}
public func IsSecondaryWeapon()	{ return true; }

public func OnReload(int i)
{
  if(i == 1) Sound("second_reload");
}

public func FMData1(int data)
{
  if(data == FM_Name)      	return "$Standard$";
  if(data == FM_AmmoID)    	return STAM;
  if(data == FM_AmmoLoad)  	return 6;
  if(data == FM_Reload)    	return 4*36;
  if(data == FM_Recharge)  	return 36;
  if(data == FM_Damage)    	return 50;
  if(data == FM_HeadDamage)	return 150;
  if(data == FM_Recoil)    	return 10;
  if(data == FM_WeaponSound)return "colt_s";
  return Default(data);
}

public func BotData1(int data)
{
  if(data == BOT_Range)    	return 200;
  if(data == BOT_Power)    	return BOT_Power_1;
  return Default(data);
}

public func Fire1()
{  
var user = GetUser();
var angle = user->AimAngle(20,100);
var trail = 200;
var size = 5; 
var range = 200;
var speed = 300;
var dir = GetDir(user)*2-1;
var x,y; user->WeaponEnd(x,y);
var ammo = CreateObject (SHOT,x,y,GetController(user));
ammo-> Launch(angle,speed,range,size,trail,GetFMData(FM_Damage));
MuzzleFlash(35,user,x,y,angle);
BulletCasing(dir*6,3,-dir*5,-20,5);
}
