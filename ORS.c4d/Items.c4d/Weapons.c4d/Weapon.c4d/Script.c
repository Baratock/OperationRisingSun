//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Waffenobjekt ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include WEPN

//---------------Neue Funktionen, die in den Waffen benutzt werden können--------------------------

public func HasLaser()					{ return false; }//Wird beim Zielen ein Laserstrahl erzeugt?
public func LaserX()						{ return 0; 		}//Verändert Ursprung des Laserstrahls
public func LaserY()						{ return 0; 		}//Verändert Ursprung des Laserstrahls
public func HeadSound()					{ return 0; 		}//Gibt den Ton an der bei einem Kopfschuss abgespielt wird
public func IsSecondaryWeapon()	{ return false; }
public func ScopeViewRange()		{ return 0; 		}//Gibt den Bereich an, den man sehen kann, wenn man zielt
public func CantCrouchAim()			{ return false; }//Sagt ob Waffe beim Liegen nicht eingesetzt werden kann

//-------------------------------  Neue WaffenmodiOptionen  ---------------------------------------

//Diese neuen Daten werden von dem Projektil SHOT benutzt 
static const FM_Spreading    		=13;//Allgemeine Streuung
static const FM_Recoil					=14;//Gibt den Rückstoß beim Abschießen der Waffe an
static const FM_HeadDamage   		=15;//Schaden in Prozent der bei einem Kopfschuss gemacht wird 
static const FM_PushingDistance =16;//Gibt an mit welcher Geschwindigkeit der Clonk bei einem Treffer geschleudert wird
static const FM_WeaponSound			=17;//Der Sound, der beim Schießen abgespielt wird

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
  if(data == FM_PushingDistance)	return 0;
  if(data == FM_Recoil)						return 0;
  if(data == FM_WeaponSound)			return 0;
  if(data == BOT_Range)    				return 100;
  if(data == BOT_DmgType)  				return DMG_Projectile;
  if(data == BOT_Ballistic)				return 0;
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
	if(!GetUser())return -1;//gibt es überhaupt noch jemand der zielt und lebt?
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

//wenn er aufhört zu zielen muss der strahl natürlich verschwinden
protected func FxLaserPointerStop (object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
	_RemoveObject(pLaserStrahl);
}


//---------------------------------Alte überarbeitete Funktionen-----------------------------------

public func FxRechargeStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  // Waffenträger weg?
  if(!GetUser()) return(0);
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return(0);
  // automatisch weiterschießen, mit JnR-Control auch bei normalen Waffen
  if(GetFMData(FM_Auto)) {
    // ... außer wenn abgebrochen oder keine Munition mehr (!)
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
  Concentration();//<---- Konzentration für den Precisionmaster einstellen
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
	//DerSound muss mit einer verzögerung abgespielt werden, damit er überhaupt abgespielt wird
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

    // Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
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

public func FxReloadTimer(object pTarget, int iNumber, int iTime) //Bei einigen Aktionen lädt der Clonk nur halb so schnell bei anderen gar nicht
{
  if(EffectVar(1,pTarget,iNumber))
  {
    GetUser()->~UpdateCharge();
    if(//Bei diesen Aktionen lädt der Clonk mit 50% mehr Geschwindigkeit
    GetUser()->~IsCrawling()||
    GetAction(GetUser())=="AimSquat"||
    GetAction(GetUser())=="AimSquatLow"
    )EffectVar(0,pTarget,iNumber)+= 3;
    else if(//Bei diesen Aktionen lädt der Clonk mit voller Geschwindigkeit
    GetAction(GetUser())=="Walk"||
    GetAction(GetUser())=="WalkArmed"||
    GetAction(GetUser())=="Aim"||
    GetAction(GetUser())=="AimLow"
    )EffectVar(0,pTarget,iNumber)+= 2;
    else if(//Bei diesen Aktionen lädt der Clonk mit halber Geschwindigkeit
    GetAction(GetUser())=="JumpArmed"||
    GetAction(GetUser())=="Jump"
    )EffectVar(0,pTarget,iNumber)++;
    //Bei allen anderen Aktionen lädt der Clonk gar nicht weiter nach
    if(EffectVar(0,pTarget,iNumber) >= EffectVar(2,pTarget,iNumber)) return -1;
  }
}

public func GetCharge() 
{
  var charge;
  var ammoid = GetFMData(FM_AmmoID);
  // lädt nach: Nachladestatus anzeigen
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

  // bereit zum Schießen
  if(!(GetUser()->~ReadyToFire())) return(false); 
  var xb,yb,xe,ye;
  //Körper über Callback abfragen falls besondere Zielpose
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

  // lädt grad nach... bitte nicht stören
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
  //Die Waffe momentan überhaupt anzeigen?
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
  // Variablen für die Transformation

  var width, height;  // Breiten- und Höhenverzerrung der Waffe
  var xskew, yskew;   // Zerrung der Waffe, wird zur Rotation gebraucht
  var size;           // Größe der Waffe in der Hand: 1000 = 100%
  // Variablen für die Position
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
  SetObjDrawTransform(width,xskew,0,yskew,height,0, obj); //Größe und Rotation

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

//-------------------------Konzentration für den Precisionmaster einstellen------------------------

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

//waffengrundobjekt darf im waffenmenü nicht auswählbar sein
public func NoWeaponChoice()	{ return GetID() == ORSW; }
