#strict 2

func Initialize() 
{
  ScheduleCall(this,"remove",1);
  return 1;
}

private func remove()
{
	return RemoveObject();
}
