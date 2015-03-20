/* Rakete */

#strict

local iSpeed, iDamage, exploding;
local shooter;

public func Acceleration() { return(5); }
public func MaxTime() { return(60); }
public func MaxSpeed() { return(180); }

func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return();
  // Waffe?
  shooter = GetShooter(byObj);
}

public func Launch(int iAngle, int iDmg, int speed)
{
  if(!speed) speed = 20;
  iSpeed = speed;
  iDamage = iDmg;
  if(!iDamage) iDamage = 35;

  SetR(+iAngle);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetAction("Travel");

  AddLight(100,RGB(255,127,0),this(),GLOW);

  AddEffect("HitCheck", this(), 1,1, 0, SHT1,shooter,true);
}

/* Timer */

private func Accelerate() { 
  if(iSpeed < MaxSpeed())
    iSpeed += Acceleration();

  SetXDir(+Sin(GetR(),iSpeed));
  SetYDir(-Cos(GetR(),iSpeed));
}

private func Smoking() {

  var dist = Distance(0,0,GetXDir(),GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;

  for(var i=0; i<dist; i+=ptrdist) {

    var x = -maxx*i/dist;
    var y = -maxy*i/dist;


    var rand = RandomX(-30,30);
    var xdir = +Sin(GetR()+rand,20);
    var ydir = -Cos(GetR()+rand,20);

    CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,RandomX(80,100),RGBa(255,200,200,60),0,0);
    for(var loop=6;loop<=18;loop+=4)
  	{
  	Schedule(Format("RocketSmoke(%d,%d,%d,%d,%d)",x+RandomX(-1,1)-xdir,y+RandomX(-1,1)+10-ydir,7,RGBa(75,65,55,40),loop),loop,0);
  	}
  }
}

global func RocketSmoke(int iX,int iY,int iAStr,int b,int iMulti)
{CreateParticle("RocketSmoke",iX,iY-iMulti,RandomX(-3,3),RandomX(-3,3),20+iAStr*(8+iMulti),b);}

private func Traveling()
{
  // kommt nich weiter
  if(GetActTime() >= MaxTime()) return(Hit());

  // beschleunigen
  Accelerate();

  // Effekte
  Smoking();
}
  
/* Treffer */

private func Hit() {
  HitObject();
}

private func HitObject(object pObject) 
{	
	if(pObject)
	if(GetOCF(pObject)&OCF_Living)
	if(pObject->~IsRealHurtable())
	{
		//Kopftreffer
		if(GetY()<= GetYTop(pObject) + GetDefCoreVal("Height",0,GetID(pObject))/3)
		{
		  DoBodyDmg(100,DMG_Projectile,pObject,BP_Head,GetController(shooter));
  	}else 
  	//Brusttreffer
  	if(GetY()> GetYTop(pObject) + GetDefCoreVal("Height",0,GetID(pObject))/3 && GetY()< GetYTop(pObject) + GetDefCoreVal("Height",0,GetID(pObject))/3*2)
  	{
  		var r = RandomX(1,100);
  		if(r > 40)DoBodyDmg(100,DMG_Projectile,pObject,BP_Body,GetController(shooter));//Oberkörper wird zu 60% getroffen
  		else if(r > 20 && r <= 40)DoBodyDmg(100,DMG_Projectile,pObject,BP_LArm,GetController(shooter));//Linker Arm wird zu 20% getroffen
  		else if(r <= 20)DoBodyDmg(100,DMG_Projectile,pObject,BP_RArm,GetController(shooter));//Rechter Arm wird zu 20% getroffen
  	}else
  	//Beintreffer
  	if(GetY()>= GetYTop(pObject) + GetDefCoreVal("Height",0,GetID(pObject))/3*2)
  	{
  		var r = RandomX(1,100);
  		//Die Chancen stehen 50-50
  		if(r <= 50)DoBodyDmg(100,DMG_Projectile,pObject,BP_LLeg,GetController(shooter));
  		if(r > 50)DoBodyDmg(100,DMG_Projectile,pObject,BP_RLeg,GetController(shooter));
  	}
  }
  exploding = true;
  BlastObjects(GetX(), GetY(), iDamage/2);
  BlastObjects(GetX(), GetY(), iDamage/2);
  Explode(iDamage,0,0,0,1,Format("rocket_explosion%d",RandomX(1,3)));
}

public func Damage() {
  if(GetDamage() > 20 && !exploding)
    Hit();
}

public func EMPShock() {
  SetAction("Idle");
}

private func GetYTop(pObj)
{
return GetY(pObj) + GetDefCoreVal("Offset",0,GetID(pObj),1);
}

public func IsBulletTarget() { return(true); }
public func NoDecoDamage() { return(true); }
public func IsMachine() { return(true); }
