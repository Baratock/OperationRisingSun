//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  SPAS-12  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include ORSW

public func HandSize()			{ return 1000;  	}
public func HandX()					{ return PostureValue(7000,7000,5000); 	}
public func HandY()					{ return PostureValue(2000,2000,0000);  }
public func BarrelYOffset()	{ return -1000;	}

public func FMData1(int data)
{
	if(data == FM_Name)       		return "10 Gauge Shells";
	if(data == FM_AmmoID)     		return STAM;
	if(data == FM_AmmoLoad)   		return 8;
	if(data == FM_AmmoUsage)  		return 1;
	if(data == FM_Recharge)   		return 36;
	if(data == FM_Reload)     		return 10*36;
	if(data == FM_Damage)     		return 4;
	if(data == FM_Spreading)			return 14;
	if(data == FM_PushingDistance)return 3;
	if(data == FM_Recoil)					return 18;
	if(data == FM_WeaponSound)		return "HF_s";
	return Default(data);
}

public func OnRecharge(int i)
{
Sound("SPAS_rc");
var dir = GetDir(GetUser())*2-1;
BulletCasing(dir*6,3,-dir*5,-20,5);
}

public func Fire1()
{
var user = GetUser();
var x,y;
user->WeaponEnd(x,y);
var dir = GetDir(user)*2-1;
var trail = 300;
var size = 3; 
var speed = 300;
for(var i=0; i < 20; i++)
{
	var range = RandomX(170,230);
	var angle = user->AimAngle();
	var ammo = CreateObject (SHOT,x,y,GetController(user));
	ammo-> Launch(angle,speed,range,size,trail,GetFMData(FM_Damage));
}
MuzzleFlash(35,user,x,y,angle);
}
