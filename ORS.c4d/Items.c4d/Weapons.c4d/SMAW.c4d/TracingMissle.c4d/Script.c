/* Rakete */

#strict
#include RSAG

local pTarget, iTimer;

public func Acceleration() { return(4); }
public func MaxTime() { return(70); }
public func MaxSpeed() { return(160); }

public func HomingBegin() { return(iTimer); }
public func MaxHomingDist() { return(800); }
public func MaxHomingAngle() { return(60); }

/* Timer */

public func SetTimer(int iTime){ iTimer=iTime; }

private func Traveling()
{
  // kommt nich weiter
  if(GetActTime() >= MaxTime()) return(Hit());

  // Effekte
  Smoking();

  // beschleunigen
  Accelerate();

  // nur lenken wenn schon etwas fliegend
  if(GetActTime() < HomingBegin()) return();

  // lenken
  if(pTarget) FollowTarget();

  if(!pTarget)pTarget = FindTarget();
}

private func FindTarget() 
{
    var targets = FindObjects(Find_Distance(1000),Find_Func("IsGPS"));
    var target;
  
    // teuerstes Objekt anivisieren
    for(var t in targets) {
        if(!target) {
            target = t;
            continue;
        }
        if(t->GetValue() > target->GetValue()) {
            target = t;
            break;
        }
    }

    return(target);
}


private func FollowTarget() {

    var iDAngle = Angle(GetX(),GetY(),GetX(pTarget),GetY(pTarget));
    var iAngle = GetR();
    
    var iDiff = Normalize(iDAngle - iAngle,-180);
    var iTurn = Min(Abs(iDiff),7);

    SetR(iAngle+iTurn*((iDiff > 0)*2-1));
}

public func NoDecoDamage() { return(true); }
