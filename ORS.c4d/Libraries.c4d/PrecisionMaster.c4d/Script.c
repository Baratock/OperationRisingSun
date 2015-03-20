//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PrecisionMaster ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#strict 2
local concentration;//aktuelle Konzentration des Clonks
local spreading;//aktuelle Streuung durch mangelnde Konzentration in Grad
local crosshair;//um eine fehlermeldung zu vermeiden
local Fired;//gibt an ob der Clonk gerade gefeuert hat

public func GetMaxConcentration(){return 100;}

public func GetSpreading()
{
var iSpr = (GetMaxConcentration()-concentration)/5;
if(Contents(0,this))if(Contents(0,this)->~HasLaser() && iSpr != 0)iSpr/=2;
return iSpr;
}

protected func Initialize()
{
	Fired=false;
	concentration=GetMaxConcentration();//Der Clonk beginnt mit voller Konzentration
  AddEffect("PrecisionMaster",this,1,1,this);//Der neue Effekt der den Clonk zittern lässt
  AddEffect("ConcentrationRegeneration",this,1,2,this);//Regeneration der Konzentration
  return _inherited();
}

//--------------------------------------------Effekte----------------------------------------------

public func FxPrecisionMasterTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  spreading = (GetMaxConcentration()-concentration)/5;//Streuung basierend auf der Konzentration des Clonks
	if(Contents(0,this))if(Contents(0,this)->~HasLaser() && spreading != 0)spreading/=2;
	if(Fired)spreading = RandomX(-spreading,spreading);
	else spreading = RandomX(-spreading/4,spreading/4);
}

public func FxConcentrationRegenerationStart (object pTarget, int iEffectNumber, int iTemp)
{
	EffectVar(0,this,iEffectNumber)=0;
}

public func FxConcentrationRegenerationTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	//einmal pro sekunde wird feuern auf false gesetzt
	if (EffectVar(0,this,iEffectNumber)==18)
	{
	Fired=false;
	EffectVar(0,this,iEffectNumber)=0;
	}
	else EffectVar(0,this,iEffectNumber)++;
	//Die Konzentration regeneriert sich
  concentration++;
  if(concentration > GetMaxConcentration())concentration=GetMaxConcentration();
}

//--------------------------------------Überladene Funktionen--------------------------------------

//Die Funtion wird vom Clonk überladen, um die Abweichung einstellen zu können
public func AimAngle(int iMaxAngle, int iRange) {
	var angle;
	var x,y,r;
	this->~WeaponAt(x,y,r);

	if(!this->~IsAiming())
		angle = (90+r)*(GetDir()*2-1);
	else
		angle = crosshair->GetAngle();
  
    if(iMaxAngle) {
		var target = this->~GetTarget(angle,iMaxAngle,iRange);
		if(target)
			angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
	}
	
	angle += spreading;//Streuung wird dem Winkel hinzuaddiert
	return(angle);
}

//Die Waffe soll beim Zielen wackeln
public func WeaponAt(&x, &y, &r)
{
  if(Contained()) return(0);

  var a = GetAction();
  var p = GetPhase();
  
  x=0; y=0; r=0;

  if(a == "WalkArmed") 
  {
    x=0;
    y=1000;
    return(1);
  }
  else if(a == "JumpArmed") 
  {
    x=500-125*p;
    y=1000;
    return(1);
  }
  else if(a == "SwimArmed") 
  {
    x=4000;
    return(1);
  }
  else if(a == "JetpackFlight") 
  {  
    x=4000;
    r=35;
    return(1);
  }

  else if(this->~IsAiming()) {
    r = (Abs(crosshair->GetAngle())-90)+spreading;//<----
    x=-Sin(65+p*20+spreading,4000)-1500;//<----  Einzige Änderungen der überladenen Funktion(Streuung hinzugerechnet)
    y=-Sin(55+p*10+spreading,5000);//<---- 
    if(a == "AimLow")
      y += 4000;
    if(a == "AimSquat")
    {
      x += 5000;
      y += 4500;
    }
    if(a == "AimSquatLow")
    {
      x += 5000;
      y += 6000;
    }
    return(1);
  }
return(0);
}

//Die Waffe soll auch beim Stehen wackeln
public func FxShowWeaponTimer(object pTarget, int iNumber, int iTime)
{
  var xoff, yoff, r;
  if(!Contents(0,pTarget)) {
    EffectVar(0, pTarget, iNumber) = 0;
    if(EffectVar(6, pTarget, iNumber)) {
      SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
      EffectVar(6, pTarget, iNumber) = 0;
    }
    SetGraphics(0, pTarget, 0, WeaponDrawLayer);
    return(FX_OK);
  }
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
  if(EffectVar(0, pTarget, iNumber) != id) {
    if(obj->~IsWeapon() || obj->~IsDrawable()) {
      EffectVar(0, pTarget, iNumber) = id;
      EffectVar(6, pTarget, iNumber) = obj;
      SetGraphics(0, pTarget,id, WeaponDrawLayer, GFXOV_MODE_Object,0,GFX_BLIT_Parent,obj);
    }
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
  var width, height; 
  var xskew, yskew;   
  var size;           
  var xaim, yaim;   
  var dir;          
  r += ObjectCall(obj,"HandR") + spreading;//Streuung dazurechnen <---- Einzige Änderung der überladenen Funktion
  width = height = xskew = yskew = 1;
  size = id->~HandSize();
  if(!size) size = 1000;
  dir  = GetDir()*2-1;
  if(r > 180 || r < -180)
    dir *= -1;
  r *= dir;
  var xfact = size * ObjectCall(obj,"HandX");
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
  SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, pTarget, WeaponDrawLayer);
  SetObjDrawTransform(width,xskew,0,yskew,height,0, obj);
  r = -r-90;
  var w = GetDefCoreVal("Width",0,id)/2;
  var brly = DefinitionCall(id,"BarrelYOffset");
  var brlx = DefinitionCall(id,"BarrelXOffset");
  var r2 = (Angle(0,0,w-brlx/1000,brly/1000)-90)*dir;
  var dist = Distance(0,0,w*1000-brlx,brly);
  EffectVar(1, pTarget, iNumber) = r;
  EffectVar(2, pTarget, iNumber) = xoff-Sin(r,size*w);
  EffectVar(3, pTarget, iNumber) = yoff+Cos(r,size*w);
  EffectVar(4, pTarget, iNumber) = xoff+Sin(r+r2,size*(dist))/1000;
  EffectVar(5, pTarget, iNumber) = yoff-Cos(r+r2,size*(dist))/1000;
}

//identifikationsfunktion
public func CanRealAim(){return true;}
