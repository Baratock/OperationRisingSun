//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DSR-1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include ORSW

public func HandSize()      { return 900; 								}
public func HandX()					{ return PostureValue(14000,10000,9000);}
public func HandY()					{ return PostureValue(-500,2500,0); } 
public func HeadSound()			{ return "Headshot"; 					}
public func ScopeViewRange(){ return 1000;								}
public func HasLaser()			{ return GetUpgrade(KLSP); 		}

public func OnReload(int i)
{
Sound("SVD_reload");
}

public func FMData1(int data)
{
	if(data == FM_Name)       	return ".308 Winchester";
	if(data == FM_AmmoID)     	return STAM;
	if(data == FM_AmmoLoad)   	return 10;
	if(data == FM_AmmoUsage)  	return 1; 		
	if(data == FM_Reload)     	return 10*36;
	if(data == FM_Recharge)   	return 1*36+1*18;
	if(data == FM_Damage)     	return 60;
	if(data == FM_HeadDamage)		return 150;			
	if(data == FM_Recoil)				return 26;	
	if(data == FM_Aim)        	return true;
	if(data == FM_Spreading)		return PostureValue(10,4,1);	
	if(data == FM_WeaponSound)	return "SVD_fire";	
	return Default(data);
}

public func BOTData1(int data)
{
	if(data == BOT_Range)    		return 800;
	return Default(data);
}

public func Fire1()
{
var trail = 40;
var size = 3; 
var range = 800;
var speed = 500;
var dir = GetDir(GetUser())*2-1;
var angle = GetUser()->AimAngle();
var x,y; GetUser()->WeaponEnd(x,y);
var ammo = CreateObject(SHOT,x,y,GetController(GetUser()));
ammo->Launch(angle,speed,range,size,trail,GetFMData(FM_Damage));
MuzzleFlash(35,GetUser(),x,y,angle);
BulletCasing(dir*6,3,-dir*5,-20,5);
}

public func IsUpgradeable(id idUpgrade) 
{
if(GetUpgrade(idUpgrade))return false ;
if(idUpgrade == KLSP) return "Baut Laserpointer an.";
}
