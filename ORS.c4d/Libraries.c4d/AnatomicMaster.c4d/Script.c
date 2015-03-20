//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ AnatomicMaster ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#strict 2
#appendto CLNK
//gibt den Schaden der Körperteile in % an
local iHead,iBody,iLeftArm,iRightArm,iLeftLeg,iRightLeg;
//zeigt ob da eine offene Wunde ist
local iBloodHead,iBloodBody,iBloodLeftArm,iBloodRightArm,iBloodLeftLeg,iBloodRightLeg;
//zeigt ob die wunde verbunden ist
local vHead,vBody,vLeftArm,vRightArm,vLeftLeg,vRightLeg;
local iLastYDir,pKiller;
local concentration,aGear;
local uncTimer;//überprüft nur alle paar Frames auf Schmerzbelastung
local bUnc;//ist der Clonk gerade bewusstlos?

public func Initialize()
{
pKiller=0;
iLastYDir=GetYDir();
iHead = 0;
iBody = 0;
iLeftArm = 0;
iRightArm = 0;
iLeftLeg = 0;
iRightLeg = 0;
vHead = 1;
vBody = 1;
vLeftArm = 1;
vRightArm = 1;
vLeftLeg = 1;
vRightLeg = 1;
iBloodHead = 0;
iBloodBody = 0;
iBloodLeftArm = 0;
iBloodRightArm = 0;
iBloodLeftLeg = 0;
iBloodRightLeg = 0;
uncTimer=true;
bUnc=false;
var xPos=50;
var yPos=200;
var b=CreateObject(B_BD);
SetPosition(xPos,yPos,b);
var b=CreateObject(B_HD);
SetPosition(xPos,yPos,b);
var b=CreateObject(B_LA);
SetPosition(xPos+1,yPos,b);
var b=CreateObject(B_RA);
SetPosition(xPos-1,yPos,b);
var b=CreateObject(B_LL);
SetPosition(xPos,yPos,b);
var b=CreateObject(B_RL);
SetPosition(xPos,yPos,b);
if(FindObject(RDMG))AddEffect("AnatomicMaster",this,2,1,this);
return _inherited();
}

public func GetBodyStatus(string sBP){return LocalN(sBP,this);}
public func ChangeBodyStatus(string sBP,int iChange){LocalN(sBP,this)+=iChange;}

public func FxAnatomicMasterTimer (object pTarget, int iEffectNumber, int iEffectTime)
{
iLastYDir=GetYDir();
//Grenzwerte dürfen nicht überschritten werden
if(iHead<0)iHead=0;
if(iBody<0)iBody=0;
if(iLeftArm<0)iLeftArm=0;
if(iRightArm<0)iRightArm=0;
if(iLeftLeg<0)iLeftLeg=0;
if(iRightLeg<0)iRightLeg=0;
if(iHead>100)iHead=100;
if(iBody>100)iBody=100;
if(iLeftArm>100)iLeftArm=100;
if(iRightArm>100)iRightArm=100;
if(iLeftLeg>100)iLeftLeg=100;
if(iRightLeg>100)iRightLeg=100;
//Beinbehinderung
if(this->~HasRealWeight())
	{
	var legs = (iLeftLeg+iRightLeg)/2;
	SetPhysical("Walk",GetPhysical("Walk",0,pTarget)-GetPhysical("Walk",0,pTarget)*legs/100,2,pTarget);
	SetPhysical("Jump",GetPhysical("Jump",0,pTarget)-GetPhysical("Jump",0,pTarget)*legs/100,2,pTarget);
	SetPhysical("Dig",GetPhysical("Dig",0,pTarget)-GetPhysical("Dig",0,pTarget)*legs/100,2,pTarget);
	}
//Armbehinderung
if(this->~CanRealAim())
	{
	var arms = (iLeftArm+iRightArm*3)/6;//Clonks sind Rechtshänder
	var maxConc = GetMaxConcentration()-GetMaxConcentration()*arms/100;
	if(concentration>maxConc)concentration=maxConc;
	}
//Blutungen
var iBodyDamage=iHead+iBody+iLeftArm+iRightArm+iLeftLeg+iRightLeg;
if(FindObject(R_BD) && iBodyDamage>0)
	{
		if(pKiller)
			{
			if(iBloodHead)pKiller->DoEnergy(-iBloodHead,this,true);
			if(iEffectTime%2+1==2 && iBloodBody)pKiller->DoEnergy(-iBloodBody,this,true);
			if(iEffectTime%4+1==4 && iBloodLeftArm)pKiller->DoEnergy(-iBloodLeftArm,this,true);
			if(iEffectTime%4+1==4 && iBloodRightArm)pKiller->DoEnergy(-iBloodRightArm,this,true);
			if(iEffectTime%4+1==4 && iBloodLeftLeg)pKiller->DoEnergy(-iBloodLeftLeg,this,true);
			if(iEffectTime%4+1==4 && iBloodRightLeg)pKiller->DoEnergy(-iBloodRightLeg,this,true);
			}else
			{
			if(iBloodHead)DoEnergy(-iBloodHead,this,true);
			if(iEffectTime%2+1==2 && iBloodBody)DoEnergy(-iBloodBody,this,true);
			if(iEffectTime%4+1==4 && iBloodLeftArm)DoEnergy(-iBloodLeftArm,this,true);
			if(iEffectTime%4+1==4 && iBloodRightArm)DoEnergy(-iBloodRightArm,this,true);
			if(iEffectTime%4+1==4 && iBloodLeftLeg)DoEnergy(-iBloodLeftLeg,this,true);
			if(iEffectTime%4+1==4 && iBloodRightLeg)DoEnergy(-iBloodRightLeg,this,true);
			}
		//Die Wunden hören langsam auf zu bluten
		if(iEffectTime%18+1==18)
			{
			if(iBloodHead>0)iBloodHead--;
			if(iBloodBody>0)iBloodBody--;
			if(iBloodLeftArm>0)iBloodLeftArm--;
			if(iBloodRightArm>0)iBloodRightArm--;
			if(iBloodLeftLeg>0)iBloodLeftLeg--;
			if(iBloodRightLeg>0)iBloodRightLeg--;
			}		
	}
if(FindObject(R_UC))
	{
	if(iEffectTime%4+1==4)uncTimer=true;
	var pro=100-iBodyDamage;
	if(pro<10)pro=10;
	if(RandomX(1,100)<=pro && iEffectTime%36+1==36 && GetAction()=="Unconscious")SetConscious(this);
	}
}

public func OnHit(int iDamage, int iType, object pFrom)
{
	if(LocalN("shooter",pFrom))pKiller=LocalN("shooter",pFrom);
	//auf Bewusstlosigkeit testen
	if(FindObject(R_UC) && uncTimer)
		{
		var iBodyDamage=(iHead+iBody+iLeftArm+iRightArm+iLeftLeg+iRightLeg)/5;
		if(RandomX(1,50) <= iBodyDamage && GetAction()!="Unconscious")
		{
		bUnc=true;
		SetAction("Unconscious");
		}
		uncTimer=false;
		}
  return _inherited();
}

public func SetConscious(object pTarget)
{
if(GetAction(pTarget)=="Unconscious")
	{
	LocalN("bUnc",pTarget)=false;
	ObjectSetAction(pTarget,"KneelUp",0,0,true);
	RemoveEffect("Unconscious", pTarget);
	}
}

func Hit()
{
if(FindObject(RDMG))
{
	//Gesamtmasse wird berechnet
	var gMasse = GetMass(this);
	for(var i=0;i < ContentsCount(0,this); i++) gMasse += GetMass(Contents(i,this));
	for (var x = 1; x <= 6; x++)
	{   
		 if(GetType(GetGear(x))==C4V_C4Object)gMasse += GetMass(GetGear(x));
	}
	//Impuls im Moment des Aufpralls wird berechnet
	var iImpuls=gMasse*iLastYDir/110;
	if(iImpuls>=50)
	{
		//Beine werden beschädigt und brechen bei einem bestimmten Schaden
		iLeftLeg+=iImpuls/3;
		iRightLeg+=iImpuls/3;
		if((iLeftLeg+iRightLeg)>=60)Sound(Format("break%d",RandomX(1,2)));
	}
}
return _inherited();
}

func Death()
{
if(bUnc)SetAction("Death");
return _inherited();
}

//------------------------------------  Ausrüstungsfunktionen  ------------------------------------
//Die Funktion gibt das Objekt vom angegeben Typen der Ausrüstung zurück falls vorhanden
public func GetGear(int iGearType)
{
  if(iGearType) 
  {
    if(aGear[iGearType])
    {
        for(gear in aGear)
        {
            if(gear) return(gear);
        }
      }
    return 0;
    }else return 0;
}

public func HasGear(int iGearType, id idGear)
{
  // nach bestimmter ID suchen
  var gear;
  if(idGear) {
    for(gear in aGear)
	  if(gear)
	    if(GetID(gear) == idGear)
	      return gear;
    return false;
  }

  // nach bestimmtem Typ suchen
  if(iGearType) {
    if(aGear[iGearType])
      return true;
    return false;
  }
  
  // keine Parameter: allgemein ob Ausrüstung da ist
  for(gear in aGear)
    if(gear)
      return true;
  
  return false;
}

public func IsRealHurtable(){return true;}
public func GetMaxConcentration(){return _inherited();}

//----Bewusstlosigkeit

protected func GetUnconscious()
{
  SetShape(-8,2,16,8,this);
 //Vertices neu setzen
 SetVertexXY(0, 0,5,this);
 SetVertexXY(1, 0,2,this);
 SetVertexXY(2, 0,9,this);
 SetVertexXY(3,-6,3,this);
 SetVertexXY(4, 6,3,this);
 SetVertexXY(5,-7,6,this);
 SetVertexXY(6, 7,6,this);
 AddEffect("Unconscious",this,1,1,this);
}

protected func IsUnconscious(){return GetAction(this) == "Unconscious";}

protected func FxUnconsciousTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
 if(!pTarget->~IsUnconscious())
 {
  SetShape(-8,-10,16,20,this);
  SetVertexXY(0, 0, 0,this);
  SetVertexXY(1, 0,-7,this);
  SetVertexXY(2, 0, 9,this);
  SetVertexXY(3,-2,-3,this);
  SetVertexXY(4, 2,-3,this);
  SetVertexXY(5,-4, 3,this);
  SetVertexXY(6, 4, 3,this);
  return -1;
 }
 //Zur Sicherheit immer wied3er neu einstellen
 SetShape(-8,2,16,8,this);
 SetVertexXY(0, 0,5,this);
 SetVertexXY(1, 0,2,this);
 SetVertexXY(2, 0,9,this);
 SetVertexXY(3,-6,3,this);
 SetVertexXY(4, 6,3,this);
 SetVertexXY(5,-7,6,this);
 SetVertexXY(6, 7,6,this);
 return true;
}


protected func FxUnconsciousStop(object pTarget)
{
	SetShape(-8,-10,16,20,this);
  SetVertexXY(0, 0, 0,this);
  SetVertexXY(1, 0,-7,this);
  SetVertexXY(2, 0, 9,this);
  SetVertexXY(3,-2,-3,this);
  SetVertexXY(4, 2,-3,this);
  SetVertexXY(5,-4, 3,this);
  SetVertexXY(6, 4, 3,this);
  return true;
}
