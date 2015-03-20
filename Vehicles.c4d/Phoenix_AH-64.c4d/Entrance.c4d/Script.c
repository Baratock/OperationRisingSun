//--------------------------------------------- Entry ---------------------------------------------
#strict 2

local pCreator;

public func Construction (object creator)
{
pCreator = creator;
}

protected func ActivateEntrance(object by) // Eingang
{   
    if (by->GetOCF() & OCF_CrewMember) 
    {
        if(LocalN("Pilot",pCreator)== 0) 
        {
            Enter(pCreator, by);
            SetController(GetController(by),pCreator);
            LocalN("Pilot",pCreator) = FindObject2(Find_Container(pCreator),Find_OCF(OCF_CrewMember),Find_Exclude(LocalN("Gunner",pCreator)));
            FinishCommand(by,true);
            return 1;
        }else if(LocalN("Gunner",pCreator)== 0)
        {
            Enter(pCreator, by);
            LocalN("Gunner",pCreator) = FindObject2(Find_Container(pCreator),Find_OCF(OCF_CrewMember),Find_Exclude(LocalN("Pilot",pCreator)));
            FinishCommand(by,true);
            return 1;
        }else return 0;
     }
}

public func AttachTo(object pBy) 
{
	SetAction("Attach",pBy);
}
