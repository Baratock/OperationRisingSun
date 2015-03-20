//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WeightMaster ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#strict 2
local aGear;

protected func Initialize()
{
  AddEffect("WeightMaster",this,1,1,this);//Der neue Effekt um dem Clonk seine Last aufzuerlegen
  return(_inherited());
}

public func FxWeightMasterTimer(pTarget)
{
    //erst werden die Originalwerte genommen
    var walk  = GetPhysical("Walk",1,pTarget);
    var jump  = GetPhysical("Jump",1,pTarget);
    var swim  = GetPhysical("Swim",1,pTarget);
    var hangle= GetPhysical("Hangle",1,pTarget);
    var scale = GetPhysical("Scale",1,pTarget);
 
    //Hat der Clonk nichts, sind die Werte normal
    if(!ContentsCount(0,pTarget) && !pTarget->HasGear())
    {
        SetPhysical("Walk",walk,2,pTarget);
    		SetPhysical("Jump",jump,2,pTarget);
    		SetPhysical("Swim",swim,2,pTarget);
    		SetPhysical("Hangle",hangle,2,pTarget);
    		SetPhysical("Scale",scale,2,pTarget);
    return 1;
    }
    //wir berechnen nun die Masse aller Gegenstände, die er trägt
    var gMasse = 0;
    for(var i=0;i < ContentsCount(0,pTarget); i++) gMasse += GetMass(Contents(i,pTarget));
    
    for (var x = 1; x <= 6; x++)
    {   
        if(GetType(GetGear(x))==C4V_C4Object)gMasse += GetMass(GetGear(x));
    }
    //Masse surlinear machen
    if(gMasse>0)gMasse=Sqrt(gMasse*20+60)-8+(gMasse/40)^5;//(Sqrt(gMasse*50)-8)*4/10+(gMasse/12)^2;
    //zum Schluss Grenzwerte prüfen
    if(gMasse<0)gMasse=0;
    if(gMasse>100)gMasse=100;
    //Die Gesamtmasse gibt den prozentualen Anteil des Abzugs vom Gesamtwert an
    SetPhysical("Walk",walk - walk * gMasse   / 100 ,0,pTarget);
  	SetPhysical("Jump",jump - jump * gMasse/2 / 100 ,0,pTarget);
  	SetPhysical("Swim",swim - swim * gMasse/2 / 100 ,0,pTarget);
  	SetPhysical("Hangle",hangle - hangle * gMasse/2 / 100 ,0,pTarget);
  	SetPhysical("Scale",scale - scale * gMasse/2 / 100 ,0,pTarget);
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
  var gear;
  if(idGear) {
    for(gear in aGear)
	  if(gear)
	    if(GetID(gear) == idGear)
	      return gear;
    return false;
  }
  if(iGearType) {
    if(aGear[iGearType])
      return true;
    return false;
  }
  for(gear in aGear)
    if(gear)
      return true;
  return false;
}

//Identifikationsfunktion
public func HasRealWeight(){return true;}
