//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Waffenobjekt ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include WEPN

//---------------Neue Funktionen, die in den Waffen benutzt werden k�nnen--------------------------

public func HasLaser()				{ return false; }//Wird beim Zielen ein Laserstrahl erzeugt?
public func LaserX()				{ return 0; 		}//Ver�ndert Ursprung des Laserstrahls
public func LaserY()				{ return 0; 		}//Ver�ndert Ursprung des Laserstrahls
public func HeadSound()				{ return 0; 		}//Gibt den Ton an der bei einem Kopfschuss abgespielt wird
public func IsSecondaryWeapon()		{ return false; }
public func ScopeViewRange()		{ return 0; 		}//Gibt den Bereich an, den man sehen kann, wenn man zielt
public func CantCrouchAim()			{ return false; }//Sagt ob Waffe beim Liegen nicht eingesetzt werden kann

//-------------------------------  Neue WaffenmodiOptionen  ---------------------------------------

//Diese neuen Daten werden von dem Projektil SHOT benutzt 
static const FM_Spreading    		=13;//Allgemeine Streuung
static const FM_Recoil				=14;//Gibt den R�cksto� beim Abschie�en der Waffe an
static const FM_HeadDamage   		=15;//Schaden in Prozent der bei einem Kopfschuss gemacht wird 
static const FM_PushingDistance 	=16;//Gibt an mit welcher Geschwindigkeit der Clonk bei einem Treffer geschleudert wird
static const FM_WeaponSound			=17;//Der Sound, der beim Schie�en abgespielt wird
static const FM_NoCrosshair			=18;//Falls kein Fadenkreuz angezeigt werden soll
static const FM_AmmoSlot			=19;

//------------------------------Neue Locale Variablen---------------------------------------
local aAmmo;
local aAmmoType;

public func Default(int data)    // Standardeinstellungen
{
  if(data == FM_Name)      				return "Standard";
  if(data == FM_AmmoID)    				return STAM;
  if(data == FM_AmmoUsage) 				return 1;
  if(data == FM_AmmoRate)  				return 1;
  if(data == FM_Auto)      				return false;
  if(data == FM_Aim)       				return 0;
  if(data == FM_Condition) 				return true;
  if(data == FM_AmmoLoad)  				return 5;
  if(data == FM_Reload)    				return 100;
  if(data == FM_Recharge)  				return 10;
  if(data == FM_Icon)      				return GetID();
  if(data == FM_Damage)    				return 0;
  if(data == FM_Spreading)				return 0;
  if(data == FM_HeadDamage)				return 100;
  if(data == FM_PushingDistance)		return 0;
  if(data == FM_Recoil)					return 0;
  if(data == FM_WeaponSound)			return 0;
  if(data == BOT_Range)    				return 100;
  if(data == BOT_DmgType)  				return DMG_Projectile;
  if(data == BOT_Ballistic)				return 0;
  if(data == FM_AmmoSlot)				return 1;
  if(data == BOT_Power)    				return BOT_Power_1;
  if(data == BOT_EMP)      				return false;
}

//--------------------------------------  Callbacks  ----------------------------------------------
public func OnRecharge(int i)
{

}
//---------------------------------------LaserPointer---------------------------------------------- 
local pLaserStrahl;

//beim zielen wird ein effekt gestartet und umgekehrt
public func OnAimStart()
{
	AddEffect("LaserPointer",this,18,1,this);
	return _inherited();
}

public func OnAimStop()
{
	RemoveEffect("LaserPointer",this);
	return _inherited();
}

protected func FxLaserPointerTimer (object pTarget, int iEffectNumber, int iEffectTime)
{
	if(!GetUser())return -1;//gibt es �berhaupt noch jemand der zielt und lebt?
	if(Contents(0,GetUser())->~HasLaser())//testet, ob die waffe einen laserpointer hat
	{
	//wenn nicht mehr gezielt wird, kann der effekt weg
	if(!GetUser()->IsAiming())return -1;
	//der alte laserstrahl wird entfernt falls er existiert
	_RemoveObject(pLaserStrahl);
	//daten der waffe werde abgefragt
	var user = Contained();
	var dir = GetDir(GetUser())*2-1;
	var angle = GetUser()->AimAngle();
	var x,y;  user->WeaponEnd(x,y);
	x+= Sin(angle,LaserX(),1);
	y+= Sin(angle*dir,LaserY(),1);
	if(dir==-1)y+= Sin(angle,2,1);
	//laserstrahl wird erzeugt
	pLaserStrahl = CreateObject(RSLS,x,y,GetController(GetUser()));
	pLaserStrahl -> Set(angle,2,LandscapeWidth(),0,0);
	}else return _RemoveObject(pLaserStrahl);//Der laserstrahl muss weg, wenn er mit einer waffe zielt, die keinen laserpointer hat
}

//wenn er aufh�rt zu zielen muss der strahl nat�rlich verschwinden
protected func FxLaserPointerStop (object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
	_RemoveObject(pLaserStrahl);
}


//---------------------------------Alte �berarbeitete Funktionen-----------------------------------

public func Initialize()
{
	aAmmo = CreateArray();
	aAmmoType = CreateArray();
	_inherited();
}

public func FxRechargeStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  // Waffentr�ger weg?
  if(!GetUser()) return(0);
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return(0);
  // automatisch weiterschie�en, mit JnR-Control auch bei normalen Waffen
  if(GetFMData(FM_Auto)) {
    // ... au�er wenn abgebrochen oder keine Munition mehr (!)
    if(stopauto || (GetUser()->Contents() != this() && GetUser() == Contained(this())) || !(GetUser()->ReadyToFire()) || !CheckAmmo(GetFMData(FM_AmmoID), GetFMData(FM_AmmoUsage))) {
      // Callback bei AutoFire
      if(GetFMData(FM_Auto))
        OnAutoStop(firemode);

      stopauto = false;
      shooting = false;
    }
    else {
      Shoot(pTarget);
    }
  }//neuer Callback
  if(GetCharge())OnRecharge(GetFireMode());
}

private func Shoot(object caller) {          // Feuern mit Feuermodus
	if(LocalN("Fired",GetUser()))LocalN("Fired",GetUser())=true;
  Concentration();//<---- Konzentration f�r den Precisionmaster einstellen
  if(!GetUser())
    return(stopauto=true);

  var ammoid = GetFMData(FM_AmmoID);
  //soll er zielen, zielt aber nicht?
  if(!(GetUser()->~IsAiming()) && GetFMData(FM_Aim)>0 && !(GetUser()->~AimOverride()))
    return(stopauto=true); //abbrechen
  // user hat sich irgendwo reinbewegt 
  if(GetUser()->Contained() && GetUser() == Contained())
    return(stopauto=true);
  // Feuern...
  Call(Format("Fire%d",firemode));
  if(GetFMData(FM_Auto)) shooting = true;
  //Wenn der Waffensound noch gespielt wird, wird er abgebrochen
  Sound(GetFMData(FM_WeaponSound),0,this,100,0,-1);
	//DerSound muss mit einer verz�gerung abgespielt werden, damit er �berhaupt abgespielt wird
  ScheduleCall(this,"WeaponSound",2,0);

  if(!GetUser()) return 0;
	
  // Munition abziehen
  if(ratecount == 1) {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-muni);
    ratecount = GetFMData(FM_AmmoRate);
  }
  else { --ratecount; }

  // Pause nach einem Schuss
  if(GetAmmo(ammoid)) Recharge();
  else {
    shooting = false;
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    // Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedr�ckt ist
    if(GetPlrCoreJumpAndRunControl(GetUser()->GetController()))
    	if(!IsReloading())
      	ScheduleCall(this(), "Reload", 5);
  }
  // HZCK soll Munition doch bitte neu anschauen
  GetUser()->~UpdateCharge();
}

public func FxReloadStart(object pTarget, int iNumber, int iTemp, int iTime)
{
  if(iTemp) return 0;
  EffectVar(1,pTarget,iNumber) = 1;
  EffectVar(2,pTarget,iNumber) = iTime*2;//<--Gesamtzeit wird verdoppelt
}

public func FxReloadTimer(object pTarget, int iNumber, int iTime) //Bei einigen Aktionen l�dt der Clonk nur halb so schnell bei anderen gar nicht
{
  if(EffectVar(1,pTarget,iNumber))
  {
    GetUser()->~UpdateCharge();
    if(//Bei diesen Aktionen l�dt der Clonk mit 50% mehr Geschwindigkeit
    GetUser()->~IsCrawling()||
    GetAction(GetUser())=="AimSquat"||
    GetAction(GetUser())=="AimSquatLow"
    )EffectVar(0,pTarget,iNumber)+= 3;
    else if(//Bei diesen Aktionen l�dt der Clonk mit voller Geschwindigkeit
    GetAction(GetUser())=="Walk"||
    GetAction(GetUser())=="WalkArmed"||
    GetAction(GetUser())=="Aim"||
    GetAction(GetUser())=="AimLow"
    )EffectVar(0,pTarget,iNumber)+= 2;
    else if(//Bei diesen Aktionen l�dt der Clonk mit halber Geschwindigkeit
    GetAction(GetUser())=="JumpArmed"||
    GetAction(GetUser())=="Jump"
    )EffectVar(0,pTarget,iNumber)++;
    //Bei allen anderen Aktionen l�dt der Clonk gar nicht weiter nach
    if(EffectVar(0,pTarget,iNumber) >= EffectVar(2,pTarget,iNumber)) return -1;
  }
}

public func GetCharge() 
{
  var charge;
  var ammoid = GetFMData(FM_AmmoID);
  // l�dt nach: Nachladestatus anzeigen
  if(IsReloading()) {
    // Nachladestatus in %
    charge = 1000*(EffectVar(0, this(), GetEffect("Reload", this())))/GetFMData(FM_Reload)/2;
  }
  // ansonsten: Ladestand anzeigen
  else {
    charge = 1000*GetAmmo(ammoid)/GetFMData(FM_AmmoLoad);
  }

  return(charge);
}

public func Fire()
{

  // bereit zum Schie�en
  if(!(GetUser()->~ReadyToFire())) return(false); 
  var xb,yb,xe,ye;
  //K�rper �ber Callback abfragen falls besondere Zielpose
	GetUser()->~GetBody(xb, yb);
	if(!xb && !yb)
	{
		xb=GetX(GetUser());
		yb=GetY(GetUser());
	}
	GetUser()->~WeaponEnd(xe,ye);
	if(xb && yb && xe && ye)
		{
		xe+=GetX(GetUser());
		ye+=GetY(GetUser());
		if(!PathFree(xb,yb,xe,ye))return false;
		}

  // l�dt grad nach... bitte nicht st�ren
  if(IsReloading()) return(false);

  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);
  // leer?
  if(!CheckAmmo(ammoid,ammousage,this())) {
    OnEmpty(firemode);
    return(false);
  }

  // Callback
  if(GetFMData(FM_Auto))
    OnAutoStart(firemode);

  // FEUAAA!!!1 OMGOMG
  Shoot();

  return(true);
}

//Winkelanzeige optimieren
global func FxShowWeaponTimer(object pTarget, int iNumber, int iTime)
{
  // Waffe aktualisieren:
  var xoff, yoff, r;  // Offset, Winkel
  // kein Inventar oder falsche Aktion
  if(!Contents(0,pTarget)) {
    EffectVar(0, pTarget, iNumber) = 0;
    if(EffectVar(6, pTarget, iNumber)) {
      SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
      EffectVar(6, pTarget, iNumber) = 0;
    }
    SetGraphics(0, pTarget, 0, WeaponDrawLayer);
    return(FX_OK);
  }
  //Die Waffe momentan �berhaupt anzeigen?
  if(!(pTarget->~WeaponAt(xoff, yoff, r))) {
    EffectVar(0, pTarget, iNumber) = 0;
    if(EffectVar(6, pTarget, iNumber)) {
      SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
      EffectVar(6, pTarget, iNumber) = 0;
    }
    SetGraphics(0, pTarget, 0, WeaponDrawLayer);
    return(FX_OK);
  }
  var obj = Contents(0,pTarget), id=GetID(obj);
  // Waffe nicht mehr aktuell
  if(EffectVar(0, pTarget, iNumber) != id) {
    // neues Objekt ist Waffe, oder ein Objekt, das gezeichnet werden soll
    if(obj->~IsWeapon() || obj->~IsDrawable()) {
      EffectVar(0, pTarget, iNumber) = id;
      EffectVar(6, pTarget, iNumber) = obj;
      SetGraphics(0, pTarget,id, WeaponDrawLayer, GFXOV_MODE_Object,0,GFX_BLIT_Parent,obj);
    }
    // neues Objekt ist keine Waffe
    else {
      EffectVar(0, pTarget, iNumber) = 0;
      if(EffectVar(6, pTarget, iNumber)) {
        SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
        EffectVar(6, pTarget, iNumber) = 0;
      }
      SetGraphics(0, pTarget, 0, WeaponDrawLayer);
      return(FX_OK);
    }
  }

  id = EffectVar(0, pTarget, iNumber);
  obj = EffectVar(6, pTarget, iNumber);

  // Ausrichtung nach Blickrichtung des Clonks
  // Variablen f�r die Transformation

  var width, height;  // Breiten- und H�henverzerrung der Waffe
  var xskew, yskew;   // Zerrung der Waffe, wird zur Rotation gebraucht
  var size;           // Gr��e der Waffe in der Hand: 1000 = 100%
  // Variablen f�r die Position
  var xaim, yaim;     // Offset, dass sich durch zielen ergibt
  var dir;            // Richtung in die das Objekt schaut
  
  //schnell noch Rotation dazurechnen oder so!
  r += ObjectCall(obj,"HandR");
  
  // Variablen mit Werten versehen
  width = height = xskew = yskew = 1;
  size = id->~HandSize();
  if(!size) size = 1000;
  dir  = GetDir()*2-1;
  if(r > 180 || r < -180)
    dir *= -1;
  r *= dir;

  var xfact = size * ObjectCall(obj,"HandX");    // Attachpunkte dazurechnen
  var yfact = size * ObjectCall(obj,"HandY");

  xoff += Cos(r,xfact)/1000 + dir*Sin(r,yfact)/1000;
  yoff -= Cos(r,yfact)/1000 - dir*Sin(r,xfact)/1000;

  if(dir == 1) 
  {
    height = -1;
    xskew = -1;
    yskew = -1;
  }

  r = -90*dir-r-90;
  height *= width *= Cos(r, size);
  xskew *= Sin(r, size);
  yskew *= -xskew;
  xoff *= dir;

  SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, pTarget, WeaponDrawLayer); //position
  SetObjDrawTransform(width,xskew,0,yskew,height,0, obj); //Gr��e und Rotation

  // abspeichern, damit abrufbar
  r = -r-90;
  var w = GetDefCoreVal("Width",0,id)/2;
  var brly = DefinitionCall(id,"BarrelYOffset");
  var brlx = DefinitionCall(id,"BarrelXOffset");
  var r2 = (Angle(0,0,w-brlx/1000,brly/1000)-90)*dir;
  var dist = Distance(0,0,w*1000-brlx,brly);
  //Log("%d - %d - %d - %d",w,brl,r2,dist);
  EffectVar(1, pTarget, iNumber) = r;
  EffectVar(2, pTarget, iNumber) = xoff-Sin(r,size*w);
  EffectVar(3, pTarget, iNumber) = yoff+Cos(r,size*w);
  EffectVar(4, pTarget, iNumber) = xoff+Sin(r+r2,size*(dist))/1000;
  EffectVar(5, pTarget, iNumber) = yoff-Cos(r+r2,size*(dist))/1000;
  //EffectVar(4, pTarget, iNumber) = xoff+Sin(r,size*(w));	
  //EffectVar(5, pTarget, iNumber) = yoff-Cos(r,size*(w));
  //Log("%d / %d",EffectVar(4, pTarget, iNumber),EffectVar(5, pTarget, iNumber));
}


//---------------------------------------  Waffensound  -------------------------------------------

private func WeaponSound()
{
Sound(GetFMData(FM_WeaponSound),0,this,100,0,0);
}

//-------------------------Konzentration f�r den Precisionmaster einstellen------------------------

private func Concentration()
{
var user = GetUser();
var maxConcentration= user->~GetMaxConcentration();
if(GetFMData(FM_Recoil) && LocalN("concentration",user))
	{
	LocalN("concentration",user)-=PostureValue(GetFMData(FM_Recoil),GetFMData(FM_Recoil)/2,GetFMData(FM_Recoil)/3);
	if(LocalN("concentration",user) < 1)LocalN("concentration",user)=1;
	if(LocalN("concentration",user) > maxConcentration)LocalN("concentration",user)=maxConcentration;
	}
}

//-------------------------------Rotation beim Aufsammeln entfernen--------------------------------
public func Entrance()
{
SetR(0);
}
//----------------------------------------Andere Funktionen----------------------------------------
//Feuermodi
private func ChangeFireMode(dummy, i)
{
  // Immer noch gen�gend Munition da?
  if(!CheckAmmo(GetFMData(FM_AmmoID, i),GetFMData(FM_AmmoUsage,i),GetUser(),this()))
  {
    // Nein. Mecker.
    Failure(dummy, i);
    // Men� will geupdatet werden, wahrscheinlich
    var item=GetMenuSelection(GetUser());
    CloseMenu(GetUser());
    ControlDigDouble(GetUser());
    SelectMenuItem (item, GetUser());
    // Fertig.
    return; 
  }
  
  CloseMenu(GetUser());
  var old = GetFireMode();
  // Schussmodus umstellen
  if(SetFireMode(i))
  {
    OnAutoStop(old);
    // Danach neue Munition einladen.
    //Reload();  //auch hier nein
  }
}

private func SetFireMode(int i) {
  if(i == 0) return;

  // Gleicher Modus: Nur nachladen wenn nicht mehr voll und l�dt nicht nach
  if(i == firemode) {
  	Reload();
    if(CheckAmmo(GetFMData(FM_AmmoID, i),GetFMData(FM_AmmoLoad),this())) return; 
    if(IsReloading()) return;
  }
  // Alte Munition ausladen
  //Empty(); //Nope behalten
  
  // Schussmodus umstellen aber vorher nachladen unterbrechen
  CancelReload();
  firemode=i;
  stopauto=false;
  ratecount = GetFMData(FM_AmmoRate, i);
  
  // Helpmessage
  if(GetUser())
    HelpMessage(GetUser()->GetOwner(),"$FireModeChanged$",GetUser(),GetFMData(FM_Name),GetFMData(FM_AmmoID));
  
  return 1;
}

public func ChangeToNextFM()
{
	var aFM = [];
	var i;// we still need the count after checking this
	
	//note all Firemodes
	for(i = 1; GetFMData(FM_Name, i); i++ )
		if(GetFMData(FM_Condition, i))
			aFM[i-1] = true;
	//use the previos as Module for the array		
	i--;
	var f = GetFireMode()-1;
	while(!aFM[f=(++f%i)]);
	f++;
	ChangeFireMode(GetUser(), f);
	return f;	
}

//WeaponMenu
public func ControlDigDouble(caller)
{
	SetUser(caller);
	//Check amount of FireModes
	var j;
	for(var i = 1; GetFMData(FM_Name, i); i++ )
		if(GetFMData(FM_Condition, i))
			j++;
	//Log("%v", j);
	var menu = CreateQuickMenu(this, caller);
	if(j > 1)
		menu->Add(QMEN_Up, 0, QMBG, "ChangeToNextFM", 0, 0);
	if(GetAmmoCount())
		menu->Add(QMEN_Down, 0, QMBG, "Empty", 0, "Empty");
	return true;
}


//Empty Weapon
public func Empty()	{ Empty2(GetSlot());	}

public func Empty2(int slot)    // Waffe ausleeren
{
	// Laden wir nach? Abbrechen.
	if(IsReloading()) RemoveEffect("Reload", this);
	if(IsRecharging()) RemoveEffect("Recharge", this);

	// Munitionsart wo raus muss
	var ammoid;
	if(slot)
		ammoid = GetAmmoType(slot);
	else
	{
		slot = GetSlot();
		ammoid = GetFMData(FM_AmmoID);
	}
	
	// Zu schiebende Munitionsmenge
	var ammoamount = GetAmmo(ammoid, this());
	// Clonk wiedergeben 
	DoAmmo(ammoid, ammoamount, GetUser());
	// Hier entfernen
	DoAmmo2(slot, ammoid, -ammoamount, this());
}

//Ammo

//---Munitionstyp---
public func GetCurrentAmmoType()	{	return GetAmmoType(GetSlot());	}
public func GetAmmoType(int slot)	{	return aAmmoType[slot-1];		}

public func SetAmmoType(int slot, id ammoid)
{
	aAmmoType[slot-1] = ammoid; 
}

//---Munitionsmenge---
public func SetAmmoCount(int slot, int amount)
{
	aAmmo[slot-1] = amount;
}

public func GetAmmoCount(int slot)	{	return aAmmo[slot-1];	}
public func GetCurrentAmmoCount()	{	return GetAmmoCount(GetSlot());	}

//---Munitionsslot---
public func GetSlot(int FM)
{
	//Get FM data for slot if no FM given use current one
	var i = GetFMData(FM_AmmoSlot, FM);
	if(i < 1)
		i = 1;
	return i;
}
public func GetSlotCount()			{	return GetLength(aAmmo);		}


//---Globals---
global func DoAmmo(id ammoid, int change, object target)
{
	if(!target)	target = this;
	if(target->~IsWeapon2())
		return DoAmmo2(target->GetSlot(), ammoid, change, target);
	else
		return _inherited(ammoid, change, target);
}

global func DoAmmo2(int slot, id ammoid, int change, object target)
{ 
	// gar keine Munition
	if(!(ammoid->~IsAmmo())) return false;
	
	// Kann 0 sein bei Objektlokalen Aufrufen.
	if(!target) target=this;
  
  	// Entsprechendes Munitionslagerobjekt suchen
  	var obj = target ->~ AmmoStoring();
  	if(!obj) obj = target;
  
  	// no ammo rule
  	if(ObjectCount(NOAM))
    	if(obj ->~ IsAmmoStorage())
			return false;
      
	// Alten Munitionsstand speichern
	var oldammoamount = obj->GetAmmo(ammoid);
	var truechange;
	var maxamount = ammoid->~MaxAmount();
	// Der neue Wert wird ausgerechnet, darf aber nicht gr��er als eventuelles MaxAmount() und nicht kleiner als 0 sein
	if(maxamount == 0)
		truechange= Max(oldammoamount+change, 0);
	else
		truechange= Max(Min(maxamount, oldammoamount+change), 0);
  
	var maxammo = target->~GetFMData(FM_AmmoLoad);
	if(maxammo)
		truechange = Min(maxammo, truechange);
  	
	// Neuer Wert dem Objekt geben, in Lokale ammoamount
	target->~SetAmmoCount(slot, truechange);
	target->~SetAmmoType(slot, ammoid);
  
	// Wenn Muni alle, Objekt entfernen
	if(truechange == 0) 
	  	target->~SetAmmoType(slot, 0);
	// Differenz zur�ckgeben: Tats�chliche �nderung.
	return truechange-oldammoamount;
}

global func GetAmmo(id ammoid, object target)
{
	if(!target)	target = this;
	if(target->~IsWeapon2())
		return GetAmmo2(target->GetSlot(), target);
	else
		return _inherited(ammoid,  target);
}

global func GetAmmo2(int slot, object target)
{	
	// Kann 0 sein bei Objektlokalen Aufrufen.
	if(!target) target=this;
	// Entsprechendes Munitionslagerobjekt suchen
	var obj = target ->~ AmmoStoring();
	if(!obj) obj = target;
	// no ammo rule
	if(ObjectCount(NOAM))
		if(obj ->~ IsAmmoStorage())
			return 0;
	return target->~GetAmmoCount(slot);
}

global func CheckAmmo2(int slot, id ammoid, int count, object target) {

 	if(!ammoid) return false;
	if(!target->~isWeapon2())	return false;
	// gar keine Munition
	if(!(ammoid->~IsAmmo())) return false;

	if(!target) target = this;
	Log("%v: %v, %v", ammoid, count, target);
    // no ammo rule
	var obj = target ->~ AmmoStoring();
	if(!obj) obj = target;
	if(ObjectCount(NOAM))
		if(obj ->~ IsAmmoStorage())
			return true;
	if(target->~AmmoType(slot))
		return false;
	if(GetAmmo2(slot, target) >= count)
		return true;
	return false;
}



//gibt beim ligen den dritten,knien den zweiten und sonst den ersten wert aus 
public func PostureValue(stehend, knieend, liegend)
{
if(GetUser())
	{
	if(GetUser()->~IsCrawling())
		return liegend;
	if(	GetAction(GetUser())=="AimSquat" || 
			GetAction(GetUser())=="AimSquatLow")
		return knieend;
	}	
	return stehend;
}

//waffengrundobjekt darf im waffenmen� nicht ausw�hlbar sein
public func NoWeaponChoice()	{ return GetID() == ORSW; }
public func IsWeapon2()			{ return true;	}
