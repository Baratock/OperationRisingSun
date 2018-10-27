xcopy ORS.c4d\* tmp\ORS.c4d /s /Y
xcopy ORS.c4f\* tmp\ORS.c4f /s /Y

"../Clonk Rage/c4group" tmp\ORS.c4d -p
"../Clonk Rage/c4group" tmp\ORS.c4f -p
pause