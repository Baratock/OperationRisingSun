//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Colt ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#strict 2

#include ORSW

public func HandSize() 			{ return 1000; }
public func HandX()    			{ return PostureValue(7000,7000,5000); }
public func HandY()    			{ return PostureValue(1000,1000,0000); }
public func BarrelXOffset()	{ return 0; 	 }
public func BarrelYOffset()	{ return -2500;}
public func HasLaser()			{ return GetUpgrade(KLSP);}
public func IsSecondaryWeapon()	{ return true; }

public func OnReload(int i)
{
  if(i == 1) Sound("colt_r");
}

public func FMData1(int data)
{
  if(data == FM_Name)      return ".45 ACP";
  if(data == FM_AmmoID)    return STAM;
  if(data == FM_AmmoLoad)  return 7;
  if(data == FM_Reload)    return 4*36;
  if(data == FM_Recharge)  return 36/2;
  if(data == FM_Damage)    return 22;
  if(data == FM_HeadDamage)return 150;
  if(data == FM_Recoil)    return 6;
  if(data == FM_WeaponSound)return "colt_s";
  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(300);
  if(data == BOT_Power)    return(BOT_Power_1);

  return(Default(data));
}


public func Fire1()    // Projektilschuss
{  
var user = GetUser();
var angle = user->AimAngle();
var trail = 100;
var size = 4; 
var range = 300;
var speed = 300;
var dir = GetDir(user)*2-1;
var x,y; user->WeaponEnd(x,y);
var ammo = CreateObject (SHOT,x,y,GetController(user));
ammo-> Launch(angle,speed,range,size,trail,GetFMData(FM_Damage));
MuzzleFlash(35,user,x,y,angle);
BulletCasing(dir*6,3,-dir*5,-20,5);
}

public func IsUpgradeable(id idUpgrade) 
{
if(GetUpgrade(idUpgrade))return false ;
if(idUpgrade == KLSP) return "Baut Laserpointer an.";
}
