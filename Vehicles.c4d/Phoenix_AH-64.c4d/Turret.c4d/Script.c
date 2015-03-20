//--------------------------------------------- Entry ---------------------------------------------
#strict 2
#include WEPN

local pCreator;
local iAngle;

public func Construction (object creator)
{
iAngle=0;
pCreator = creator;
SetObjectOrder(this,pCreator);
AddEffect("Attach",this,49,1,this);
}

public func AttachTo(object pBy) 
{
	SetAction("Attach",pBy);
}

public func FxAttachTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
if(GetAction(pCreator)=="FlyTurn") SetAction("Turn");
SetActionData(7);
SetDir(GetDir(pCreator));
SetR(GetR(pCreator)+iAngle);
}

public func ControlUp(object controller)
{
if(GetDir()==DIR_Left)
{
iAngle+=2;
if(iAngle > 10)iAngle=10;
else if(iAngle < -90)iAngle=-90;
}else
{
iAngle-=2;
if(iAngle < -10)iAngle= -10;
else if(iAngle > 90)iAngle= 90;
}
return 1;
}

public func ControlDown(object controller)
{
if(GetDir()==DIR_Left)
{
iAngle-=2;
if(iAngle > 10)iAngle=10;
else if(iAngle < -90)iAngle=-90;
}else
{
iAngle+=2;
if(iAngle < -10)iAngle= -10;
else if(iAngle > 90)iAngle= 90;
}
return 1;
}

private func VerticesAktualisieren()//Damit sich beim Drehen auch die Vertices mitbewegen
{   
    var vertexCount = GetDefCoreVal("Vertices", "DefCore", GetID());
    if(GetDir() == DIR_Right)
    {
		  for (var i = 0; i < vertexCount; i++) 
		  {
		     var vertexX = GetDefCoreVal("VertexX", "DefCore", GetID(), i);
		     var vertexY = GetDefCoreVal("VertexY", "DefCore", GetID(), i);
		     SetVertex(i, 0, -vertexX, this, 2);
		     SetVertex(i, 1, vertexY, this, 2);
		  }
      return 1;
  	}else if(GetDir() == DIR_Left)
		{
		  for (var i = 0; i < vertexCount; i++) 
		  {
		     var vertexX = GetDefCoreVal("VertexX", "DefCore", GetID(), i);
		     var vertexY = GetDefCoreVal("VertexY", "DefCore", GetID(), i);
		     SetVertex(i, 0, vertexX, this, 2);
		     SetVertex(i, 1, vertexY, this, 2);
		  }
		  return 1;
		}
		iAngle = -iAngle;
}

//-----------------------------------------------Waffenteil----------------------------------------
public func FMData1(int data)
{
if(data == FM_Name)       return("MG"); 
if(data == FM_AmmoID)     return(STAM);
if(data == FM_AmmoLoad)   return(100);
if(data == FM_Reload)     return(36*10);
if(data == FM_Recharge)   return(2);
if(data == FM_Damage)     return(10);
if(data == FM_Auto)       return(true);

return(Default(data));
}

public func BOTData1(int data)
{
  if(data == BOT_Range)    return(500);
  if(data == BOT_DmgType)  return(DMG_Projectile);

  return(Default(data));
}


public func Fire1()
{
var user = GetUser();
var trail = 40;
var size = 3; 
var range = 1000;
var speed = 400;
var dir = GetDir()*2-1;
var angle = GetR()+dir*90;
var x,y; user->WeaponEnd(x,y);
var ammo = CreateObject (SHT1,x,y,GetController(user));
ammo-> Launch(angle,speed,range,size,trail,GetFMData(FM_Damage));
BulletCasing(dir*6,3,-dir*5,-20,5);
}

public func OnAutoStart()
{
  Sound("m4a1",0,0,0,0,1);
}

public func OnAutoStop()
{
  Sound("m4a1",0,0,0,0,-1);
}

public func FMData2(int data)
{
if(data == FM_Name)       return "Rocket"; 
if(data == FM_Icon)       return RSAA;
if(data == FM_AmmoID)     return MIAM;
if(data == FM_AmmoLoad)   return 1;
if(data == FM_Reload)     return 36*20;
if(data == FM_Damage)     return 200;

return(Default(data));
}

public func BOTData2(int data)
{
if(data == BOT_Range)    return(500);
if(data == BOT_DmgType)  return(DMG_Projectile);

return(Default(data));
}

public func Fire2()
{
var missle = pCreator->CreateObject(LMIS,GetVertex(10,0,pCreator),GetVertex(10,1,pCreator),GetController(GetUser()));
var speed = Sqrt(GetXDir(pCreator)**2+GetYDir(pCreator)**2);
var angle = GetR(pCreator)-90+GetDir(pCreator)*180;
SetYDir(GetYDir(pCreator),missle);
SetXDir(GetXDir(pCreator),missle);
SetR(angle,missle);
ScheduleCall(missle,"Launch",16,0,angle,70,speed);
SetCursor(GetController(GetUser()),missle,true,true);
}

public func NoWeaponChoice()	{ return 1; }
