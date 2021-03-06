/*-- Quickmenu--*/

#strict 2

//Statics
static const QMEN_Throw = 0;
static const QMEN_Up	= 1;
static const QMEN_Right	= 2;
static const QMEN_Down	= 3;
static const QMEN_Left	= 4;

static const QMEN_Animation = 5;
static const QMEN_RingDistance = 45;
//Locals
local pTarget;
local pCommandObj;

local aItemIcon;
local aItemBackground;
local aItemCommand;
local aItemPar;
local aItemExists;

//Value Callbacks
public func MaxItems()	{	return 5;	}

public func PosX(int i)
{
	if(i == QMEN_Throw)
		return 0;
	if(i == QMEN_Up)
		return 0;
	if(i == QMEN_Down)
		return 0;
	if(i == QMEN_Left)
		return -QMEN_RingDistance;
	if(i == QMEN_Right)
		return QMEN_RingDistance;
}
public func PosY(int i)
{
	if(i == QMEN_Throw)
		return 0;
	if(i == QMEN_Up)
		return -QMEN_RingDistance;
	if(i == QMEN_Down)
		return QMEN_RingDistance;
	if(i == QMEN_Left)
		return 0;
	if(i == QMEN_Right)
		return 0;
}

//------------------Important----------------------------

//global method to Create the Menu on a specific object
global func CreateQuickMenu(object CommandObj, object pClonk)
{
	if(!CommandObj)
		CommandObj = this;
	if(!pClonk)
		pClonk = this;
		
	//Menu erstellen und Grundeinstellungen vornehmen
	var pMenu = CreateObject(QMEN, AbsX(GetX(pClonk)), AbsY(GetY(pClonk)), GetOwner(pClonk));
	pMenu->Create(CommandObj,pClonk);
	return pMenu;
	
}

//Add one Item to Menu Mechanic
public func Add(int Index, id IconID, id BackgroundID, string szCommand, Parameter,  string szBGName)
{
	if(Index >= MaxItems())
		return false;
		
	//Add all Data
	aItemIcon[Index]		= IconID;
	aItemBackground[Index]	= BackgroundID;
	aItemCommand[Index]		= szCommand;
	aItemPar[Index]			= Parameter;
	aItemExists[Index]		= true;
	
	OnAdd(Index, IconID, BackgroundID, szCommand, Parameter, szBGName);
	return true;
}

public func Initialize()
{
	aItemIcon = CreateArray();
	aItemBackground = CreateArray();
	aItemCommand = CreateArray();
	aItemPar = CreateArray();
	aItemExists = CreateArray();
}

public func Create(object CommandObj, object pClonk)
{
	pTarget = pClonk;
	pCommandObj = CommandObj;
	//Sicht und Steuerung auf Menuumsetzten: Aus dem Magierkombomenu gemobbst
	SetCursor(GetOwner(), this, 1, 1);
	SelectCrew(GetOwner(), pTarget, 0, 1);
	SetViewCursor(GetOwner(), pTarget);
	
	SetVisibility(VIS_Owner);
	//SelctionCheck Effect
	SetAction("Opening");
}

//What happens whem Item are added, can differ if there are other Menus
protected func OnAdd(int Index, id IconID, id BackgroundID, string szCommand, Parameter,  string szBGName)
{
	
	var i = 2*Index+1;
	var j = 2*Index+2;
	if(IconID)
		SetGraphics(0, this, IconID, j, GFXOV_MODE_IngamePicture);
	if(BackgroundID)
		SetGraphics(szBGName, this, BackgroundID, i, GFXOV_MODE_IngamePicture);
	if(GetAction() == "Opening")
		Opening();
	if(GetAction() == "Closing")
		Closing();
		
}


//scaling of the UI in 1/1000
protected func ScaleMenu(int scale)
{
	//UI
	SetObjDrawTransform(scale, 0, 0, 0, scale, 0, this, 0);
	
	//Element 1 -5
	for(var a = 0; a < 5; a++)
	{
		var IconID = aItemIcon[a];
		var BackgroundID = aItemBackground[a];
	
		var sc = scale/2;
		
		var i = 2*a+1;
		var j = 2*a+2;
		//Scaling for Icons
		var w = GetDefCoreVal("Picture", "DefCore", IconID, 2);
		var h = GetDefCoreVal("Picture", "DefCore", IconID, 3);
		var s = Max(h, w);
		var x = sc*PosX(a);
		var y = sc*PosY(a);
		SetObjDrawTransform(sc*s/160,0,x, 0, sc*s/160,y, this, j);
		
		w = GetDefCoreVal("Picture", "DefCore", BackgroundID, 2);
		h = GetDefCoreVal("Picture", "DefCore", BackgroundID, 3);
		s = Max(h, w);
		var x = sc*PosX(a);
		var y = sc*PosY(a);
		SetObjDrawTransform(sc*s/160,0,x, 0, sc*s/160,y, this, i);
	}
}

//Activate Selected Item
public func ActivateItem(int Index)
{
	if(GetAction() == "Closing")
		return;
	//Look if Command  is Valid
	if(aItemExists[Index] && aItemCommand[Index] && GetType(aItemCommand[Index]) == C4V_String)
		pCommandObj->Call(aItemCommand[Index], aItemPar[Index]);
	Close();
}


//EffectTimer for Checking if the Menu is Still in use
public func InUse()
{
	if(this != GetCursor(GetOwner(pTarget)))
	{
		Close();
		return -1;
	}
	return true;
}

public func Closing()
{
	var t = GetActTime();
	var i = QMEN_Animation -t;
	var s = i*1000/QMEN_Animation;
	ScaleMenu(s);
	//Close after Animation
	if(t >= QMEN_Animation)
		RemoveObject();
}

public func Opening()
{
	var t = GetActTime();
	var s = t*1000/QMEN_Animation;
	ScaleMenu(s);
	//Finish after Animation
	if(t >= QMEN_Animation)
		SetAction("InUse");
}

// Close Menu
public func Close()
{
	if(GetAction() == "Closing")
		return;
	ClearLastPlrCom();
	if(pTarget)
		SetCursor(GetOwner(), pTarget, 1, 1);
	SetAction("Closing");
	return;
}


//-------------------------Controls-----------------------------

public func ControlDig()		{	Close();	}
public func ControlSpecial()	{	Close();	}
public func ControlSpecial2()	{	Close();	}

public func ControlThrow()		{	ActivateItem(QMEN_Throw);	}
public func ControlUp()			{	ActivateItem(QMEN_Up);	}
public func ControlDown()		{	ActivateItem(QMEN_Down);	}
public func ControlLeft()		{	ActivateItem(QMEN_Left);	}
public func ControlRight()		{	ActivateItem(QMEN_Right);	}


