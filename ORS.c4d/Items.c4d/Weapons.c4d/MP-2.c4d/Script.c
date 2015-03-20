//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MP2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include ORSW

public func HandSize() 			{ return 1000; }
public func HandX()    			{ return PostureValue(6500,4500,2000); }
public func HandY()    			{ return PostureValue(1000,-1000,-500); }
public func BarrelXOffset()	{ return 0; 	 }
public func BarrelYOffset()	{ return -3500;}
public func IsSecondaryWeapon()	{ return true; }

public func OnReload(int i)
{
  if(i == 1) Sound("MP_r");
}

public func FMData1(int data)
{
  if(data == FM_Name)      	return "9mm Parabellum";
  if(data == FM_AmmoID)    	return STAM;
  if(data == FM_AmmoLoad)  	return 30;
  if(data == FM_Reload)    	return 4*36;
  if(data == FM_Recharge)  	return 36/12;
  if(data == FM_Damage)    	return 6;
  if(data == FM_HeadDamage)	return 150;
  if(data == FM_Recoil)    	return 8;
  if(data == FM_Spreading)	return 2;
  if(data == FM_Auto)       return true;
  if(data == FM_WeaponSound)return "MP_s";
  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return 200;
  if(data == BOT_Power)    return BOT_Power_1;

  return(Default(data));
}


public func Fire1()
{  
var user = GetUser();
var angle = user->AimAngle(20,100);
var trail = 80;
var size = 2; 
var range = 260;
var speed = 300;
var dir = GetDir(user)*2-1;
var x,y; user->WeaponEnd(x,y);
var ammo = CreateObject (SHOT,x,y,GetController(user));
ammo-> Launch(angle,speed,range,size,trail,GetFMData(FM_Damage));
MuzzleFlash(35,user,x,y,angle);
BulletCasing(dir*6,3,-dir*5,-20,5);
}

