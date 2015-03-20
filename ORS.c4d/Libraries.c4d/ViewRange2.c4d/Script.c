//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Sichtbereich  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#appendto HZCK
local iViewDistance;//Sichtweite
local iPrecision;//Maßabstände

public func Initialize()
{
iPrecision=50;
if(FindObject(R_V2))
	AddEffect("ViewRange2",this,2,1,this);
return _inherited();
}

public func FxViewRange2Timer(object pTarget, int iEffectNumber, int iEffectTime)
{
SetViewDistance();
//Sicht vom Clonk verschwinden lassen
SetFoW(true,GetController());
SetPlrViewRange(0,this);
//Winkel finden
var relX,relY,iAngle;
var dir = -1+2*GetDir();
if(this->~IsAiming())iAngle=this->~GetWeaponR()-90;
else if(this->~IsGRAiming())iAngle=dir*this->~GrenadeAimAngle()-90;
else iAngle=!GetDir()*180;
relX=Cos(iAngle,iPrecision,1);
relY=Sin(iAngle,iPrecision,1);
if(FindObject(R_V1))
{
for(var i=0;i<(iViewDistance/iPrecision);i++)
	{
	if(i==0)
		SetPlrViewRange(10);
	else if(!GBackSolid(i*relX,i*relY) && PathFree(GetX()+i*relX,GetY()+i*relY,GetX()+(i-1)*relX,GetY()+(i-1)*relY))
		SetPlrViewRange(i*iPrecision+40);
	else break;
	}
}else
for(var i=0;i<(iViewDistance/iPrecision);i++)
	{
	//Mindestens 1 Sichtobjekt erstellen
	if(i==0)
		var sight=CreateObject(SIGT,i*relX,i*relY,GetController(this));
	//Sichtobjektkette geht nicht durch Material
	else if(!GBackSolid(i*relX,i*relY) && PathFree(GetX()+i*relX,GetY()+i*relY,GetX()+(i-1)*relX,GetY()+(i-1)*relY))
		var sight=CreateObject(SIGT,i*relX,i*relY,GetController(this));
	//sonst endet die Sichtkette vollständig
	else 
		break;
	//Objekt als Sicht für Clonk einstellen
	SetPlrViewRange(1,sight);
	SetVisibility(VIS_None,sight);
	}
}

private func SetViewDistance()
{
//Sichtweite einstellen
if(!Contents(0,this))
	iViewDistance=800;
else if(Contents(0,this)->~ScopeViewRange() && this->~IsAiming())
	iViewDistance=Contents(0,this)->~ScopeViewRange();
else 
	iViewDistance=800;
}

public func HasView(){return true;}
