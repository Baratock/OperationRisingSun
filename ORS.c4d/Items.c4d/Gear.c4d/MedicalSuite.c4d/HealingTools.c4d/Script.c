//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Heilungswerkzeuge  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#strict 2
local pMedicalSuite;

public func Initialize(){return AddEffect("Existance",this,1,1,this);}

public func Activate(object pUser)
{
pMedicalSuite->~Heal(pUser);
}

//Sollten sich die Tools im Freien befinden sollen sie verschwinden
public func FxExistanceTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
if(!Contained())return RemoveObject();
if(GetID(Contained()->~GetGear(1))!=G_MS)return RemoveObject();
}
