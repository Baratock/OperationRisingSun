//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Sichtbereich  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#appendto HZCK
local pView;//Sichtobjekt
local iViewDistance;//Sichtweite
local ViewOffset;

public func Initialize()
{
iViewDistance=800;
ViewOffset=0;
if(FindObject(R_V1))
	{
	pView=CreateObject(VIEW,0,0,GetController()); 
	AddEffect("ViewRange1",this,1,1,this);
	}
return _inherited();
}

public func FxViewRange1Timer(object pTarget, int iEffectNumber, int iEffectTime)
{
//wenn der Clonk woanders drin ist, muss sich die Sicht mitbewegen
var dir = -1+2*GetDir();
if(Contained())pView->AttachTo(Contained());
else pView->AttachTo(this);
//Sichtbarkeit nur, wenn der Clonk gerade ausgewählt ist
if(GetCursor(GetController())==this)SetVisibility(VIS_Owner,pView);
else SetVisibility(VIS_None,pView);
if(Contained())if(GetCategory(Contained()) & C4D_Vehicle)SetVisibility(VIS_None,pView);
//Winkel einstellen
if(this->~IsAiming())SetR(this->~GetWeaponR()+90,pView);
else if(this->~IsGRAiming())SetR(dir*this->~GrenadeAimAngle()-90+180,pView);
else SetR(GetDir()*180,pView);
//Größe nachjustieren
SichtWeite();
if(!this->~IsAiming())ViewOffset=0;
SetViewOffset(GetController(),ViewOffset,0);
SetObjDrawTransform (1000*4,0,0,0,1000*4,0,pView);
}

private func SichtWeite()
{
if(!Contents(0,this))
	iViewDistance=800;
else if(Contents(0,this)->~ScopeViewRange() && this->~IsAiming())
	iViewDistance=Contents(0,this)->~ScopeViewRange();
else 
	iViewDistance=800;
SetPlrViewRange(iViewDistance,this);
}

public func HasView(){return true;}
