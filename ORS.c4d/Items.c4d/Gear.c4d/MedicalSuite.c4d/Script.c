//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Medical Suite  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
#include GEAR

local aBulks;//speichert alle Lebensbalken
local pToolkit;
local aBulks;
//----------------------------------------  Callbacks  --------------------------------------------
public func GetGearType()	{ return GEAR_Armor; }
public func IsEquipment()	{ return true; }
public func IsChemicalProduct() { return true; }
public func HealingRate() { return 8; }//wie viel leben pro sekunde geheilt werden(in %/s)
public func HealingDistance() { return 16; }//Reichweite der Heilfertigkeit in Pixeln

public func GearEffect(object pClonk)
{
	//Erstmal werden alle Primärwaffen abgelegt
	for(var i=0;i<ContentsCount(0,pClonk);i++)
	{
	if(!Contents(i,pClonk)->~IsSecondaryWeapon() && Contents(i,pClonk)->~IsWeapon())pClonk->Exit(Contents(i,pClonk));
	}
	//HeilWerkzeuge dem Clonk geben
	pToolkit=CreateContents(HTLS,pClonk);
	LocalN("pMedicalSuite",pToolkit)=this;
	//LebensBalken von verbündeten anzeigen
	AddEffect("LebensBalken",this,9,1,this,0,pClonk);
	//Danach wird der Clonk neu eingekleidet
  SetGraphics("MedicalSuited",pClonk,G_MS);
  return Sound("Connect");
}

public func GearEffectReset(object pClonk)
{
  SetGraphics("",pClonk);
}

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return;
  if(iType == DMG_Melee)			return 0;
  if(iType == DMG_Projectile)	return -5;//der Stoff ist anfälliger gegen durchschlagende Projektile
  if(iType == DMG_Explosion)	return 0;
  if(iType == DMG_Fire)				return 0;
  if(iType == DMG_Energy)			return 0;
  if(iType == DMG_Bio)				return 0;
}

//--------------------------------------  Heilfunktionen  -----------------------------------------
protected func Heal(object pUser) 
{
var pTargets = FindObjects(Find_Distance(HealingDistance(),GetX(pUser),GetY(pUser)),Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive));
if(!pTargets)return 0;
CreateMenu(HTLS,pUser,pUser,0,"Kein Verletzter in unmittelbarer Nähe.",1,1);
SetGlobal(99,this);//Den Anzug in einer globalen Variable speichern für den Zugriff von Außen
for(var i=0; i<GetLength(pTargets);i++) 
	{
	if(GetEnergy(pTargets[i])==GetPhysical("Energy",0,pTargets[i])/1000&&
	pTargets[i]->~GetBodyStatus(BP_Head)==0&&
	pTargets[i]->~GetBodyStatus(BP_Body)==0&&
	pTargets[i]->~GetBodyStatus(BP_RArm)==0&&
	pTargets[i]->~GetBodyStatus(BP_LArm)==0&&
	pTargets[i]->~GetBodyStatus(BP_LLeg)==0&&
	pTargets[i]->~GetBodyStatus(BP_RLeg)==0)continue;
  var pTarget = pTargets[i];
  var iBodyDamage=pTarget->~GetBodyStatus(BP_Head)+
									pTarget->~GetBodyStatus(BP_Body)+
									pTarget->~GetBodyStatus(BP_RArm)+
									pTarget->~GetBodyStatus(BP_LArm)+
									pTarget->~GetBodyStatus(BP_LLeg)+
									pTarget->~GetBodyStatus(BP_RLeg);
  AddMenuItem(Format("%s mit %d% Leben und %d Schaden am Körper",GetName(pTarget),(GetEnergy(pTarget)*10)/(GetPhysical("Energy",0,pTarget)/10000),iBodyDamage),Format("Global(99)->PostHeal(Object(%d),Object(%d))",ObjectNumber(pUser),ObjectNumber(pTarget)),GetID(pTarget),pUser);
	}
var pTargets = FindObjects(Find_Distance(HealingDistance(),GetX(pUser),GetY(pUser)),Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive),Find_Action("Unconscious"));
for(var i=0; i<GetLength(pTargets);i++)
	{
  var pTarget = pTargets[i];
  if(pTarget==this)continue;
  AddMenuItem(Format("%s ist Bewusstlos",GetName(pTarget)),Format("SetConscious(Object(%d))",ObjectNumber(pTarget)),GetID(pTarget),pUser);
	}
}

public func PostHeal(object pUser,object pTarget)
{
var iMultiplier = 1;
if(pTarget == pUser)iMultiplier *= 3;//sich selbst zu heilen ist deutlich anspruchsvoller
AddEffect("Healing",this,10,36/HealingRate()*iMultiplier,this,0,pTarget,pUser);
ObjectSetAction(pUser, "Heal");
}

public func FxHealingStart(object pTarget, int iEffectNumber, int iTemp,object target,object pUser)
{
EffectVar(0,this,iEffectNumber)=target;
EffectVar(1,this,iEffectNumber)=pUser;
}

public func FxHealingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
if(!EffectVar(0,this,iEffectNumber || !EffectVar(1,this,iEffectNumber)))return -1;//existieren heiler und geheilter?
if(!GetAlive(EffectVar(0,this,iEffectNumber)) || !GetAlive(EffectVar(1,this,iEffectNumber)))return -1;//leben beide noch?
if(EffectVar(1,this,iEffectNumber)->~GetGear(1)!=this)return -1;//ist der suite noch angezogen?
if(GetAction(EffectVar(1,this,iEffectNumber))!="Heal")return -1;//will der clonk noch heilen?
if(ObjectDistance(EffectVar(0,this,iEffectNumber),EffectVar(1,this,iEffectNumber))>HealingDistance())return -1;//reicht die distanz zwischen beiden aus?
if(!PathFree (GetX(EffectVar(0,this,iEffectNumber)),GetY(EffectVar(0,this,iEffectNumber)),
							GetX(EffectVar(1,this,iEffectNumber)),GetY(EffectVar(1,this,iEffectNumber))))return -1;//ist der Weg zwischen den beiden frei?
//wenn das Ziel vollständig geheilt ist, wird der Effekt abgebrochen
if(	GetEnergy(EffectVar(0,this,iEffectNumber))==GetPhysical("Energy",0,EffectVar(0,this,iEffectNumber))/1000&&
		EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_Head)==0&&
		EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_Body)==0&&
		EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_RArm)==0&&
		EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_LArm)==0&&
		EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_LLeg)==0&&
		EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_RLeg)==0)return -1;
//beschädigte Körperteile werden geheilt
if(GetEnergy(EffectVar(0,this,iEffectNumber))!=GetPhysical("Energy",0,EffectVar(0,this,iEffectNumber))/1000)DoEnergy(1,EffectVar(0,this,iEffectNumber));
if(EffectVar(0,this,iEffectNumber)->GetBodyStatus(BP_Head)>0)EffectVar(0,this,iEffectNumber)->~ChangeBodyStatus(BP_Head,-1);
if(EffectVar(0,this,iEffectNumber)->GetBodyStatus(BP_Body)>0)EffectVar(0,this,iEffectNumber)->~ChangeBodyStatus(BP_Body,-1);
if(EffectVar(0,this,iEffectNumber)->GetBodyStatus(BP_RArm)>0)EffectVar(0,this,iEffectNumber)->~ChangeBodyStatus(BP_RArm,-1);
if(EffectVar(0,this,iEffectNumber)->GetBodyStatus(BP_LArm)>0)EffectVar(0,this,iEffectNumber)->~ChangeBodyStatus(BP_LArm,-1);
if(EffectVar(0,this,iEffectNumber)->GetBodyStatus(BP_RLeg)>0)EffectVar(0,this,iEffectNumber)->~ChangeBodyStatus(BP_RLeg,-1);
if(EffectVar(0,this,iEffectNumber)->GetBodyStatus(BP_LLeg)>0)EffectVar(0,this,iEffectNumber)->~ChangeBodyStatus(BP_LLeg,-1);
//aktuelles Leben ausgeben als Message
var iBodyDamage=EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_Head)+
								EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_Body)+
								EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_RArm)+
								EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_LArm)+
								EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_LLeg)+
								EffectVar(0,this,iEffectNumber)->~GetBodyStatus(BP_RLeg);
PlayerMessage(GetController(EffectVar(1,this,iEffectNumber)),"Leben: %d%   Körperschaden: %d",EffectVar(1,this,iEffectNumber),(GetEnergy(EffectVar(0,this,iEffectNumber))*10)/(GetPhysical("Energy",0,EffectVar(0,this,iEffectNumber))/10000),iBodyDamage);
}

public func FxHealingStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
if(GetAction(EffectVar(1,this,iEffectNumber))=="Heal")ObjectSetAction(EffectVar(1,this,iEffectNumber), "Walk");
}


//--------------------------------------  Lebensanzeigen  -----------------------------------------

public func FxLebensBalkenStart(object pTarget, int iEffectNumber, int iTemp,object user)
{
EffectVar(0,this,iEffectNumber)=user;
//Arrays zur Speicherung der HUDs erstellen
aBulks=CreateArray(0);
}

public func FxLebensBalkenTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
if(!EffectVar(0,this,iEffectNumber))return -1;//existiert der Clonk?
if(!GetAlive(EffectVar(0,this,iEffectNumber)))return -1;//lebt er noch?
if(EffectVar(0,this,iEffectNumber)->~GetGear(1)!=this)return -1;//hat er den Suite noch an?
for(var obj in FindObjects(Find_Allied(GetController(EffectVar(0,this,iEffectNumber))),Find_OCF(OCF_CrewMember)))
	{
	var bUeberspringen = false;//sagt, ob das objekt schon einen balken hat
	for(var i = 0;i < GetLength(aBulks);i++)
		{
		if(obj==LocalN("pClonk",aBulks[i]) || obj==EffectVar(0,this,iEffectNumber))//testet, ob einer der bereits existierenden Balken zu diesem Clonk gehört
			{
			bUeberspringen = true;//Ja es hat wohl einen
			continue;
			}
		}
	if(!bUeberspringen)//wird ausgeführt wenn das Objekt noch keine Lebensanzeige hat
		{
		//roter balken und umrandung wird erstellt
		if(FindObject(RDMG))
			{
			var pLife = CreateObject(M_BL,0,10,GetController(EffectVar(0,this,iEffectNumber)));
			var pBalken = CreateObject(M_BB,0,10,GetController(EffectVar(0,this,iEffectNumber)));
			CreateBody(obj,EffectVar(0,this,iEffectNumber));
			}else
			{
			var pLife = CreateObject(LFBK,0,10,GetController(EffectVar(0,this,iEffectNumber)));
			var pBalken = CreateObject(BULK,0,10,GetController(EffectVar(0,this,iEffectNumber)));
			}
		//in die lokalen Variablen vom Balken werden noch ein paar Daten eingetragen
		LocalN("pClonk",pLife)=obj;
		LocalN("pBulk",pLife)=pBalken;
		LocalN("pMedic",pLife)=EffectVar(0,this,iEffectNumber);
		//beides wird an den Clonk rangehängt
		pBalken->AttachTo(obj);
		pLife->AttachTo(obj);
		aBulks[GetLength(aBulks)]=pLife;//Balken in die Liste eintragen
		}
	}
	//sich um das Toolkit sorgen
if(!pToolkit)
	{
	pToolkit=CreateContents(HTLS,EffectVar(0,this,iEffectNumber));
	LocalN("pMedicalSuite",pToolkit)=this;
	ObjectSetAction(EffectVar(0,this,iEffectNumber),"Walk");
	}
}


public func CreateBody(object pClonk, object pMedic)
{
var a=CreateObject(M_HD);
a->SetClonk(pClonk,pMedic);
var b=CreateObject(M_LA);
b->SetClonk(pClonk,pMedic);
var c=CreateObject(M_LL);
c->SetClonk(pClonk,pMedic);
var d=CreateObject(M_RA);
d->SetClonk(pClonk,pMedic);
var e=CreateObject(M_RL);
e->SetClonk(pClonk,pMedic);
var f=CreateObject(M_BD);
f->SetClonk(pClonk,pMedic);
f->SetParts(a,b,d,c,e);
return f;
}




