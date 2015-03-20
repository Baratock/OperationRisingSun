//---------------------------------------  Phoenix AH-64  -----------------------------------------

#strict 2
#include WEPN

public func NoWeaponChoice() { return true; }

//----------------------------------------Lokale Variablen-----------------------------------------
local iComX;
local iComY;
local iTurnSpeed;
local Pilot;
local Gunner;
local Geschuetz;
local Entrance;
local hud;

func Initialize() {
	Entrance = CreateObject(P_EN,GetVertex(8,0),GetVertex(8,1));
	Entrance->AttachTo(this);
	Entrance->SetActionData(8);
	Geschuetz = CreateObject(P_GN,GetVertex(7,0),GetVertex(7,1));
	Geschuetz->AttachTo(this);
  SetComDir(COMD_Stop);
  SetAction("Wait");
  iComY = 0;
  iComX = 0;
  Pilot = 0;
  Gunner = 0;
  hud=0;
//----------------------------------------------Waffen---------------------------------------------
  firemode = 1;
  ratecount = GetFMData(FM_AmmoRate);
  stopauto = false;
  upgrades = CreateArray();
  SetUser();
  return 1;
}

//------------------------------------------Die Steuerung------------------------------------------

private func SetGeschwindigkeit(int comdir, &vx, &vy)//für einen flüssigen Geschwindigkeitswechsel
{
  vx = vy = 0;
  var max = GetPhysical("Float", 0, this);
  if         (comdir == COMD_Up)             { vy = -max; }
  else if(comdir == COMD_UpLeft)     { vy = -max; vx = -max; }
  else if(comdir == COMD_Left)       { vx = -max; }
  else if(comdir == COMD_DownLeft) { vy =  max; vx = -max; }
  else if(comdir == COMD_Down)       { vy =  max; }
  else if(comdir == COMD_DownRight){ vy =  max; vx =  max; }
  else if(comdir == COMD_Right)      { vx =  max; }
  else if(comdir == COMD_UpRight)  { vy = -max; vx =  max; }
  else return 0;
  return 1;
}

private func FxIntJnRBewegungStart(object target, int number, int temp, int vx, int vy)
{
  if(temp) return;
  EffectVar(0, target, number) = vx;
  EffectVar(1, target, number) = vy;
}

private func FxIntJnRBewegungTimer(object target, int number, int time)
{
  if(!Fliegt()) return -1;

  var dest_vx = EffectVar(0, target, number);
  var dest_vy = EffectVar(1, target, number);

  iComX = BoundBy(dest_vx - GetXDir(0, 100), -GetMaxSpeed(), GetMaxSpeed());
  iComY = BoundBy(dest_vy - GetYDir(0, 100), -GetMaxSpeed(), GetMaxSpeed());

  if(iComX == 0 && iComY == 0) return -1;
}

public func ControlUpdate(object controller, int comdir)
{
  if(Fliegt())
  {
    var dest_vx, dest_vy;
    SetGeschwindigkeit(comdir, dest_vx, dest_vy);
    if(dest_vx == 0) dest_vx = 50 * Positiv(GetXDir(0, 100));
    if(dest_vy == 0) dest_vy = 50 * Positiv(GetYDir(0, 100));
    if(GetEffect("IntJnRBewegung", this))
      RemoveEffect("IntJnRBewegung", this);
    AddEffect("IntJnRBewegung", this, 1, 10, this, 0, dest_vx, dest_vy);
  }
}

public func ContainedLeft(object controller)
{
	if(controller == Pilot)
	{
  ResetControl();
  MoveLeft();
  return 1;
  }else return 1;
}

public func ContainedLeftDouble(object controller)
{
	if(controller == Pilot)
	{
  ResetControl();
  // Drehung
  if (Fliegt())
    if (GetDir() && (GetAction() != "FlyTurn"))
      { SetDir(DIR_Left); SetAction("FlyTurn"); iComX = -iComX; }
  return 1;
  }else return 1;
}
  
public func ContainedRight(object controller)
{
	if(controller == Pilot)
	{
  ResetControl();
  MoveRight();
  return 1;
  }else return 1;
}

public func ContainedRightDouble(object controller)
{
	if(controller == Pilot)
	{
  ResetControl();
  // Drehung
  if (Fliegt())
    if (!GetDir() && (GetAction() != "FlyTurn"))
      { SetDir(DIR_Right); SetAction("FlyTurn"); iComX = -iComX;  }
  return 1;
  }else return 1;
}

public func ContainedUp(object controller)
{
	if(controller == Pilot)
	{
  ResetControl();
  // Im Flug: Flugsteuerung  
  if (Fliegt())
  {
   iComY = Max(iComY - GetBeschleunigung(), -GetMaxSpeed());
     return 1;
  }else {
  return Starten();
  }
  }else if(controller == Gunner) Geschuetz->ControlUp(Gunner);
  return 1;
}

public func ContainedDown(object controller)
{
	if(controller == Pilot)
	{
  ResetControl();
  if (Fliegt())
  {
    iComY = Min(iComY + GetBeschleunigung(), GetMaxSpeed());
    return 1;
  }
  return 1;
  }else if(controller == Gunner) Geschuetz->ControlDown(Gunner);
  return 1;
}

public func ContainedDownDouble(pByObject)
{
  ResetControl();
  return Aussteigen(pByObject);
}

public func ContainedDigSingle()
{
  return 1;  
}

public func ContainedThrow(object controller)
{
    if(controller == Gunner)
    {
    /*var missle = CreateObject(LMIS,GetVertex(13,0),GetVertex(13,1),GetController(Gunner));
    var speed = Sqrt(GetXDir()**2+GetYDir()**2);
    var angle = GetR()-90+GetDir()*180;
    SetYDir(GetYDir(),missle);
    SetXDir(GetXDir(),missle);
    SetR(angle,missle);
		ScheduleCall(missle,"Launch",16,0,angle,70,speed);
    //missle->Launch(angle,70,speed);
    SetCursor(GetController(Gunner),missle,true,true);*/
    Geschuetz->ControlThrow(Gunner);
    }
    else if(controller == Pilot)ControlThrow(Pilot);
    return 1;
}

public func ContainedDigDouble(object controller)
{
    if(controller == Gunner)Geschuetz->ControlDigDouble(Gunner);
    else if(controller == Pilot)return 1;
    return 1;
}


//--------------------------------------------Bewegung---------------------------------------------

private func MoveLeft()
{  
  if (Fliegt())
  {
    iComX = Max(iComX - GetBeschleunigung(), -GetMaxSpeed());
    return 1;
  }
  return 0;
}

private func MoveRight()
{  
  if (Fliegt())
  {
    iComX = Min(iComX + GetBeschleunigung(), GetMaxSpeed());
    return 1;
  }
  return 0;
}

private func Starten(bool fVertical)
{
  SetAction("Starten");
  iComX=Sin(GetR(),25);
	iComY=-Cos(GetR(),25);
  // Noch keine Zielrichtung
  SetComDir(COMD_None);
}

private func StopFlying()
{
  SetAction("Wait");
  SetRDir();
}

private func Fliegen() // PhaseCall aller Flugaktionen
{
  // Gegenwärtige Geschwindigkeit ermitteln
  var iVx=GetXDir(0,100), iVy=GetYDir(0,100);
  // Wunschgeschwindigkeit ermitteln
  var idVx=iVx+iComX, idVy=iVy+iComY;
  // Geschwindigkeit weiterführen
  iVx += BoundBy(iComX, -20, 20);
  iComX = idVx-iVx;
  var iMaxRaise = Min(Abs(iComX), 10)-Min(Abs(iVx), 20);
  iVy += BoundBy(iComY, iMaxRaise, 20); iComY = idVy-iVy;
  SetWinkel();
  // Neu berechnete Geschwindigkeit setzen
  SetXDir(iVx, 0, 100);
  SetYDir(iVy, 0, 100);
  
  return 1;
}

private func GeschwindigkeitAnpassen()
{
  // Bei hoher Geschwindigkeit Rotorgeschwindigkeit erhöhen
  if (Abs(GetXDir()) > 25) return SetAction("FlyFast");
  else return SetAction("Fly");
}

private func FlyTurningProcess()
{
  if(GetPhase() == 1) iTurnSpeed = GetXDir();
  else SetXDir(Cos(180*GetPhase()/GetActMapVal("Length", "FlyTurn"),iTurnSpeed));
  SetRDir();
  // Normale Flugverarbeitung
  return Fliegen();
}

//--------------------------------------------Waffen-----------------------------------------------
public func FMData1(int data)
{
if(data == FM_Name)      return("Hydras");
if(data == FM_AmmoID)    return(MIAM);
if(data == FM_AmmoLoad)  return(8);

if(data == FM_Reload)    return(200);
if(data == FM_Recharge)  return(12);

if(data == FM_Damage)    return(30);

if(data == FM_Auto)      return(true);

return(Default(data));
}

public func Fire1()
{
var ammo = CreateObject(LMIS,GetVertex(10,0),GetVertex(10,1),GetController(Pilot));//Rakete an der Raketenhalterung erstellen 
var speed = Sqrt(GetXDir()**2+GetYDir()**2);//rechnet die Geschwindigkeit des Helis aus(Satz des Pythagoras)
var angle = GetR()-90+GetDir()*180;
ammo->Launch(angle,GetFMData(FM_Damage),speed);
}

//-------------------------------------------Eingang-----------------------------------------------

public func Aussteigen(object pByObject)
{
    // Nur Auf dem Boden rausgehen
  if (GetContact(0, -1, 8))
    if (pByObject == Pilot)
    {
      Exit(Pilot);
      SetPosition(GetX(Entrance),GetY(Entrance),Pilot);//<--Der Clonk soll ja auch bei dem Eingang und nicht durch das Metall aussteigen
      Pilot = 0;
      SetAction("Wait");
      return 1;
    }else if(pByObject == Gunner)
    {
      Exit(Gunner);
      SetPosition(GetX(Entrance),GetY(Entrance),Gunner);
      Gunner = 0;
      return 1;
    }  
 return 0;
}

//------------------------------------------Kontaktabfrage-----------------------------------------

/*protected func ContactTop()
{
	DoDamage(100,this);
}*/

//---------------------------------------------Vertices--------------------------------------------

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
}

//---------------------------------------Eigene Funktionen-----------------------------------------
private func SetWinkel()
{
    if(iComX<0 && GetDir() == DIR_Left)
    {
        if(Abs(iComX) > 5*GetR()){ SetRDir(2);}
        SetRDir(-2);
    }else if(iComX>0 && GetDir() == DIR_Left)
    {
        if(Abs(iComX) < 5*GetR()){ SetRDir(-2);}
        SetRDir(2);
    }else if(iComX<0 && GetDir() == DIR_Right)
    {
        if(Abs(iComX) > 5*GetR()){ SetRDir(2);}
        SetRDir(-2);
    }else if(iComX>0 && GetDir() == DIR_Right)
    {
        if(Abs(iComX) < 5*GetR()){ SetRDir(-2);}
        SetRDir(2);
    }else if(iComX == 0)
    {
    if(GetR() > 0)SetRDir(-5,this,100);
    else if(GetR()== 0)SetRDir();
    else if(GetR() < 0)SetRDir(5,this,100);
    }   
}

public func Damage() //Testet auf Zerstörung
{
  if(GetDamage() < GetLeben()) return;
  RemoveObject(Entrance);
  RemoveObject(Geschuetz);
  Explode(200);
}

public func Fliegt() //Fliegt der Heli?
{
  return (GetProcedure()=="FLOAT");
}
   
private func Positiv(int x)
{
  if(x > 0) return 1;
  return -1;
}
   
public func ResetControl()
{
  SetCommand(this, "None");
}

public func ReadyToFire()
{
  if(GetAction() == "Wait") return 0;
  return 1;
}

public func GetLeben()
{
return 400;
}

public func GetBeschleunigung()
{
return 150;
}

public func GetMaxSpeed()
{
return 300;
}

public func UpdateCharge(object driver) 
{
if(driver == Pilot)hud = driver->GetHUD(); // Der Fahrer bekommt den Hud
if(hud) hud->Update(this(), driver->AmmoStoring(),driver);
return 1;
}

public func IsBulletTarget()
{
return 1;
}

public func IsAircraft()
{
return 1;
}
