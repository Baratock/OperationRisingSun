//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ M24 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#strict 2
#include ORSW

public func HandSize()      { return 1000; 								}
public func HandX()					{ return PostureValue(13500,9000,6500);}
public func HandY()					{ return PostureValue(0000,2000,0000);} 
public func HasLaser()			{ return GetUpgrade(KLSP); 		}
public func LaserY()				{ return -1; 									}
public func LaserX()				{ return -5; 									}
public func HeadSound()			{ return "Headshot"; 					}
public func BarrelYOffset()	{ return -1000; 							}
public func ScopeViewRange(){ return 1000;								}

public func OnRecharge(int i)
{
Sound("m24_recharge");
var dir = GetDir(GetUser())*2-1;
BulletCasing(dir*6,3,-dir*5,-20,5);
}

public func OnReload(int i)
{
Sound("m24_reload");
}

public func FMData1(int data)
{
	if(data == FM_Name)         return ".308 Winchester";
	if(data == FM_AmmoID)       return STAM;
	if(data == FM_AmmoLoad)     return 5;
	if(data == FM_AmmoUsage)    return 1; 		
	if(data == FM_Reload)       return 36*10;
	if(data == FM_Recharge)     return 36*3;
	if(data == FM_Damage)       return 110;		
	if(data == FM_HeadDamage)		return 200;
	if(data == FM_Recoil)				return 8;
	if(data == FM_Spreading)		return PostureValue(10,4,0);
	if(data == FM_WeaponSound)	return "m24_shot";
	if(data == FM_Aim)          return true;	
	return Default(data);
}

public func BOTData1(int data)
{
	if(data == BOT_Range)    		return 1000;
	return Default(data);
}

public func Fire1()
{
var trail = 40;
var size = 2; 
var range = 1000;
var speed = 500;
var dir = GetDir(GetUser())*2-1;
var angle = GetUser()->AimAngle();	
var x,y; GetUser()->WeaponEnd(x,y);
var ammo = CreateObject(SHOT,x,y,GetController(GetUser()));
ammo->Launch(angle,speed,range,size,trail,GetFMData(FM_Damage));
MuzzleFlash(35,GetUser(),x,y,angle);
}

public func IsUpgradeable(id idUpgrade) 
{
if(GetUpgrade(idUpgrade))return false ;
if(idUpgrade == KLSP) return "Baut Laserpointer an.";
}
