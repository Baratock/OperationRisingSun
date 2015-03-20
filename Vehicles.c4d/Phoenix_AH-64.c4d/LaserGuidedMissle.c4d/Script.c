//--------------------------------------  Laserguided Rocket  -------------------------------------

#strict 2
#include MISS

local dir, shootingVehicle;

public func Acceleration() { return(1); }

func Construction(object byObj) {
  _inherited();
  dir = GetDir(byObj);
  shootingVehicle = byObj;
}

public func MaxTime() { return(10*36); }

//-------------------------------------Steuerung der Rakete----------------------------------------
public func ControlUp()
{
if(dir)SetR(GetR()-10);
else SetR(GetR()+10);
}

public func ControlDown()
{
if(dir)SetR(GetR()+10);
else SetR(GetR()-10);
}
//-------------------------------------------Sonstiges---------------------------------------------

//Die Rakete soll nicht den Schützen treffen
private func HitObject(object pObject) {
  if(pObject == shootingVehicle)return 0;
  return _inherited();
}
