/*Conquest*/

#strict 2
#include TEAM

local chooser;

protected func Initialize() {
 aTeamKills = [];
 if(FindObject(CHOS)) chooser = true;
  return _inherited();
}

protected func Activate(iPlr) // Defaultnachrichten
{
  if (IsFulfilled())
  {
    if(ObjectCount(V_FP) == TeamFlagCount(GetPlayerTeam(iPlr)))
      return(MessageWindow("$MsgGoalFulfilled$",iPlr));
    else
      return(MessageWindow("$MsgGoalLost$",iPlr));
  }
  return(MessageWindow(Format("$MsgGoalUnfulfilled$",ObjectCount(V_FP) - TeamFlagCount(GetPlayerTeam(iPlr))),iPlr));
}

public func ChooserFinished()
{
  chooser = false;
}

protected func TeamFlagCount(int iTeam) {
 var i, icount;
 var aFlags = FindObjects(Find_ID(V_FP));
 while(i < GetLength(aFlags))
 {
  if(LocalN("iFlaggers",aFlags[i]) == iTeam)
   if(LocalN("iProcess",aFlags[i]) == 120)
    icount++;
  i++;
 }
 return icount;
}

public func IsFulfilled() // Siegreiches Team?
{
 if(chooser) return false;
 for(var team = 0;team < GetTeamCount();team++)
 {
  if(TeamFlagCount(GetTeamByIndex(team)) == ObjectCount(V_FP))
  {
   EliminateLosers();
   Message("@<c %x>$WinMsg$</c>",0,GetTeamColor(GetTeamByIndex(team)),GetTeamName(GetTeamByIndex(team)));
	  
   //Leben noch Verlierer? -> Ligainkompatibilität!
   if(LosersAlive())
    return false;
   
   EvaluationForAll();
   return true;
  }
  
 }
}

private func EliminateLosers()  // Hier trennt sich die Spreu vom Weizen
{
  for(var i=0 ; i < GetPlayerCount() ; i++)
    if(!TeamFlagCount(GetPlayerTeam(GetPlayerByIndex(i))))
      EliminatePlayer(GetPlayerByIndex(i)); // HARHAR
}

//HUD info
public func GetHUDInfo(int player, object hud) {
	var team = GetPlayerTeam(player);
	
	//eigenes Team immer vorne
	if(team && team > 0)
		var sting = Format("<c %x>%d</c>",GetTeamColor(team),TeamFlagCount(team));
	
	var anotherteam;
	for(var i=0; i<GetTeamCount(); ++i) {
		anotherteam = GetTeamByIndex(i);
		if(anotherteam != team)
			sting = Format("%s : <c %x>%d</c>",sting,GetTeamColor(anotherteam),TeamFlagCount(anotherteam));
	}
	
	return sting;
}

protected func CheckFlags() {
 if(chooser) return false;
 for(var i;i < GetTeamCount();i++)
 {
  aPoints[GetTeamByIndex(i)] = TeamFlagCount(GetTeamByIndex(i));
  UpdateScoreboard(GetTeamByIndex(i));
 }
 return true;
}

local aTeamKills;
public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr)
{
 if(iPlr == iMurdererPlr || !Hostile(iPlr,iMurdererPlr))
  aTeamKills[iMurdererPlr]++;
 return _inherited(iPlr, pClonk, iMurdererPlr);
}

public func RemovePlayer(int iPlr)
{
 _inherited(iPlr);
 if(iPlr == -1) return 0;
 aTeamKills[iPlr] = 0;
}
