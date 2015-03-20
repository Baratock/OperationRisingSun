#strict 2

static const BP_Head  = "iHead";
static const BP_Body  = "iBody";
static const BP_LArm  = "iLeftArm";
static const BP_RArm  = "iRightArm";
static const BP_LLeg  = "iLeftLeg";
static const BP_RLeg  = "iRightLeg";

global func DoBodyDmg(int iDmg, int iType, object pTarget, string sBodyTarget,int dmgplayer, bool bWounds)
{
  if(!pTarget)
    if(!(pTarget = this))
      return 0;
  if(!sBodyTarget)var sBodyTarget="iBody";
  if(!bWounds)var bWounds=true;

  var dmgdealer = dmgplayer-1;
  if(dmgdealer < 0)
  	dmgdealer = GetController(pTarget);

  var red = pTarget->~OnDmg(iDmg, iType); //reduction
  var dmg = iDmg*(100-red)/100;
  
  if(this && pTarget->GetOCF() & OCF_CrewMember || dmgplayer)
  {
    DebugLog("DoDmg sets killer: %d, owner of %s", "damage", this->GetOwner(), this->GetName());
    SetKiller(dmgdealer, pTarget);
  }
  //Schaden machen
  var pFrom = this;
  pTarget->~LastDamageType(iType);
  pTarget->~OnHit(dmg/1000, iType, pFrom);
  
  //Blutung verursachen
  if(sBodyTarget=="iHead")var bBlood="iBloodHead";
  if(sBodyTarget=="iLeftArm")var bBlood="iBloodLeftArm";
  if(sBodyTarget=="iRightArm")var bBlood="iBloodRightArm";
  if(sBodyTarget=="iLeftLeg")var bBlood="iBloodLeftLeg";
  if(sBodyTarget=="iRightLeg")var bBlood="iBloodRightLeg";
  if(sBodyTarget=="iBody")var bBlood="iBloodBody";
  if(bBlood)LocalN(bBlood,pTarget)=dmg+pTarget->~GetBodyStatus(sBodyTarget);

  return LocalN(sBodyTarget,pTarget)+=dmg;
}
