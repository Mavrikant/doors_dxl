/*

 \file    Baselinesets.inc

 \brief   This include contains the functions for Baselinesets

 \author  muders01

 \date    06.02.2014

 \note    (C) Copyright Leopold Kostal GmbH&Co KG \n\n
          Contents and presentations are protected world-wide.\n
          Any kind of using, copying etc. is prohibited without prior permission. \n
          All rights - incl. industrial property rights - are reserved.

*/

DB dbBLSGui = null
DBE dbeBLSFolder,dbeBLSTreeView
//Dialogbox to show the baselinesetDefinition
DB dbBLSShowBaselinesetDefinition
DB dbBLSCreateBaselineset
DBE dbeBLSCloseBaselineset
DBE dbeBLSDefModuleList
DBE dbeBLSDefSearchString
DBE dbeBLSMajor
DBE dbeBLSMinor
DBE dbeBLSSuffix
DBE dbeBLSDescription
DBE dbeBLSCreateBaselinesetIncrease
DBE dbeBLSCreateShowBaselinsets
DBE dbeBLSCreateStoreLinks,dbeBLSCreateUpdateHistory, dbeBLSCreateUpdateCustomerAttribute, dbeBLSCreateUpdateBLS,dbeBLSCreateUpdateImplState
DB dbBLSCreateDefinition
DBE dbeBLSCreateName
DBE dbeBLSCreateDesc
string sBLSDefinitionName =""
string sBLSDefinitionDescription =""

BaselineSetDefinition blsdefBLSSelected = null

Skip skBSLBaselineSets = createString
Skip skBSLDefinition = createString
Skip skBSLStoredModule = createString

int iBLSSearchCounter = 0

/* This function is Adding the selected Module to the BaselinesetDefinition
parameter:
	BaselineSetDefinition blsdefTemp:	The Definition, where the module should be added
	ModName_ mn:				The reference to the module
return string: empty, if succcesful: else contains the error message */
string  sBLSDefAddModule(BaselineSetDefinition blsdefTemp , ModName_ mn)
{
	string sRetVal = ""
	//basic checks
	if((!null blsdefTemp) && (!null mn))
	{
		string sError =""
                //add the module to the definition
		sError = addModule(mn, blsdefTemp)
		if(!null(sError)) sRetVal = sRetVal sError
	}
	return sRetVal
}

/* This function is deleting the selected Module to the BaselinesetDefinition
parameter:
	BaselineSetDefinition blsdefTemp:	The Definition, where the module should be deleted
	ModName_ mn:				The reference to the module
return string: empty, if succcesful: else contains the error message */
string sBLSDefDeleteModule(BaselineSetDefinition blsdefTemp , ModName_ mn)
{
	string sRetVal = ""
	//basic checks
	if((!null blsdefTemp) && (!null mn))
	{
		string sError
                //add the module to the definition
		sError = removeModule(mn, blsdefTemp)
		if(!null(sError)) sRetVal = sRetVal sError
	}
	return sRetVal
}

/* This function is locking the selected the BaselinesetDefinition
parameter:
	BaselineSetDefinition blsdefTemp:	The Definition, where the module should be deleted
return string: empty, if succcesful: else contains the error message */
string sBLSDefLock(BaselineSetDefinition blsdefTemp)
{
	string sRetVal = ""
	//basic checks
	if(!null blsdefTemp)
	{
		string sError
		//lock the Definition
		sError = lock(blsdefTemp)
		if(!null(sError)) sRetVal = sRetVal sError
	}
	return sRetVal
}

/* This function is unlocking the selected the BaselinesetDefinition
parameter:
	BaselineSetDefinition blsdefTemp:	The Definition, where the module should be deleted
return string: empty, if succcesful: else contains the error message */
string sBLSDefUnLock(BaselineSetDefinition blsdefTemp)
{
	string sRetVal = ""
	//basic checks
	if(!null blsdefTemp)
	{
		string sError
		//lock the Definition
		sError = unlock(blsdefTemp)
		if(!null(sError)) sRetVal = sRetVal sError
	}
	return sRetVal
}

/* This function is storing the BaselinesetDefinition
parameter:
	BaselineSetDefinition blsdefTemp:	The Definition, where the module should be deleted
return string: empty, if succcesful: else contains the error message */
string  sBLSDefSaveDefinition(BaselineSetDefinition blsdefTemp)
{
	string sRetVal = ""
	//basic checks
	if(!null blsdefTemp)
	{
		string sError
		sError = save(blsdefTemp)
		if(!null(sError)) sRetVal = sRetVal sError
	}
	return sRetVal
}

/* This function is updating the String for the Treeview
parameter:
	DBE dbeTemp:	Dialogbox (in this case a button, which is starting the Export

global parameter
	DBE dbeBLSFolder: The field to put the information.

return - */
void vBLSEvaluateTreeviewString(DBE dbeTemp, Folder fTreeView)
{
	// clear existing structure
    	empty(dbeTemp)
    	//initial check
    	if(null fTreeView) return
	string sTreeView  = name (fTreeView)
	string sTreeViewSeperator = "/"
	insert(dbeTemp, sTreeView, iconFolder, iconFolderOpen)

	skBSLBaselineSets = createString
	skBSLDefinition = createString
	skBSLStoredModule = createString
	//find the definitions
	BaselineSetDefinition bSDef
	for bSDef in fTreeView do{
		string sBSDName =  name(bSDef)
		string sInsertString = sTreeView sTreeViewSeperator sBSDName
		insert(dbeTemp,sInsertString , iconFolder, iconFolderOpen)
		put(skBSLDefinition , sInsertString , bSDef)

		//now find the baselines of the definitions
		BaselineSet  bls = null
		for bls in bSDef do{
			string sVersion = versionID(bls)
			string 	sTemp = sInsertString sTreeViewSeperator sVersion
			bool bIsOpen = isOpen(bls)
			if(bIsOpen)  sTemp = sTemp " (Open)"
			insert(dbeTemp, sTemp , iconFolder, iconFolderOpen)
			put(skBSLBaselineSets , sTemp , bls)
			put(skBSLDefinition , sTemp , bSDef)
			//
			Skip skShownModuleList = createString
			//add the info of the included modules
			ModuleVersion mv
	         	for mv in bls do
        	        {
                		string sModule = name(mv) " [" (versionString mv) "]"
                		insert(dbeTemp, sTemp sTreeViewSeperator sModule, iconFormal, iconFormal)
                		put(skBSLStoredModule,sTemp sTreeViewSeperator sModule, mv)
                		put(skShownModuleList,name(mv), name(mv))
               	 	}
               	 	//now check the deleted modules
               	 	ModName_ mnTemp = null
			for mnTemp in bSDef do{
				//print sTemp "\n"
				//replace the string by the key?
				string sModule = name(mnTemp)
				if(!find(skShownModuleList,sModule,sModule))
				{
					if(isDeleted(mnTemp)) insert(dbeTemp, sTemp sTreeViewSeperator sModule, iconFormalDeleted, iconFormalDeleted)
				}
			}
			delete skShownModuleList
		}
	}
}

/* This function is calling the browser to select the project
parameter:
	DBE dbeTemp:	Dialogbox (in this case a button, which is starting the Export

global parameter
	DBE dbeBLSFolder: The field to put the information.

return - */
void vBLSDoFolderBrowse(DBE dbeTemp)
{
	string sTemp = sMEExplorer(dbBLSGui ,
                                current Folder,
                                MINI_EXP_DESCRIPTIVE_MODS|MINI_EXP_PROJECT)
        if(!isVoid_(sTemp))
        {
        	//check a change in the Folder
        	string sOldFolderName = get(dbeBLSFolder)
        	if(sOldFolderName != sTemp)
        	{
        		//project has changed
        	    	set(dbeBLSFolder, sTemp)
 		       	//reset the selected Baselineset
	        	blsdefBLSSelected = null
		    	Folder fTemp = folder(sTemp)
    			vBLSEvaluateTreeviewString(dbeBLSTreeView,fTemp)
    		}
    	}
}

/* This function is called during selection an treeview element
parameter:
	DBE dbeTemp:	Dialogbox (in this case a treeview), which is firing the callback

global parameter -

return - */
void vBLSonSelection(DBE dbeTemp)
{
	string sSelected = get dbeTemp
	//first search the definition
	BaselineSetDefinition blsDefTemp
	if (find(skBSLDefinition, sSelected , blsDefTemp ))
	{
		blsdefBLSSelected = blsDefTemp
	}
	//Check the baselinesets
	BaselineSet  bls = null
	if(find(skBSLBaselineSets,sSelected,bls))
	{
		bool bIsOpen = isOpen(bls)
		//activate the button
		if(bIsOpen)active(dbeBLSCloseBaselineset)
		else inactive(dbeBLSCloseBaselineset)
	}
}

/*
This function is called during closing the GUI
parameter:
	DB dbTemp: 	The Dialogbox, which is calling the GUI
return */

void vBLSDoClose(DB dbTemp)
{
	//firts unlock the selected BLS, if necessary
	if(!null blsdefBLSSelected) unlock (blsdefBLSSelected)

	release(dbTemp)
	hide dbTemp
	dbTemp = null
}

/*
This function is called during closing the GUI
parameter:
	DBE dbeTemp: 	The Dialogbox, which is calling the GUI
return */

void vBLSCreateDoClose(DBE dbeTemp)
{
	//firts unlock the selected BLS, if necessary
	if(!null blsdefBLSSelected) unlock (blsdefBLSSelected)

	release(dbBLSCreateBaselineset)
	hide dbBLSCreateBaselineset
//	dbBLSCreateBaselineset = null
}

/*
This function is updateing the listview for the modules
parameter:
	DBE dbeTemp: The dialogboxelement, which is calling this function
return:
	-
*/
void vBLSDefShowUpdateModuleList(DBE dbeTemp,BaselineSetDefinition blsdefTemp , bool bShowFullPath)
{
	//clear the Listview
	empty(dbeTemp)
	//running variable
	int iMEAmountOfModules = 0
	Module mTemp = null
	ModName_ mnTemp = null
	for mnTemp in blsdefTemp do{
		//print sTemp "\n"
		//replace the string by the key?
		if(bShowFullPath != true) insert(dbeTemp, iMEAmountOfModules, name(mnTemp) )
		else insert(dbeTemp, iMEAmountOfModules, fullName(mnTemp) )
		iMEAmountOfModules++
	}
}

/*
This function is called to select the modules
parameter:
	DBE dbeTemp: 	The Dialogbox, which is calling the GUI

return
	- */
void vBDSDDefDoModuleBrowse(DBE dbeTemp)
{
	// get the module from the Explorer
/*	string sModName = sMEExplorer(null,
	                                current Folder,
        	                        MINI_EXP_FORMAL_MODS)*/
 	//reset the modulelist
 	skTemp = createString
 	//check a change in the Folder
	string sFolder = get(dbeBLSFolder)
	if(!isVoid_(sFolder))
	{
    		Folder fTemp = folder(sFolder)
 	        skTemp = skMEExplorer(dbBLSShowBaselinesetDefinition, fTemp, MINI_EXP_FORMAL_MODS|MINI_EXP_PROJECT|MINI_EXP_FOLDER)
		//print "Modules found: \n"
		string sTemp =""
		string sError = ""
		sError = sError sBLSDefLock(blsdefBLSSelected)
		for sTemp in skTemp do{
			sError = sError sBLSDefAddModule(blsdefBLSSelected, module sTemp)
		}
		//store the Definition
		sError = sError sBLSDefSaveDefinition(blsdefBLSSelected)
		sError = sError sBLSDefUnLock(blsdefBLSSelected)
		if(sError != "") ack sError
		vBLSDefShowUpdateModuleList(dbeBLSDefModuleList,blsdefBLSSelected,true)
        }
	delete skTemp
//	vBLSDefShowUpdateModuleList(dbeBLSDefModuleList,true)
}
/*
This function is called to delete modules from the baselinset
parameter:
	DBE dbeTemp: 	The Dialogbox, which is calling the GUI

return
	- */
void vBDSDDefDeleteModule(DBE dbeTemp)
{
	int iSelected = 0
	//check the ListView of the Modulelist
	string sError = ""
	sError = sError sBLSDefLock(blsdefBLSSelected)
	for iSelected in dbeBLSDefModuleList do
	{
		if (selected(dbeBLSDefModuleList, iSelected))
		{
			string sTemp = getColumnValue(dbeBLSDefModuleList,iSelected,0)
			sError = sError sBLSDefDeleteModule(blsdefBLSSelected,module sTemp)
		}
	}
	//store the Definition
	sError = sError sBLSDefSaveDefinition(blsdefBLSSelected)
	sError = sError sBLSDefUnLock(blsdefBLSSelected)
	if(sError != "") ack sError
	vBLSDefShowUpdateModuleList(dbeBLSDefModuleList,blsdefBLSSelected,true)
}
/*
This function is called to find a module in the baselinset
parameter:
	DBE dbeTemp: 	The Dialogbox, which is calling the GUI
return	- */
void vBDSDDefFindNext(DBE dbeTemp)
{
	//first get the content of the searchfiela
	string sSearch = get(dbeBLSDefSearchString)
	if(isVoid_(sSearch)) return
	set(dbeBLSDefModuleList,iBLSSearchCounter,false)
	//loop through the List View
	int iNumberOfElements = noElems(dbeBLSDefModuleList)
	string sTemp= ""
	bool bTotalLoop = false
	int iCountedElements = 0
	while (bTotalLoop == false){
		//for the complete loop
		iCountedElements++
		if(iCountedElements > iNumberOfElements) {
			bTotalLoop = true
			ack "String not found!"
		}
		iBLSSearchCounter++
		if(iBLSSearchCounter >= iNumberOfElements){
                        ack "End of list reached!"
			iBLSSearchCounter = 0
		}
		sTemp = getColumnValue(dbeBLSDefModuleList,iBLSSearchCounter,0)
		if(bFindString(sTemp,sSearch,false)) {
			set(dbeBLSDefModuleList,iBLSSearchCounter,true)
			bTotalLoop = true
		}
	}
}

/* This function is showing the content of the baselineset
parameter:
	BaselineSetDefinition blsdefTemp:	The Definition, where the modules should be shown
return - */
void vBLSDefShowBaselinesetDefinition(BaselineSetDefinition blsdefTemp)
{
	dbBLSShowBaselinesetDefinition= create (dbBLSGui,"Show  Modules of the Baselinesets",styleCentered )
	close (dbBLSShowBaselinesetDefinition,true,vBLSDoClose)

	//Add the frames to the dialogbox
//	dbeBLSFrameModule= frame(dbBLSShowBaselinesetDefinition ,"Modules", 650,250)
/*		dbeBLSFrameModule ->"top"->"form"
		dbeBLSFrameModule ->"left"->"form"
		dbeBLSFrameModule ->"right"->"form"
		dbeBLSFrameModule ->"bottom"->"unattached"
*/
		//Add the frames to the dialogbox
	dbeBLSFrameButton = frame(dbBLSShowBaselinesetDefinition ,"Buttons", 650,50)
		dbeBLSFrameButton ->"top"->"unattached"
		dbeBLSFrameButton ->"left"->"form"
		dbeBLSFrameButton ->"right"->"form"
		dbeBLSFrameButton ->"bottom"->"form"
	dbeBLSFrameModule= frame(dbBLSShowBaselinesetDefinition ,"Modules", 650,250)
		dbeBLSFrameModule ->"top"->"form"
		dbeBLSFrameModule ->"left"->"form"
		dbeBLSFrameModule ->"right"->"form"
		dbeBLSFrameModule ->"bottom"->"flush"->dbeBLSFrameButton

	//now configure the DBES
       	string saValue[] = {""}
      	//ListView of the modules
      	dbeBLSDefModuleList = listView(dbBLSShowBaselinesetDefinition,listViewOptionMultiselect ,400,15,saValue)
      		dbeBLSDefModuleList -> "top" -> "inside" -> dbeBLSFrameModule
    		dbeBLSDefModuleList -> "left" -> "inside" -> dbeBLSFrameModule
     		dbeBLSDefModuleList -> "right" -> "inside" -> dbeBLSFrameModule
      		dbeBLSDefModuleList -> "bottom" -> "inside" -> dbeBLSFrameModule

	dbeBLSDelete = button(dbBLSShowBaselinesetDefinition, "Delete", vBDSDDefDeleteModule)
     		dbeBLSDelete -> "top" -> "inside" -> dbeBLSFrameButton
      		dbeBLSDelete -> "left" -> "inside" -> dbeBLSFrameButton
      		dbeBLSDelete -> "right" -> "unattached"
      		dbeBLSDelete -> "bottom" -> "inside" -> dbeBLSFrameButton
      	dbeBLSDef = button(dbBLSShowBaselinesetDefinition, "Browse...", vBDSDDefDoModuleBrowse)
	//dbeBLSDelete = button(dbBLSShowBaselinesetDefinition, "Delete", vBDSDDefDeleteModule)
		dbeBLSDef -> "left" -> "unattached"
		dbeBLSDef -> "right" -> "inside" -> dbeBLSFrameButton
		dbeBLSDef -> "top" -> "aligned" -> dbeBLSDelete
		dbeBLSDef -> "bottom" -> "inside" -> dbeBLSFrameButton
	dbeBLSDefFindNext = button(dbBLSShowBaselinesetDefinition, "Find Next", vBDSDDefFindNext)
		dbeBLSDefFindNext -> "left" -> "unattached"
		dbeBLSDefFindNext -> "right" -> "spaced" -> dbeBLSDef
		dbeBLSDefFindNext -> "top" -> "aligned" -> dbeBLSDef
		dbeBLSDefFindNext -> "bottom" -> "inside" -> dbeBLSFrameButton
	dbeBLSDefSearchString = field(dbBLSShowBaselinesetDefinition, "Find", "" , 30, false)
		dbeBLSDefSearchString -> "left" -> "unattached"
		dbeBLSDefSearchString -> "right" -> "spaced" -> dbeBLSDefFindNext
		dbeBLSDefSearchString -> "top" -> "aligned" -> dbeBLSDefFindNext
		dbeBLSDefSearchString -> "bottom" -> "inside" -> dbeBLSFrameButton
      	realize dbBLSShowBaselinesetDefinition
      	insertColumn(dbeBLSDefModuleList , 0, "Module", 630,iconNone)
      	vBLSDefShowUpdateModuleList(dbeBLSDefModuleList,blsdefTemp,true)
	// raise the GUI
	block(dbBLSShowBaselinesetDefinition)
	destroy(dbBLSShowBaselinesetDefinition)
}

/* This function is called after activation of an treeview element
parameter:
	DBE dbeTemp:	Dialogbox (in this case a treeview), which is firing the callback

global parameter -

return - */
void vBLSonActivate(DBE dbeTemp)
{
	string sSelected = get dbeTemp
	//print"on Activation " "\t s: " sSelected "\n"
	//first search the definition
	BaselineSetDefinition blsDefTemp
	if (find(skBSLDefinition, sSelected , blsDefTemp ))
	{
		blsdefBLSSelected = blsDefTemp
		//Configure the BaselineseDefinition
		vBLSDefShowBaselinesetDefinition(blsDefTemp)
	}

         ModuleVersion mv
         if(find(skBSLStoredModule,sSelected,mv))
         {
         	// open the module
		Module mTemp = load(mv, true)
//		print "    " (fullName mv) " [" (versionString mv) "]" "\n"
         }
}


/* This function is called during the opening of an treeview element
parameter:
	DBE dbeTemp:	Dialogbox (in this case a treeview), which is firing the callback

global parameter -

return - */
bool bBLSdoExpandDirectory(DBE dbeTemp, string sTreeEntry)
{
	//print sTreeEntry
	return(true)
}

/*
This function is updateing the listview for the Baselinesets
parameter:
	DBE dbeTemp: The dialogboxelement, which is calling this function
return:
	-
*/
void vBLSCreateShowbaselineset(DBE dbeTemp,BaselineSetDefinition blsdefTemp)
{
	//clear the Listview
	empty(dbeTemp)

	//running variable
	int iMEAmountOfModules 	= 0
	BaselineSet  bls 		= null
	
	for bls in blsdefTemp do{
		string sVersion = versionID(bls)
		bool bIsOpen = isOpen(bls)
		string sTemp = ""
		if(bIsOpen)  sTemp = "(Open)"
		insert(dbeTemp, iMEAmountOfModules, sVersion )
		set(dbeTemp, iMEAmountOfModules, 1, sTemp )
		iMEAmountOfModules++
	}

	if(!null bls)
	{
		set(dbeBLSSuffix,suffix(bls)"")
		set(dbeBLSDescription,annotation(bls))
	}
}

/* This function is called after the BaselinesetDefinition was created. The Definition is created and stored.
parameter:
	DBE dbeTemp:	Dialogbox, which is firing the callback

global parameter -

return - */
void vBLSDoUpdateValues(DBE dbeTemp)
{
        sBLSDefinitionName = get(dbeBLSCreateName)
        sBLSDefinitionDescription = get(dbeBLSCreateDesc)
	if(!isVoid_(sBLSDefinitionName))
	{
		//get the folder
		string sFolder = get(dbeBLSFolder)
		if(!isVoid_(sFolder))
		{
			Folder fTemp = folder(sFolder)
			BaselineSetDefinition  bsd = null
			string sMessage = create(fTemp,sBLSDefinitionName,sBLSDefinitionDescription,bsd)
		}
	}
	vBLSDoClose(dbBLSCreateDefinition)
}
/* This function is creating a new baselineset Definito of the selected Baselineset
parameter:
	DBE dbeTemp:	Dialogbox, which is firing the callback

global parameter -

return - */
void vBLSCreateDefinition(DBE dbeTemp)
{
	//raise the GUI
	dbBLSCreateDefinition= create (dbBLSGui,"Create Baselineset Definition",styleCentered )
	close (dbBLSCreateDefinition,false,vBLSDoClose)

	dbeBLSCreateName = field(dbBLSCreateDefinition, "Name","",50)
	dbeBLSCreateDesc = field(dbBLSCreateDefinition, "Description","",50)

	dbeBLSCreateClose = button(dbBLSCreateDefinition,"Close",vBLSDoUpdateValues,false)

      	//Create the GUI
      	realize dbBLSCreateDefinition
	// raise the GUI
	block(dbBLSCreateDefinition)
	destroy(dbBLSCreateDefinition)
	//update the Treeview
	//Get the content of the project field
	string sFolder = get(dbeBLSFolder)
	if(!isVoid_(sFolder))
	{
    		Folder fTemp = folder(sFolder)
		vBLSEvaluateTreeviewString(dbeBLSTreeView, fTemp)
	}
}

/* This function is creating a new baseline of the selected Baselineset
parameter:
	DBE dbeTemp:	Dialogbox, which is firing the callback

global parameter -

return - */
void vBLSCreateDoCreate(DBE dbeTemp)
{

	int iIncrease 		= get(dbeBLSCreateBaselinesetIncrease)
	bool bMajorIncrease = false
	if (iIncrease == 0)  bMajorIncrease = true
	
    string sNewSuffix 		= get(dbeBLSSuffix)
	string sNewDescription 	= get(dbeBLSDescription)
	bool bStoreLinks 		= get(dbeBLSCreateStoreLinks)
	bool bUpdateHistory 	= get(dbeBLSCreateUpdateHistory)
	bool bUpdateBaselineset = get(dbeBLSCreateUpdateBLS)
	bool bUpdateImplementationState = get(dbeBLSCreateUpdateImplState)
	bool bUpdateCustomerAttributes 	= get(dbeBLSCreateUpdateCustomerAttribute)
	
	//print "iIncrease: " iIncrease "\n"
	//if no baselineset is selected -> exit the function without doing anything
	if(null blsdefBLSSelected) return

	BaselineSet  bls = null
	//find the last bls in the definition
	for bls in blsdefBLSSelected do{
	
	}
	
	string sError = null
	string str_baseline_error = null 
	
    if(bUpdateBaselineset != false)
    {
		if((null bls)  || !isOpen(bls))
		{
			//create a new baselineset, when the old one is open.
			sError = create(blsdefBLSSelected,
				bMajorIncrease,
				sNewSuffix,
				sNewDescription,
				bls)
		}
	}
	
	//no error available
	if(null sError)
	{
		//Create a Skiplist to get the modules, which are already stored
		Skip skModulesInBLS = createString
		
		//only if the baselineset is available
		if(!null bls)
		{
			ModuleVersion mv
			for mv in bls do{
				//print "MV: "  fullName(mv) "\n"
				put(skModulesInBLS,fullName(mv),fullName(mv))
			}
		}
		
		//no error occured -> add all modules to the baselineset
		//create first the Skiplist for the baselineset
		Skip skModuleList		= create
		int skipIndex			= 0
		int iAmountOfModules	= 0
		int iCountModules 		= 1

		//count the Amount of Modules
		ModName_ mnTemp = null

		for mnTemp in blsdefBLSSelected do{
			iAmountOfModules++
		}
		
		//open all modules
		progressStart(dbBLSCreateBaselineset,  "Baselineing of " iAmountOfModules " Modules", "Something", iAmountOfModules)

		bool bIsCanceled = false
		mnTemp = null
		
		//now open all modules of the Baselinesetdefinition
		for mnTemp in blsdefBLSSelected do{
			if (bIsCanceled != false) continue
			//Is the module already available in the bls -> don't store again.
			string sModuleName = fullName(mnTemp)
			if (find(skModulesInBLS,sModuleName,sModuleName))
			{
				//print "BLS: " sModuleName "\n"
				//for the progressbar
				progressStep iCountModules++
				continue
			}
			string sInfoString = "Baselineing " name(mnTemp) " (" iCountModules "/" iAmountOfModules ") "
			progressMessage(sInfoString "\nOpening Module")

			if(bStoreLinks!= false)
			{
				progressMessage(sInfoString "\nStoring Links")
				Module mTarget = mModuleOpenAsEdit(fullName(mnTemp), false, true)
				if(!null mTarget)
				{
					//disable all Trigger
					vTriggerBasicDisable()
					//print fullName(mnTemp) "\n"
					//Storing the Links in the attributes
					vLinksStoreLinksInAttribute(mTarget)
					//Close the modules wiht saveing
	                        	//bModuleClose(mTarget, true)
				}
			}
			if(bUpdateImplementationState != false)
            {
				progressMessage(sInfoString "\nUpdateing Implementations Status")
                Module mTarget = mModuleOpenAsEdit(fullName(mnTemp), false, true)
				if(!null mTarget)
				{
					//disable all Trigger
					vTriggerBasicDisable()
	                        	//Update the Modules without progressbar
					vImplStatUpdateModule(mTarget,false)

        	                	//Close the modules with saveing
                	        	//bModuleClose(mTarget, true)
				}
            }
            if(bUpdateCustomerAttributes != false)
            {
				progressMessage(sInfoString "\nUpdateing Customer Attributes")
                Module mTarget = mModuleOpenAsEdit(fullName(mnTemp), false, true)
				if(!null mTarget)
				{
	                //disable all Trigger
					vTriggerBasicDisable()
					//fill the customer attribute
					iFCAFillModule(mTarget,true)
					//bModuleClose(mTarget, true)

                }
            }
			
			if(bUpdateBaselineset != false)
            {
                progressMessage(sInfoString "\nAdding Module to Baseline")
				//Add the module to the skiplist
				put(skModuleList,skipIndex,mnTemp)
				//Add the Module to the baselineset
				str_baseline_error = addBaselines(skModuleList, bls)
				
				if(str_baseline_error != ""){
					print "Error: " str_baseline_error ", when create baseline for module: "fullName(mnTemp)"\n"
				}
				
				//delete the module from the baselineset again.
				delete(skModuleList,skipIndex)
			}
			
            if(bUpdateHistory != false && str_baseline_error == null)
			{
				progressMessage(sInfoString "\nUpdateing History")
				Module mTarget = mModuleOpenAsEdit(fullName(mnTemp), false, true)
				if(!null mTarget)
				{
					//disable all Trigger
					vTriggerBasicDisable()
					//The module is now editable-> add the history depending on the increase level
					string sHistory = sNewDescription
					
					if(!isVoid_(sNewSuffix)) sHistory = "(" sNewSuffix ")" sNewDescription
					
					//Major 
	                if(bMajorIncrease != false) { 
						vHistoryAddChangeDescription(mTarget,sHistory,0)  
					}
					//Minor
	                else{ 
						vHistoryAddChangeDescription(mTarget,sHistory,1) 
					}
					
        	        //Close the modules with saving
					save (mTarget)
				}
            }
            
			progressStep iCountModules++
			if(progressCancelled()){
				if( confirm(dbBLSCreateBaselineset, "Do you really want to abort the check?") ) bIsCanceled = true
			}
		}

		progressStop;
		vBLSCreateShowbaselineset(dbeBLSCreateShowBaselinsets,blsdefBLSSelected)
		delete skModuleList
        delete skModulesInBLS
	}
	if(!null sError) print sError

}


/* This function is called to create at Baselineset
parameter:
	DBE dbeTemp:	Dialogbox , which is firing the callback

global parameter -

return - */
void  vBLSdoCreateBaselineset(DBE dbeTemp)
{
	if(null blsdefBLSSelected)
	{
		//there is no Baselineset selected -> ask what to do
		string sMessage = "Please select a Baselineset Definition"
		string buttons[] = {"Cancel"}

		//print sMessage "\n"
		int iAnswer = messageBox(sMessage,buttons,msgQuery)
		return
	}
	dbBLSCreateBaselineset= create (dbBLSGui,"Create Baselinesets",styleCentered )
	close (dbBLSCreateBaselineset,false,vBLSDoClose)
	//now configure the DBES
       	string saValue[] = {""}
      	//ListView of the modules
      	dbeBLSCreateShowBaselinsets = listView(dbBLSCreateBaselineset,listViewOptionMultiselect ,400,15,saValue)
 //     	dbeBLSCreateNew = button(dbBLSGui,"New",vBLSdoCreateBaselineset,false)
 	//configure Checkbox
 	string sInc[] = {"Major","Minor"}
	dbeBLSCreateBaselinesetIncrease = radioBox(dbBLSCreateBaselineset, "Increase:",sInc,0)
 	beside(dbBLSCreateBaselineset)
        dbeBLSSuffix = field(dbBLSCreateBaselineset, "Suffix","",10)


        below(dbBLSCreateBaselineset)
        left(dbBLSCreateBaselineset)
        //dbeBLSDescription = field(dbBLSCreateBaselineset, "Description","",40)
	dbeBLSDescription = text(dbBLSCreateBaselineset, "Description","",40,false)

        //Add the frames to the dialogbox
	dbeBLSCreateFrame = frame(dbBLSCreateBaselineset,"Options", 400,50)
	//	dbeBLSCreateFrame -> "bottom" -> "form"
	dbeBLSCreateStoreLinks = toggle(dbBLSCreateBaselineset, "Store Links",true)
		dbeBLSCreateStoreLinks -> "top" -> "inside" -> dbeBLSCreateFrame
		dbeBLSCreateStoreLinks -> "left" -> "inside" -> dbeBLSCreateFrame
		dbeBLSCreateStoreLinks -> "right" -> "inside" -> dbeBLSCreateFrame
		dbeBLSCreateStoreLinks -> "bottom" -> "inside" -> dbeBLSCreateFrame
	dbeBLSCreateUpdateImplState = toggle(dbBLSCreateBaselineset, "Update ImplState",true)
		dbeBLSCreateUpdateImplState -> "top" -> "aligned" -> dbeBLSCreateStoreLinks
		dbeBLSCreateUpdateImplState -> "left" -> "spaced" -> dbeBLSCreateStoreLinks
	dbeBLSCreateUpdateCustomerAttribute = toggle(dbBLSCreateBaselineset, "Update Customer Attributes",true)
		dbeBLSCreateUpdateCustomerAttribute -> "top" -> "aligned" -> dbeBLSCreateUpdateImplState
		dbeBLSCreateUpdateCustomerAttribute -> "left" -> "spaced" -> dbeBLSCreateUpdateImplState
	dbeBLSCreateUpdateHistory = toggle(dbBLSCreateBaselineset, "Update History",true)
		dbeBLSCreateUpdateHistory -> "top" -> "aligned" -> dbeBLSCreateUpdateCustomerAttribute
		dbeBLSCreateUpdateHistory -> "left" -> "spaced" -> dbeBLSCreateUpdateCustomerAttribute
	dbeBLSCreateUpdateBLS = toggle(dbBLSCreateBaselineset, "Update Baselineset",true)
		dbeBLSCreateUpdateBLS -> "top" -> "aligned" -> dbeBLSCreateUpdateHistory
		dbeBLSCreateUpdateBLS -> "left" -> "spaced" -> dbeBLSCreateUpdateHistory


	dbeBLSCreateClose = button(dbBLSCreateBaselineset,"Close",vBLSCreateDoClose,false)
		dbeBLSCreateClose -> "top" -> "spaced" -> dbeBLSCreateFrame
		dbeBLSCreateClose -> "left" -> "unattached"
		dbeBLSCreateClose -> "right" -> "form"
		dbeBLSCreateClose -> "bottom" -> "form"

	dbeBLSCreateNew = button(dbBLSCreateBaselineset,"Create/Update",vBLSCreateDoCreate,true)
		dbeBLSCreateNew -> "top" -> "aligned" -> dbeBLSCreateClose
		dbeBLSCreateNew -> "left" -> "unattached"
		dbeBLSCreateNew -> "right" -> "spaced" -> dbeBLSCreateClose

      	//Create the GUI
      	realize dbBLSCreateBaselineset

      	insertColumn(dbeBLSCreateShowBaselinsets , 0, "Baseline Set", 299,iconNone)
      	insertColumn(dbeBLSCreateShowBaselinsets , 1, "Open", 100,iconNone)
	vBLSCreateShowbaselineset(dbeBLSCreateShowBaselinsets,blsdefBLSSelected)
	// raise the GUI
	block(dbBLSCreateBaselineset)
	destroy(dbBLSCreateBaselineset)

	//reset the selected Baselineset
	blsdefBLSSelected = null
	inactive(dbeBLSCloseBaselineset)
	//Get the content of the project field
	string sFolder = get(dbeBLSFolder)
	if(!isVoid_(sFolder))
	{
    		Folder fTemp = folder(sFolder)
    		//update the treeview
		vBLSEvaluateTreeviewString(dbeBLSTreeView, fTemp)
	}
}

/* This function is called to delete at Baselineset
parameter:
	DBE dbeTemp:	Dialogbox , which is firing the callback

global parameter -

return - */
void  vBLSdoDeleteBaselinesetDef(DBE dbeTemp)
{
	if(null blsdefBLSSelected)
	{
		//there is no Baselineset selected -> ask what to do
		string sMessage = "Please select a Baselineset Definition"
		string buttons[] = {"Cancel"}

		//print sMessage "\n"
		int iAnswer = messageBox(sMessage,buttons,msgQuery)
		return
	}
	if( confirm(dbBLSGui, "Do you really want delete Baselineset Definition" name(blsdefBLSSelected) "? \nAll baselinesets will be lost!!") )
	{
		string sMessage = delete(blsdefBLSSelected)
		//update the treeview
		//Get the content of the project field
		string sFolder = get(dbeBLSFolder)
		if(!isVoid_(sFolder))
		{
    			Folder fTemp = folder(sFolder)
			vBLSEvaluateTreeviewString(dbeBLSTreeView, fTemp)
		}
	}
}
/* This function is called to close an open Baselineset
parameter:
	DBE dbeTemp:	Dialogbox , which is firing the callback

global parameter -

return - */
void  vBLSdoCloseBaselineset(DBE dbeTemp)
{
	if(null blsdefBLSSelected)
	{
		//there is no Baselineset selected -> ask what to do
		string sMessage = "Please select a Baselineset Definition"
		string buttons[] = {"Cancel"}
		//print sMessage "\n"
		int iAnswer = messageBox(sMessage,buttons,msgQuery)
		return
	}
	//get the last Baselineset
	BaselineSet  bls
	for bls in blsdefBLSSelected do{}
	//is the basleline open?
	bool bIsOpen = isOpen(bls)
	if(bIsOpen)
	{
              	//lock the Definition
		lock(blsdefBLSSelected)
                //add the module to the definition
		string sError = close(bls)
		if(!null sError) print "BLS is not closed"
		//unlock the Definition again
		unlock(blsdefBLSSelected)
		//reset the selected Baselineset
		blsdefBLSSelected = null
		inactive(dbeBLSCloseBaselineset)
		//Get the content of the project field
		string sFolder = get(dbeBLSFolder)
		if(!isVoid_(sFolder))
		{
    			Folder fTemp = folder(sFolder)
			vBLSEvaluateTreeviewString(dbeBLSTreeView, fTemp)
		}
	}
}

/* This function is evaluationg the GUI for the baselinestselection
parameter:
	DB dbParent:	The parent Dialogbox

global parameter

return string: not evaluated yet*/
string sBLSRaiseGui(DB dbParent)
{
	//raise the GUI
	if (null dbParent) dbBLSGui = centered "Show  Baselinesets"
	else dbBLSGui = create (dbParent,"Show  Baselinesets",styleCentered )

//	DBE dbeBLSMenuBar = menuBar(dbBLSGui)

	//now add the differne DBE'S
	// show the selected folder
	Project pTemp = current
	string sFolderName = fullName(pTemp)
	Folder fTemp = folder(sFolderName)
	dbeBLSFolder = field(dbBLSGui, "",sFolderName,50)
	dbeBLSFolderSelect = button(dbBLSGui,"Browse",vBLSDoFolderBrowse,false)
		dbeBLSFolderSelect -> "left" -> "flush" -> dbeBLSFolder
		dbeBLSFolderSelect -> "right" -> "form"
		dbeBLSFolderSelect -> "top" -> "aligned" -> dbeBLSFolder
	dbeBLSTreeView = treeView(dbBLSGui,0,300,20)
	//Add the frames to the dialogbox
	dbeBLSDefinitionFrame = frame(dbBLSGui,"Baselineset Definition", 300,50)
			//dbeAttribCompMainFrame ->"top"->"form"
			dbeBLSDefinitionFrame ->"left"->"form"
			dbeBLSDefinitionFrame ->"right"->"form"
			dbeBLSDefinitionFrame ->"bottom"->"unattached"

	dbeBLSCreateBaselineset = button(dbBLSGui,"Create",vBLSCreateDefinition,false)
		dbeBLSCreateBaselineset -> "top" -> "inside" -> dbeBLSDefinitionFrame
		dbeBLSCreateBaselineset -> "left" -> "inside" -> dbeBLSDefinitionFrame
		dbeBLSCreateBaselineset -> "right" -> "inside" -> dbeBLSDefinitionFrame
		dbeBLSCreateBaselineset -> "bottom" -> "inside" -> dbeBLSDefinitionFrame
	dbeBLSDeleteBaselinesetDef = button(dbBLSGui,"Delete",vBLSdoDeleteBaselinesetDef,false)
		dbeBLSDeleteBaselinesetDef -> "top" -> "aligned" -> dbeBLSCreateBaselineset
		dbeBLSDeleteBaselinesetDef -> "left" -> "flush" -> dbeBLSCreateBaselineset


	dbeBLSBaselinesetFrame = frame(dbBLSGui,"Baselineset", 300,50)
			//dbeAttribCompMainFrame ->"top"->"form"
		dbeBLSDefinitionFrame ->"left"->"form"
		dbeBLSDefinitionFrame ->"right"->"form"
		dbeBLSDefinitionFrame ->"bottom"->"unattached"

	dbeBLSCreateBaselineset = button(dbBLSGui,"Create",vBLSdoCreateBaselineset,false)
		dbeBLSCreateBaselineset -> "top" -> "inside" -> dbeBLSBaselinesetFrame
		dbeBLSCreateBaselineset -> "left" -> "inside" -> dbeBLSBaselinesetFrame
		dbeBLSCreateBaselineset -> "right" -> "inside" -> dbeBLSBaselinesetFrame
		dbeBLSCreateBaselineset -> "bottom" -> "inside" -> dbeBLSBaselinesetFrame

	dbeBLSCloseBaselineset = button(dbBLSGui,"Close",vBLSdoCloseBaselineset,false)
		dbeBLSCloseBaselineset -> "left" -> "flush" -> dbeBLSCreateBaselineset
		dbeBLSCloseBaselineset -> "top" -> "aligned" -> dbeBLSCreateBaselineset



	realize dbBLSGui
//	addMenu(dbeBLSMenuBar, "Format", 'F', entries, mn, hot,help, inactive)
	inactive(dbeBLSCloseBaselineset)

	set(dbeBLSTreeView, bBLSdoExpandDirectory)
	set(dbeBLSTreeView, vBLSonSelection, vBLSonActivate)
	//update the treeview
	vBLSEvaluateTreeviewString(dbeBLSTreeView, fTemp)
	// raise the GUI
	block(dbBLSGui)
	destroy(dbBLSGui)
//	show(dbBLSGui)

	return ""
}

//sBLSRaiseGui(null)