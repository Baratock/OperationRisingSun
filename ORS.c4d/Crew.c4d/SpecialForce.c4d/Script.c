/*---- ORS-SpecialForceSoldat ----*/

#strict 2
#include HZCK
#include L_WM
#include L_PM

local GrenAimAngle, pGrenCrosshair, GrenStrengh;

public func WeaponCollectionLimit() { return 2; } // max. Waffen im Inventar
public func ObjectCollectionLimit() { return 2; } // max. Objekte im Inventar

//-------------------  Eine Primäre und eine Sekundäre Waffe kann gewählt werden  -----------------
protected func RejectCollect(id idObj, object pObj)
{
  var effect;
  if(effect = GetEffect("CollectionException", pObj))
    if(EffectVar(0, pObj, effect) == this())
      return 1;
  if(idObj == SPNP) return;
  if(GetID(pObj)==HTLS)
  	if(ContentsCount(HTLS)==0)return 0;
  	else return 1;
  if(pObj ->~ IsAmmoPacket())
    if(!CustomContentsCount("IsAmmoPacket"))
      return 0;
  if(pObj ->~ IsWeapon()) 
  {
    if(!(pObj ->~ OnCollection(this()))) {
      if(ContentsCount(idObj) || CustomContentsCount("IsWeapon") >= WeaponCollectionLimit() || (!pObj->~IsSecondaryWeapon() && GetID(GetGear(GEAR_Armor))==G_MS))
        return 1;
      else if(CustomContentsCount("IsWeapon")==1 && ((CustomContentsCount("IsSecondaryWeapon") == 1 && !pObj->~IsSecondaryWeapon()) || (CustomContentsCount("IsSecondaryWeapon") == 0 && pObj->~IsSecondaryWeapon())))
        return 0;
      else if(CustomContentsCount("IsWeapon")==0)
      	return 0;
      else return 1;
    }
    else
      return 0;
  }
  if(pObj ->~ IsEquipment() && ContentsCount(idObj) > 0)
    return 1;
  if((ContentsCount() - (WeaponCollectionLimit() + ObjectCollectionLimit()))>=0)
    return 1;
  
  for(var gear in aGear)
    if(gear)
      if(GetID(gear) == idObj)
        return 1;
  return 0;
}

/*=======Steuerung=======*/

protected func ControlCommand(string strCommand, object pTarget, int iTx, int iTy, object pTarget2, int iData, object pCmdObj){

 if(IsGRAiming())
 {
  if(GetEffect("GrenadeAimer",this))
   RemoveEffect("GrenadeAimer",this);
  if(iTx < GetX())
   SetDir(DIR_Left,this);
  if(iTx > GetX())
   SetDir(DIR_Right,this);
  ActualizePhase();
  var iThrowDir = (-1+2*GetDir(this));
  var AimAngle = -360*(GetDir(this)-1)+iThrowDir*Angle(GetX(),GetY(),iTx,iTy);
  if(AimAngle <= 150 && AimAngle >= 20)
   {
   AddEffect("GrenadeAimer",this,10,1,this,0,AimAngle);
   return 1;
   }
  //Log("%v",AimAngle);
 }
return _inherited(strCommand, pTarget, iTx, iTy, pTarget2, iData, pCmdObj);
}

protected func ControlDigDouble() {
  if (GetAction() == "Push" || IsRiding()) {
    GetActionTarget()->~ControlDigDouble(this());
    return 1;
  }

  if (Control2Grab("ControlDigDouble")) return(0);
  if (Control2Contents("ControlDigDouble")) return(1);
  
  if(IsCrawling())
  {
   return 1;
  }
  
  return(_inherited());
}

protected func ControlDigSingle() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlDig");
  
  if (ControlGrenadeAim("ControlDigSingle")) return 1;
  if (Control2Grab("ControlDig")) return(0);
  if (Control2Contents("ControlDig")) return(1);
  if (ControlLadder("ControlDig") ) return(1);
  
  if(IsCrawling())
  {
   return 1;
  }
  
  return(_inherited());
}

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  if (ControlGrenadeAim("ControlUpdate", comdir, dig, throw)) return 1;
  if(Control2Grab("ControlUpdate", comdir, dig, throw)) return 0;
  if(Control2Contents("ControlUpdate", comdir, dig, throw)) return 1;
  if(ControlAim("ControlUpdate", comdir, dig, throw)) return 1;
  if(ControlLadder("ControlUpdate", comdir, dig, throw)) return 1;

  return(_inherited(clonk, comdir, dig, throw));
}


protected func ControlLeftDouble() {

  if(IsHealing()) {
	  StopHealing();
	  return(1);
  }
  if (ControlGrenadeAim("ControlLeftDouble")) return 1;
  if (ControlAim("ControlLeftDouble")) return 1;
  if (Control2Grab("ControlLeftDouble")) return 0;
  if (Control2Contents("ControlLeftDouble")) return 1;
  if (ControlJetpack("ControlLeftDouble") ) return 1;
  if (ControlAgility("ControlLeftDouble") ) return 1;
  return(_inherited());
}

protected func ControlRightDouble() {

  if(IsHealing()) {
	  StopHealing();
	  return(1);
  }
  if (ControlGrenadeAim("ControlRightDouble")) return 1;
  if (ControlAim("ControlRightDouble")) return 1;
  if (Control2Grab("ControlRightDouble")) return 0;
  if (Control2Contents("ControlRightDouble")) return 1;
  if (ControlJetpack("ControlRightDouble") ) return 1;
  if (ControlAgility("ControlRightDouble") ) return 1;
  return(_inherited());
}

protected func ControlUpDouble() {
  //wenn der Clonk kriecht aufstehen
  if(IsCrawling() && !IsAiming())
  {
   if(PathFree(GetX(this),GetY(this)+5,GetX(this),GetY(this)-5)||Stuck(this))
    StopCrawling();
   return 1;
  }
    
  if (ControlAim("ControlUp")) return(1);
  if (Control2Grab("ControlUpDouble")) return(0);
  if (Control2Contents("ControlUpDouble") ) return(1);
  if (ControlJetpack("ControlUpDouble") ) return(1);
  if (ControlAgility("ControlUpDouble") ) return(1);
  return(_inherited());
}

protected func ControlLeft() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlLeft");

  if (ControlGrenadeAim("ControlLeft")) return 1;
  if (ControlAim("ControlLeft")) return 1;
  if (Control2Grab("ControlLeft")) return 0;
  if (Control2Contents("ControlLeft")) return 1;
  if (ControlLadder("ControlLeft") ) return 1;
  if (ControlJetpack("ControlLeft") ) return 1;
  if (ControlAgility("ControlLeft") ) return 1;

  return(_inherited());
}

protected func ControlRight() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlRight");

  if (ControlGrenadeAim("ControlRight")) return 1;
  if (ControlAim("ControlRight")) return 1;
  if (Control2Grab("ControlRight")) return 0;
  if (Control2Contents("ControlRight")) return 1;
  if (ControlLadder("ControlRight") ) return 1;
  if (ControlJetpack("ControlRight") ) return 1;
  if (ControlAgility("ControlRight") ) return 1;

  return(_inherited());
}

protected func ControlDown() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlDown");
  if (ControlGrenadeAim("ControlDown")) return 1;
  if (ControlAim("ControlDown")) return 1;
  if (Control2Grab("ControlDown")) return 0;
  if (Control2Contents("ControlDown") ) return 1;
  if (ControlLadder("ControlDown") ) return 1;
  if(Contents(0))
    if (GetPlrDownDouble(GetOwner()))
    {
     if(ReadyToGrenadeAim())
     {
         StartGrenadeAim();
         return 1;
     }
     if(ReadyToCrouchAim())
     {
         StartCrouchAiming();
         return 1;
      
     }
		 if(ReadyToSquatAim())
     {
         StartSquatAiming(); 
         return 1;
     }
  return(_inherited());
}
}

protected func ControlUp() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlUp");
  //Interne Commandos wie Springen beim Kriechen umgehen
   if(IsCrawling() && !IsAiming())
  {
   return 1;
  }
  
  if (ControlGrenadeAim("ControlUp")) return 1;
  if (ControlAim("ControlUp")) return 1;
  if (Control2Grab("ControlUp")) return 0;
  if (Control2Contents("ControlUp") ) return 1;
  if (ControlLadder("ControlUp") ) return 1;
  if (ControlAgility("ControlUp") ) return 1;
  return(_inherited());
}

//Rüstung zurückgeben
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

protected func ControlThrow() {
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlThrow");
  //Im Liegen nicht werfen aber nur wenn er nicht zielt
  if(IsCrawling() && !IsAiming())
   return 1;
  // Bei vorherigem Doppel-Stop nur Ablegen  
  if ( GetPlrDownDouble(GetOwner()) )
  {
    AddEffect("SquatAimTimeout", this(), 1, 15, this());
    return(inherited());
  }
  // Steuerung an gerittenes Objekt weitergeben
  if(IsRiding())
    if(GetActionTarget()->~ControlThrow(this()))
      return 1;
  if (ControlGrenadeAim("ControlThrow")) return 1;
  if (Control2Grab("ControlThrow")) return 0;
  if (Control2Contents("ControlThrow") ) return 1;
  if (ControlLadder("ControlThrow") ) return 1;

  return(inherited());
}

/*========InventarWechsel========*/

protected func ControlSpecial()
  {
  [$CtrlInventoryDesc$|Image=INVT]
  if(!Contents()) return;
  // Hardcode: BR-Bombe darf man nicht abwählen
  if(Contents()->GetID() == GBRB)
    return;
  //Beim Granaten Zielen die Granate nicht wegdrücken
  if(IsGRAiming())
  {
  	// nächste Granate suchen
   for(var i = 1; i < ContentsCount(); i++)
  	if(Contents(i)->~IsGrenade())
  	{
  			// zur Granate wechseln
  			ShiftContents(0,0,Contents(i)->GetID(),true);
  			break;
  	}
  }
  // wenn wir zielen, wollen wir nur Waffen haben  
  else
  if(IsAiming() && Contents(0)->~IsWeapon())
  {
   if(IsCrawling())
   {
    for(var i = 1; i < ContentsCount(); i++)
  		if(Contents(i)->~IsWeapon() && !Contents(i)->~CantCrouchAim())
  		{
  			// zur Waffe wechseln
  			ShiftContents(0,0,Contents(i)->GetID(),true);
  			break;
  		}
  	}
   else
   {
  	// nächste Waffe suchen
  	for(var i = 1; i < ContentsCount(); i++)
  		if(Contents(i)->~IsWeapon())
  		{
  			// zur Waffe wechseln
  			ShiftContents(0,0,Contents(i)->GetID(),true);
  			break;
  		}
    }
  }
  else
	  // Inventory verschieben
  	ShiftContents(0,0,0,1);
  UpdateCharge();
  }


/*=======GranatenWurfSystem=======*/

protected func ReadyToGrenadeAim() {
 if(Contained(0)) return false;
 if(GetAction(this) == "Walk" || GetAction(this) == "WalkArmed")
 { 
  if(Contents(0,this)->~IsGrenade())
   return true;
  else
   return false;
 }
}

protected func StartGrenadeAim() {
 SetAction("GrenadeAim",this);
 pGrenCrosshair = CreateObject(GRCH);
 SetOwner(GetController(this),pGrenCrosshair);
 SetAction("Crosshair",pGrenCrosshair,this);
 SetObjDrawTransform(1000,0,0,0,1000,-20000,pGrenCrosshair);
  var iThrowDir = (-1+2*GetDir(this));
  GrenAimAngle = 88;
 SetR(GrenAimAngle*iThrowDir,pGrenCrosshair);
 GrenStrengh = 3;
 PlayerMessage(GetController(this),Format("Power:%d",GrenStrengh*5),this);
 //Effect für die Zielvisualisierung
 AddEffect("GrenParabell",this,1,1,this);
 SetPhase(2);
 return true; 
}

protected func ControlGrenadeAim(string command) {
 if(!IsGRAiming()) return false;
 
 var pGR = Contents(0,this);
 
 if(!pGR)
 {
  SetAction("Walk");
  return true;
 }
 
 if(!pGR->~IsGrenade())
 {
  SetAction("Walk");
  return true;
 }
 
 if(GetEffect("GrenadeAimer",this))
  RemoveEffect("GrenadeAimer",this);
 
 else if(command == "ControlDown")
       AimDown(this,1,"GrenadeConf");
 
 else if(command == "ControlUp")
       AimUp(this,1,"GrenadeConf");

 else if(command == "ControlLeft") {
		if(GetDir() == DIR_Left) return false;
	    SetDir(DIR_Left);
	    ActualizePhase();
	}
	
 else if(command == "ControlRight") {
		if(GetDir() == DIR_Right) return false;
		SetDir(DIR_Right);
	    ActualizePhase();
  }
 else if(command == "ControlThrow")
		ThrowGrenade();
		
 else if(command == "ControlLeftDouble")
		SetAction("Walk");
		
 else if(command == "ControlRightDouble")
		SetAction("Walk");
 
 else if(command == "ControlDigSingle")
 		SetStrengh();
 
 
 else if(command == "ControlUpdate") 
    AimUpdate(this,Par(1),1,"GrenadeConf",GetID());
 
 return true;
}

protected func SetStrengh()
{
 GrenStrengh++;
 if(GrenStrengh >= 6)
  GrenStrengh = 2;
 PlayerMessage(GetController(this),Format("Power:%d",GrenStrengh*5),this);
 return true;
}

protected func GrenadeConf(int Conf, object Controller) {
 if(Conf < 0)
  {
   GrenadeAimUp(Conf,Controller);
  }
 else
  {
   GrenadeAimDown(Conf,Controller);
  }
 
}
protected func IsGRAiming() {
 if(GetAction() == "GrenadeAim")
  return true;
}

//NachUntenZielen
protected func GrenadeAimDown(int iSteps,pTarget) {
 if(GetPlrCoreJumpAndRunControl(GetController(this)))
  var iStepAngle = 2;
 else
  var iStepAngle = 8;
 if(GrenAimAngle >= 110-(iSteps*iStepAngle))
  return false;
 pTarget->ChangeGrenAimAngle(iSteps);
  return true;
}

protected func GrenadeAimAngle(){return GrenAimAngle;}

//NachObenZielen
protected func GrenadeAimUp(int iSteps,pTarget) {
 if(GetPlrCoreJumpAndRunControl(GetController(this)))
  var iStepAngle = 2;
 else
  var iStepAngle = 8;
 if(GrenAimAngle <= 47-(iSteps*iStepAngle))
  return false;
 pTarget->ChangeGrenAimAngle(iSteps);
  return true;
}

//WinkelWechsel
protected func ChangeGrenAimAngle(iSteps) {
 if(GetPlrCoreJumpAndRunControl(GetController(this)))
  GrenAimAngle += iSteps*2;
 else
  GrenAimAngle += iSteps*8;
 ActualizePhase();
 return true;
}

protected func ActualizePhase() {
 var iThrowDir = (-1+2*GetDir(this));
 SetR(GrenAimAngle*iThrowDir,pGrenCrosshair);
 if(GrenAimAngle <= 48)
  return SetPhase(7);
 if(GrenAimAngle <= 56)
  return SetPhase(6);
 if(GrenAimAngle <= 64)
  return SetPhase(5);
 if(GrenAimAngle <= 72)
  return SetPhase(4);
 if(GrenAimAngle <= 80)
  return SetPhase(3);
 if(GrenAimAngle <= 88)
  return SetPhase(2);
 if(GrenAimAngle <= 96)
  return SetPhase(1);
 if(GrenAimAngle > 97)
  return SetPhase(0); 
}

//Das Werfen
protected func ThrowGrenade() {
 var iThrowAngle = GetPhase(this) + 1;
 var iThrowDir = (-1+2*GetDir(this));
 var objGrenade = Contents(0);
 Exit(objGrenade,4*iThrowDir,-3,30*iThrowDir,Sin((GrenAimAngle)*iThrowDir,2*GrenStrengh),-Cos(GrenAimAngle,2*GrenStrengh),10*iThrowDir);
 if(!(objGrenade->~IsUsed()))
  objGrenade->~Active(this);
 SetAction(Format("GrenThrow%d", iThrowAngle));
 return true;
}

protected func FxGrenadeAimerStart(object pTarget, int iEffectNumber, int iTemp,int iTargetAngle) {
EffectVar(1,pTarget,iEffectNumber) = iTargetAngle;
pTarget->~ActualizePhase();
return true;
}

protected func FxGrenadeAimerTimer(object pTarget, int iEffectNumber, int iEffectTime) {
 //nach unten zielen
 if(LocalN("GrenAimAngle",pTarget) < EffectVar(1,pTarget,iEffectNumber) && LocalN("GrenAimAngle",pTarget) <= 113)
 {
  LocalN("GrenAimAngle",pTarget)++;
  pTarget->~ActualizePhase();
  return true;
 }
 //nach oben zielen
 if(LocalN("GrenAimAngle",pTarget) > EffectVar(1,pTarget,iEffectNumber) && LocalN("GrenAimAngle",pTarget) >= 43)
 {
  LocalN("GrenAimAngle",pTarget)--;
  pTarget->~ActualizePhase();
  return true;
 }
 //Werfen
 if(LocalN("GrenAimAngle",pTarget) == EffectVar(1,pTarget,iEffectNumber)|| LocalN("GrenAimAngle",pTarget) < 44 || LocalN("GrenAimAngle",pTarget) > 112)
 {
  pTarget->~ThrowGrenade();
  return -1;
 }
 
}

//Nach dem Werfen
protected func AfterThrowing() {
return SetAction("Walk");
}

protected func AfterAiming() {
 return RemoveObject(pGrenCrosshair);
}

//Effect für die Visualisierung

protected func FxGrenParabellTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
 if(!pTarget->~IsGRAiming())
  return -1;
 var dir = (-1+2*GetDir(pTarget));
 var para = AddParabel(pTarget, GetX(pTarget)+4*dir, GetY(pTarget)-4,dir*LocalN("GrenAimAngle",pTarget), 20*LocalN("GrenStrengh",pTarget));
 SetOwner(GetController(pTarget),para);
 return true;
}

protected func FxGrenParabellStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
 RemoveParabel(pTarget);
 return true;
}

/*===Kriechen und Kriechendzielen===*/

public func ContextStartCrawl(object pClonk) {
[$Crawl$|Condition=ReadyToCrawl]
 SetAction("LayDown",pClonk);
}

//Überprüfen ob er Bereit ist zum Kriechen
protected func ReadyToCrawl() {
//Nur wenn der Clonk läuft oder steht
 if(Contained(0))
  return false;
 if(GetAction() == "Walk" || GetAction() == "WalkArmed")
  return true;
 else
 return false;
} 

//Effecte und ander Notwendige Sachen setzten
protected func StartCrawling() {
 AddEffect("Crawling",this,3,1,this);
 SetAction("Crawl");
 return true;
}

//Überprüfen ob der Clonk Kriecht
protected func IsCrawling() {
 if(GetAction(this) == "Crawl" || GetAction(this) == "AimCrouch")
  return true;
}

//Aufhöhren zu Kriechen
protected func StopCrawling() {
 RemoveEffect("Crawling",this);
 SetAction("FlatUp");
 return true;
}

//Effect fürs Kriechen
protected func FxCrawlingStart(object pTarget, int iEffectNumber, int iTemp) {
 SetPhysical("Walk",12500,2,pTarget);
 //Shape setzen
 SetShape(-8,2,16,8,pTarget);
 //Vertices neu setzen
 SetVertexXY(0, 0,5,pTarget);
 SetVertexXY(1, 0,2,pTarget);
 SetVertexXY(2, 0,9,pTarget);
 SetVertexXY(3,-3,2,pTarget);
 SetVertexXY(4, 3,2,pTarget);
 SetVertexXY(5,-5,3,pTarget);
 SetVertexXY(6, 5,3,pTarget);
}

protected func FxCrawlingTimer(object pTarget, int iEffectNumber, int iEffectTime) {
 //Geschwindigkeit immerwieder anpasssen
 SetPhysical("Walk",12500,2,pTarget);
 //alles was nicht Kriecgeb oder hinlegen ist abbrechen
 if(GetAction()=="Scale")  return SetAction("Crawl"); // Anstoßen an der Wand
 if(GetAction()=="Hangle") return SetAction("Crawl"); // Anstoßen an der Decke
 if(GetAction() == "Walk")   return SetAction("Crawl"); // Mysteriöse Walk-Aktion
 if(GetAction()=="Tumble")
  if(GBackSolid(GetVertex(2, 0, this), GetVertex(2, 1, this) + 2))
 	 return SetAction("Crawl"); // Bei Objekttreffern liegen bleiben
 if(IsCrawling()) return true;
 //Log("Fail");
 return -1;
}


protected func FxCrawlingStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
 //Zurücksetzen der Geschwindigkeit dank dess Weightmasters nicht nötig
 //Shape und Vertices zurücksetzen
 SetShape(-8,-10,16,20,pTarget);
 SetVertexXY(0, 0, 0,pTarget);
 SetVertexXY(1, 0,-7,pTarget);
 SetVertexXY(2, 0, 9,pTarget);
 SetVertexXY(3,-2,-3,pTarget);
 SetVertexXY(4, 2,-3,pTarget);
 SetVertexXY(5,-4, 3,pTarget);
 SetVertexXY(6, 4, 3,pTarget);
 //Log("Test");
}

/*protected func AbortCrawling() 
{
  // Bei manchen Aktionen nicht abbrechen   
  if(GetAction()=="Scale")  return(SetAction("Crawl")); // Anstoßen an der Wand
  if(GetAction()=="Hangle") return(SetAction("Crawl")); // Anstoßen an der Decke
  if(GetAction()=="Tumble") return(SetAction("Crawl")); // Bei Objekttreffern liegen bleiben
  if(GetAction()=="Walk")   return(SetAction("Crawl")); // Mysteriöse Walk-Aktion
  if(IsCrawling())  return(0);  // Immer noch am Kriechen
  // Shape und Vertices zurücksetzen
  StopCrawling();
}*/

protected func ReadyToCrouchAim() {
 return (!IsAiming()
 && IsCrawling()
 && Contents(0)->~CanAim()
 && !Contents(0)->~CantCrouchAim());
}

protected func StartCrouchAiming() {
//Abbrechen wie bei Squad
 if(Contained(0)) return false;
 //Überprüfen ob der Clonk Kriecht
 if(GetAction() != "Crawl") return false;
 //Anhalten und Action setzen
 SetXDir(0);
 SetAction("AimCrouch");
 //Phase setzen
 SetPhase(AimAngle2Phase(90));
 SetComDir(COMD_Stop);
 FxCrawlingStart(this);
 
 InitCrosshair(); 
 ScheduleCall(this(),"UpdateAiming",1);
 
 if(Contents(0)) Contents(0)->~AimStart();
}

//Generrelle Aimingerweiterungen

public func WeaponAt(&x, &y, &r) {

  if(Contained()) return 0;

  var a = GetAction();
  var p = GetPhase();

  // x < 0: Richtung hinter den Clonk
  // x > 0: Richtung vor den Clonk
  x=0; y=0; r=0;

  // hält Waffe in der Hand
  if(a == "WalkArmed") {
    x=0;
    y=1000;
    return 1;
  }
  
  else if(a == "JumpArmed") {
    x=500-125*p;
    y=1000;
    return 1;
  }
  else if(a == "SwimArmed") {
    x=4000;
    return 1;
  }
  //Jetpack
  else if(a == "JetpackFlight") {
  
    x=4000;
    r=35;
    return 1;
  }


  // zielt mit der Waffe
  else if(IsAiming()) {
    //if(p>4) r=(p-4)*11;
    //else    r=p*23-90;
    r = (Abs(crosshair->GetAngle())-90);
    x=-Sin(65+p*20,4000)-1500;
    y=-Sin(55+p*10,5000);
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
    if(a == "AimCrouch")
    {
      x += 11500;
      y += 12000;
    }
    return 1;
  }
 return 0;

}
static const AIM_CrawlingMAX = 130;
static const AIM_CrawlingMIN = 70;

protected func AimMax() {
 if(IsCrawling())
  return AIM_CrawlingMAX;
 else
  return AIM_Max;
}

protected func AimMin() {
 if(IsCrawling())
  return AIM_CrawlingMIN;
 else
  return 0;
}

protected func DoAiming(int iChange) {
  //zielen wir überhaupt?
  if(!IsAiming())
    return;

  var angle = Abs(crosshair->GetAngle()) + iChange;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%AimStep();

  // Winkel wird zu groß?
  if(angle > AimMax() || angle < AimMin())
    return;

  if(GetDir() == DIR_Left)
    angle = 360-angle;
  crosshair->SetAngle(angle);

  UpdateAiming();
}

protected func ControlAim(string command,a,b,c,d,e,f,g,h,i) {

 if(GetEffect("WeaponAimer",this))
  RemoveEffect("WeaponAimer",this);
 return _inherited(command,a,b,c,d,e,f,g,h,i);
}

protected func DoMouseAiming(int iTx, int iTy) {
	if(!Contained()) {

		var iAngle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);

		if(iAngle > 0)
			SetDir(DIR_Right);
		else
			SetDir(DIR_Left);

		//Winkel wird zu groß?
		iAngle = BoundBy(iAngle,-AimMax(),+AimMax());
    if(Inside(iAngle,-AimMin(),+AimMin()))
    {
     if(iAngle > 0)
      iAngle = AimMin();
     else
      iAngle = -AimMin();
    }
    //Winkelprüfen ob schon nah am Zielwinkel drann
     if(Inside(iAngle,crosshair->GetAngle()-5,crosshair->GetAngle()+5))
     {
		crosshair->SetAngle(iAngle);
		UpdateAiming();
		//Wichtig: Waffe updaten
		EffectCall(this,LocalN("wpneffect",this),"Timer");

		// Fertig, Feuern!
		if(!(Contents()->IsRecharging()) && !(Contents()->IsShooting()))
			FireAimWeapon();
		return true;
	 }
	 else
	 {
	  //Effekt für Flüssiges Umschauen
	  AddEffect("WeaponAimer",this,1,1,this,0,iAngle);
	 }
	}
	return false;
}

protected func FxWeaponAimerStart(object pTarget, int iEffectNumber, int iTemp, int iAimAngle)
{
	EffectVar(1,pTarget,iEffectNumber) = iAimAngle;
	return true;
}

protected func FxWeaponAimerTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	var iAimAngle = EffectVar(1,pTarget,iEffectNumber);
	var iTmpAngle = LocalN("crosshair",pTarget)->GetAngle();
	if(iAimAngle > iTmpAngle)
		iTmpAngle +=2;
	else
		iTmpAngle -=2;
	LocalN("crosshair",pTarget)->SetAngle(iTmpAngle);
	pTarget->UpdateAiming();
	if(Inside(iTmpAngle,iAimAngle-1,iAimAngle+1))
		return -1;
	return true;
}

protected func StopAiming() {      // Zielen beenden
  SetComDir(COMD_Stop);
  if(IsCrawling())
   SetAction("Crawl");
  else
  {
  SetAction("Walk");
  }
  this()->~CheckArmed();
  Sound("Grab");
  
  DeleteCrosshair();
 
  //Vertices zurücksetzen
  ResetVertices();
  if(IsCrawling())
   FxCrawlingStart(this);
   
  
  // Callback
  if(Contents(0)) Contents(0)->~AimStop();
  return(1);
}

private func UpdateVertices() {
  var x,y,r;
  this->~WeaponAt(x,y,r);
  if(IsCrawling())
   SetVertexXY(0,x/1000 * (GetDir()*2-1),y/1000,0,0);
  else
  {
   SetVertex(0,0,x/1000 * (GetDir()*2-1),0,2);
   SetVertex(0,1,y/1000,0,2);
  }
}

protected func ReadyToFire() {
 if(IsCrawling() && IsAiming())
  return true;
return _inherited();
}

private func ChangeWeapon(object pTarget) {
    // Zielaktion anpassen
    if(pTarget->~IsWeapon())  {
      var phase = GetPhase();
      if(IsCrawling() && !pTarget->~CantCrouchAim())
        SetAction("AimCrouch");      
 else if(pTarget->~GetFMData(FM_Aim) == 2 || pTarget->~GetFMData(FM_Aim) == -1)
        SetAction("AimSquatLow");
      else
        SetAction("AimSquat");
        
      SetPhase(phase);
    }
    else {
      StopAiming();
    }
}

//Callback für denn Antiwallhack
public func GetBody(int &x, int &y)
{
	if(IsCrawling())
	{
		x = GetX(this);
		y = GetY(this)+6;
	}
	else
	{
		x = GetX(this);
		y = GetY(this);
	}
	return true;
}
