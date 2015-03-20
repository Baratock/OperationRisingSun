//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ RPG-7 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include ORSW

local iTimer;

public func HandSize() 			{ return 850; 								}
public func HandX()    			{ return PostureValue(6000,-1000);}
public func HandY()    			{ return PostureValue(-2000,0000);}
public func BarrelXOffset()	{ return 0; 									}
public func BarrelYOffset()	{ return 0; 									}
public func CantCrouchAim() { return true; }//Sagt ob Waffe beim Liegen nicht eingesetzt werden kann

public func Initialize()
{
	AddEffect("Munition",this(),17,1,this());
	return (_inherited());
}

public func OnReload(int i)
{
Sound("rocket_reload");
}

public func FMData1(int data)
{
	if(data == FM_Name)      	return "Panzerbrechende Granate";
	if(data == FM_Icon)      	return RSAG;
	if(data == FM_AmmoID)    	return MIAM;
	if(data == FM_AmmoLoad)  	return 1;
	if(data == FM_Reload)    	return 36*8;
	if(data == FM_Recharge)  	return 0;
	if(data == FM_Aim)       	return 1;
	if(data == FM_Damage)    	return 50;
	if(data == FM_WeaponSound)return "RPG_shot";
	return Default(data);
}

public func Fire1()
{
var user = Contained();
var dir = GetDir(GetUser())*2-1;var angle = GetUser()->AimAngle();
var x,y;
GetUser()->WeaponEnd(x,y);
var ammo = CreateObject(RSAG,x,y,GetController(user));
ammo->Launch(angle,GetFMData(FM_Damage), 50);
SoundLevel("RaketenFlug",80,ammo);
RauchErzeugen();
}

public func FMData2(int data)
{
	if(data == FM_Name)      	return "Homing-Missle";
	if(data == FM_Icon)      	return RSTM;
	if(data == FM_AmmoID)    	return MIAM;
	if(data == FM_AmmoLoad)  	return 1;
	if(data == FM_Reload)    	return 10*36;
	if(data == FM_Recharge)  	return 0;
	if(data == FM_Aim)       	return 1;
	if(data == FM_Damage)    	return 50;
	if(data == FM_WeaponSound)return "RPG_shot";
	return Default(data);
}

public func Fire2()
{
var user = GetUser();
var dir = GetDir(GetUser())*2-1;
var angle = GetUser()->AimAngle();
var x,y;
GetUser()->WeaponEnd(x,y);
var ammo = CreateObject(RSTM,x,y,GetController(user));
ammo->~SetTimer(iTimer);
ammo->Launch(angle,GetFMData(FM_Damage), 50);
SoundLevel("RaketenFlug",80,ammo);
RauchErzeugen();
}

public func ControlDigDouble(caller)
{
  SetUser(caller);
	if(GetFMData(FM_Name)!="Homing-Missle")Feuermodimenu();
	else
	{
	CreateMenu(GetID(), GetUser(), 0, 0, "Aktionen");
	AddMenuItem("Suchcounter", "SetTimerMenu", GetID(this()), GetUser()); 
	AddMenuItem("Feuermodi", "Feuermodimenu", GetID(this()), GetUser()); 
	}
  
  return true;
}

private func SetTimer(dummy,int iTime){ iTimer=iTime; }

public func SetTimerMenu()
{
CreateMenu(GetID(), GetUser(), 0, 0, "Timer einstellen",0,1);
AddMenuItem("1/4 Sekunde", "SetTimer", GetID(this()), GetUser(),0,9);
AddMenuItem("1/2 Sekunde", "SetTimer", GetID(this()), GetUser(),0,18);
AddMenuItem("3/4 Sekunde", "SetTimer", GetID(this()), GetUser(),0,27);
AddMenuItem("1 Sekunde", "SetTimer", GetID(this()), GetUser(),0,36);
}

public func Feuermodimenu()
{
var name;
  // Das Menü für die Schussmodiauswahl
  CreateMenu(GetID(), GetUser(), 0, 0, "$FireMode$", C4MN_Extra_Info, 0, true);
  // Solang suchen, bis keine Schussmodi mehr da sind
  for(var i = 1; name=GetFMData(FM_Name, i); i++ )
  {
    // Vorraussetzung muss erfüllt sein
    if(GetFMData(FM_Condition, i))
    {
      var id = GetFMData(FM_AmmoID, i);
      var iconid = GetFMData(FM_Icon, i);
      var obj = CreateObject(iconid);
      // Wir haben ein Objekt fürs Menü und legen die Ammografik drauf
      SetGraphics(0, obj, id, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(650,0,5000,0,650,5000, obj, 1);
      
      // Eintragstext
      var entryname=Format("%s (%s)", name, GetName(0, id));
      
      // Genug Ammo für zumindest einen Schuss vorhanden sein
      if(!CheckAmmo(id,GetFMData(FM_AmmoUsage,i),GetUser(),this()))
      {
        // Name des Eintrags wird grau
        entryname=Format("<c %x>%s</c>", RGB(100,100,100), entryname);
        // Eintrag grau
        obj->SetClrModulation(RGB(100,100,100));
      }
      // Eintrag hinzufügen.
      AddMenuItem(entryname, "ChangeFireMode", GetID(this()), GetUser(), 0, i, Format("$DescFireMode$", name), 4, obj); 
      // Objekte natürlich wieder löschen
      RemoveObject(obj);
    }
  }
}

public func FxMunitionTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if(pTarget->GetCharge()< 1000)SetGraphics("Unloaded");
	else SetGraphics("");
}

public func RauchErzeugen()
{
	var x,y,dy,dx,r;
	GetUser()->WeaponBegin(x,y);
	GetUser()->WeaponAt(dy,dx,r);
	if(GetDir(GetUser()))r-=180;
	if(!GetDir(GetUser()))r*=-1;
	var dir = GetDir(GetUser())*2-1;
	for(var i=0;i <= 30;i++)
	{	
		var randomD = RandomX(4,20);
		var randomR = RandomX(-40,40);
		var xDir = Cos(r+randomR,randomD);
		var yDir = Sin(r+randomR,randomD);
  	CreateParticle("AbluftRauch",x,y,xDir,yDir,80,RGBa(255, 255, 255, 100));
  }
  return 1;
}
