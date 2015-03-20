//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Tracer ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#strict 2

#include ORSW

public func HandSize() 					{ return 900; }
public func HandX()    					{ return PostureValue(7000,7000,5000); }
public func HandY()    					{ return PostureValue(1000,1000,0000); }
public func BarrelXOffset()			{ return 0; 	 }
public func BarrelYOffset()			{ return -2500;}
public func HasLaser()					{ return GetUpgrade(KLSP);}
public func IsSecondaryWeapon()	{ return true; }

public func OnReload(int i)
{
  if(i == 1) Sound("second_reload");
}

public func FMData1(int data)
{
  if(data == FM_Name)      	return "GPS-Emitter";
  if(data == FM_AmmoID)    	return STAM;
  if(data == FM_AmmoLoad)  	return 1;
  if(data == FM_Reload)    	return 10*36;
  if(data == FM_Recharge)  	return 36;
  if(data == FM_Damage)    	return 10;
  if(data == FM_WeaponSound)return "colt_s";
  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    	return(100);
  if(data == BOT_Power)    	return(BOT_Power_1);

  return(Default(data));
}


public func Fire1()
{  
var user = GetUser();
var angle = user->AimAngle();
var trail = 1;
var size = 2; 
var range = 800;
var speed = 300;
var dir = GetDir(user)*2-1;
var x,y; user->WeaponEnd(x,y);
var ammo = CreateObject (TRSH,x,y,GetController(user));
ammo-> Launch(angle,speed,range,size,trail,GetFMData(FM_Damage));
}

public func IsUpgradeable(id idUpgrade) 
{
if(GetUpgrade(idUpgrade))return false ;
if(idUpgrade == KLSP) return "Baut Laserpointer an.";
}
