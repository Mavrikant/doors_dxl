Module fkt_openModuleForReadByUniqueID_noDisplay(string str_ModuleID_Func){ //Get Module from moduleID

    Module modCurrent = null 
    Item itmCurrent
    string strCurrent = ""

    itmCurrent = itemFromID(str_ModuleID_Func) //Get item from moduleID
                
    if (itmCurrent == null) //If item doesn't exist
        {
            print("Could not get item handle for ID: " str_ModuleID_Func "");
            modCurrent = null
        }
    else //If item exists
        {
            strCurrent = fullName(itmCurrent) //Get fullName of item
            modCurrent = read(strCurrent, false) //Read the module from item
        }

    return(modCurrent)
}

string LinkMod="/Playground/Kennes01/DOORS Links"
Object ot 
Object o

Module source_mod = fkt_openModuleForReadByUniqueID_noDisplay("0004fd73")
Module target_mod = fkt_openModuleForReadByUniqueID_noDisplay("00022261")

for o in source_mod do{
	if(isSelected(o)){
		for ot in target_mod do{
			if(isSelected(ot)){
				o -> LinkMod -> ot
				break
			}
		}
	}
}