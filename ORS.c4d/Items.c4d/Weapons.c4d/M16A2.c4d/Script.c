//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ M16A2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include ORSW

local schuss,iSpeed,iMaxSpeed,iMinSpeed, pParabel;

public func HandSize()      { return 1000; }
public func HandX()         { return PostureValue(5000,5000,4000); }
public func HandY()         { return PostureValue(1000,1000,0000); }
public func BarrelXOffset()	{ return 0; }
public func BarrelYOffset()	{ return -1000; }

public func Initialize()
{
schuss = 0;
iSpeed = 100;
iMinSpeed = 100;
iMaxSpeed = 180;
return  _inherited();
}

public func FMData1(int data)
{
	if(data == FM_Name)			return ".223 Remington"; 
	if(data == FM_AmmoID)     	return STAM;
	if(data == FM_AmmoLoad)   	return 30;
	if(data == FM_AmmoUsage)  	return 3;
	if(data == FM_Reload)     	return 36*4;
	if(data == FM_Recharge)   	return 36/2;
	if(data == FM_Damage)     	return 18;
	if(data == FM_HeadDamage)	return 150;
	if(data == FM_Recoil)		return 6;
	if(data == FM_Spreading)	return PostureValue(3,2,1);
	if(data == FM_Auto)       	return false; // Feursalve
	if(data == FM_WeaponSound)	return "m16_shot";
	return Default(data);
}

public func BOTData1(int data)
{
  if(data == BOT_Range)    		return 400;
  if(data == BOT_DmgType) 		return DMG_Projectile;
  return Default(data);
}

public func Fire1()
{
	if(GetUser() && GetUser()->~ReadyToFire())
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
		//Ton pro Kugel abspielen
		Sound(GetFMData(FM_WeaponSound),0,this,100,0,-1);
		ScheduleCall(this,"WeaponSound",2,0);
		//Es werden 3 schüsse abgefeurt: Feuerstoß
		if(GetUser())
		{
			if (++schuss < 3)return ScheduleCall(0,"Fire1",4);
			else if(schuss >= 3) schuss = 0;
		}
	}
}

public func FMData2(int data)
{
	if(data == FM_Name)      		return "40mm Grenade"; 
	if(data == FM_AmmoID)     	return STAM;
	if(data == FM_Icon)					return 40MM;
	if(data == FM_AmmoLoad)   	return 1;
	if(data == FM_AmmoUsage)  	return 1;
	if(data == FM_AmmoSlot)		return 2;
	if(data == FM_Reload)     	return 36*10;
	if(data == FM_Recharge)   	return 36;
	if(data == FM_Damage)     	return 30;
	if(data == FM_WeaponSound)	return "m203_f";
	if(data == FM_NoCrosshair)	return true;
	return Default(data);
}

public func BOTData2(int data)
{
  if(data == BOT_Range)    		return 400;
  if(data == BOT_DmgType) 		return DMG_Projectile;
  return Default(data);
}

public func Fire2()
{
var user = GetUser();
var dir = GetDir(user)*2-1;
var angle = user->AimAngle();
var x,y; user->WeaponEnd(x,y);
//Granate erzeugen
var ammo = CreateObject (40MM,x,y,GetController(user));
ammo-> Launch(angle,iSpeed,GetFMData(FM_Damage));
//Rauchpartikel
RauchErzeugen();
//Granatenhülle auswerfen
BulletCasing(dir*6,3,-dir*5,-20,8);
Sound(GetFMData(FM_WeaponSound),0,this,100,0,-1);
}

public func RauchErzeugen()
{
	var x,y,dy,dx,r;
	GetUser()->WeaponEnd(x,y);
	GetUser()->WeaponAt(dy,dx,r);
	if(GetDir(GetUser()))r-=180;
	if(!GetDir(GetUser()))r*=-1;
	r-=180;
	var dir = GetDir(GetUser())*2-1;
	for(var i=0;i <= 8;i++)
	{	
		var randomD = RandomX(1,4);
		var randomR = RandomX(-40,40);
		var xDir = Cos(r+randomR,randomD);
		var yDir = Sin(r+randomR,randomD);
  	CreateParticle("AbluftRauch",x,y,xDir,yDir,30,RGBa(255, 255, 255, 100));
  }
  return 1;
}

public func ControlDig(pClonk)
{
	//wenn er mit Granatenwerfer zielt
  if(GetUser()->~IsAiming())
  if(GetFireMode()==2)
  {
  	//range erhöhen
    iSpeed += 20;
    //Max überschritten?
    if(iSpeed>iMaxSpeed) iSpeed = iMinSpeed;
    //Stärke anzeigen
    PlayerMessage(GetOwner(pClonk), "Range %d", pClonk, iSpeed);
    return 1;
  }
  
  return _inherited();
}

public func OnReload(int i)
{
  if(GetFireMode()==1)Sound("M16A2_reload");
  if(GetFireMode()==2)Sound("m203_r");
}

public func OnAimStart(int iFireMode)
{
AddEffect("Aiming",this,2,1,this);
}

public func FxAimingTimer (object pTarget, int iEffectNumber, int iEffectTime)
{
if(!GetUser())return -1;
if(!GetUser()->~IsAiming())return -1;
var x,y,xDir,yDir,iClr;
var user = GetUser();
user->WeaponEnd(x,y);
//Farbmodulation nach Ladestand einstellen
if(GetCharge()==1000)iClr=RGB(0,255,0);
else iClr=RGB(255,0,0);
//Parabel anzeigen
if(GetFireMode()==2 && GetID(Contents(0,GetUser()))==GetID(this))pParabel=AddParabel(user,GetX(user)+x,GetY(user)+y,user->AimAngle(),iSpeed*6/11,iClr);
else RemoveParabel(GetUser());
}

public func FxAimingStop (object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
if(GetUser())RemoveParabel(GetUser());
else if(pParabel)RemoveObject(pParabel);
}
