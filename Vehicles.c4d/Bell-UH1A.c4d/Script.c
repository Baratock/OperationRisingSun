//------------------------------------------- Bell UH-1 -------------------------------------------

#strict 2

//----------------------------------------Lokale Variablen-----------------------------------------
local xAcc;
local yAcc;
local iTurnSpeed;
local Pilot;
local Entrance;
local hud;
local iSpeed;

func Initialize() {
	Entrance = CreateObject(B_EN,GetVertex(9,0),GetVertex(9,1));
	Entrance->AttachTo(this);
	Entrance->SetActionData(9);
  SetComDir(COMD_Stop);
  SetAction("Wait");
  yAcc = 0;
  xAcc = 0;
  Pilot = 0;
  hud=0;
  return 1;
}

//-----------------------------------------Die Steuerung-------------------------------------------

private func SetGeschwindigkeit(int comdir, &vx, &vy)//für einen flüssigen Geschwindigkeitswechsel
{
  vx = vy = 0;
  var max = GetPhysical("Float", 0, this);
  if     (comdir == COMD_Up)         { vy = -max; }
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

  xAcc = BoundBy(dest_vx - GetXDir(0, 100), -GetMaxSpeed(), GetMaxSpeed());
  yAcc = BoundBy(dest_vy - GetYDir(0, 100), -GetMaxSpeed(), GetMaxSpeed());

  if(xAcc == 0 && yAcc == 0) return -1;
}

public func ControlUpdate(object controller, int comdir)
{
  if(Fliegt())
  {
    var dest_vx, dest_vy;
    SetGeschwindigkeit(comdir, dest_vx, dest_vy);
    if(dest_vx == 0) dest_vx = 50 * Positiv(GetXDir(0, 100));
    if(dest_vy == 0) dest_vy = 50 * Positiv(GetYDir(0, 100));
    if(GetEffect("IntJnRBewegung", this)) RemoveEffect("IntJnRBewegung", this);
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
      { SetDir(DIR_Left); SetAction("FlyTurn"); xAcc = -xAcc; }
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
      { SetDir(DIR_Right); SetAction("FlyTurn"); xAcc = -xAcc;  }
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
   yAcc = Max(yAcc - GetBeschleunigung(), -GetMaxSpeed());
     return 1;
  }else {
  return Starten();
  }
  }else return 1;
}

public func ContainedDown(object controller)
{
	if(controller == Pilot)
	{
  ResetControl();
  if (Fliegt())
  {
    yAcc = Min(yAcc + GetBeschleunigung(), GetMaxSpeed());
    return 1;
  }
  return 1;
  }else return 1;
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
	return 1;
}

public func ContainedDigDouble(object controller)
{
	if(controller == Pilot)return 1;
	return 1;
}


//--------------------------------------------Bewegung---------------------------------------------

private func MoveLeft()
{  
	if (Fliegt())
	{
    xAcc = Max(xAcc - GetBeschleunigung(), -GetMaxSpeed());
    return 1;
	}
	return 0;
}

private func MoveRight()
{
	if (Fliegt())
	{
    xAcc = Min(xAcc + GetBeschleunigung(), GetMaxSpeed());
    return 1;
	}
	return 0;
}

private func Starten(bool fVertical)
{
	SetAction("Starten");
	xAcc=Sin(GetR(),25);
	yAcc=-Cos(GetR(),25);
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
  var idVx=iVx+xAcc, idVy=iVy+yAcc;
  // Geschwindigkeit weiterführen
  iVx += BoundBy(xAcc, -20, 20);
  xAcc = idVx-iVx;
  var iMaxRaise = Min(Abs(xAcc), 10)-Min(Abs(iVx), 20);
  iVy += BoundBy(yAcc, iMaxRaise, 20); yAcc = idVy-iVy;
  SetWinkel();
  // Neu berechnete Geschwindigkeit setzen
  SetXDir(iVx, 0, 100);
  SetYDir(iVy, 0, 100);
  iSpeed=GetSpeed();
  return 1;
}

private func GeschwindigkeitAnpassen()
{
  // Bei hoher Geschwindigkeit Rotorgeschwindigkeit erhöhen
  if (GetSpeed() >= GetMaxSpeed()/2) return SetAction("FlyFast");
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
    } 
 return 0;
}

//----------------------------------------Kontaktabfrage-------------------------------------------

/*protected func ContactTop()
{
	DoDamage(100,this);
}*/

//-------------------------------------------Vertices----------------------------------------------

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

//--------------------------------------Eigene Funktionen------------------------------------------
private func SetWinkel()
{
	if(xAcc<0 && GetDir() == DIR_Left)
	{
    if(Abs(xAcc) > 3*GetR()){ SetRDir(1);}
    SetRDir(-1);
	}else if(xAcc>0 && GetDir() == DIR_Left)
	{
    if(Abs(xAcc) < 3*GetR()){ SetRDir(-1);}
    SetRDir(1);
  }else if(xAcc<0 && GetDir() == DIR_Right)
  {
    if(Abs(xAcc) > 3*GetR()){ SetRDir(1);}
    SetRDir(-1);
  }else if(xAcc>0 && GetDir() == DIR_Right)
  {
    if(Abs(xAcc) < 3*GetR()){ SetRDir(-1);}
    SetRDir(1);
  }else if(xAcc == 0)
  {
  	if(GetR() > 0)SetRDir(-6,this,100);
  	else if(GetR()== 0)SetRDir();
  	else if(GetR() < 0)SetRDir(6,this,100);
  }   
}

public func Damage() //Testet auf Zerstörung
{
  if(GetDamage() < GetLeben()) return;
  RemoveObject(Entrance);
  Explode(100);
}

public func Fliegt() //Fliegt der Heli?
{
  return GetProcedure()=="FLOAT";
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
	return 30;
}

public func GetMaxSpeed()
{
	return 400;
}

public func GetSpeed()
{
	return Sqrt(GetXDir()**2+GetYDir()**2);
}

public func UpdateCharge(object driver) 
{
if(driver == Pilot)hud = driver->GetHUD(); // Der Fahrer bekommt den Hud
else return 1;
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
