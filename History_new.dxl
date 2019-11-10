/*

 \file    History.dxl

 \brief   This include contains the functions forwriting the hitory of an project

 \author  muders01

 \date    02.06.2014. Update: 05.08.2019 by nguye003

 \note    (C) Copyright Leopold Kostal GmbH&Co KG \n\n
          Contents and presentations are protected world-wide.\n
          Any kind of using, copying etc. is prohibited without prior permission. \n
          All rights - incl. industrial property rights - are reserved.

*/

//is defining the Versionlevels e.g. 1.0; 1.2.3
const string sHistChangeLogKey 			= "Change log"
const string sHistChangeLogVersion 		= "Version"
const string sHistFirstVersion 			= "0.0.0"
const string sHistVersionSeparator 		= "."
const string str_Annotation 			= "Annotation"
const int iHistAmountOfVersionLevels 	= 3

const string sHistSaveSeparator = ";"
const string sHistSaveEntrySeparator = "\t"


string cutBeforeSearchString (string processVal, string searchString) {
	bool stringFound = false
	string newVal = "", cutVal = ""
	int offset = 0
	int Length = 0
	int valLength = length(processVal)
	int searchStringLength = length(searchString)
	if (findPlainText(processVal,searchString,offset,Length,true,true)){
		newVal = processVal[0:offset-1]
		stringFound = true
	}
	if (!stringFound) {
		newVal = processVal
	}
	return newVal
}

/*
This function is incrementing the Version on the selected position
parameter:
	int iIncrementPosition:THe position to increment
return:
	string:  the new  version*/
string sHistoryEvaluateVersion(string sOldVersion, int iIncrementPosition)
{
print name (current Module)
	//basic check
	if(isVoid_(sOldVersion)|| (iIncrementPosition <0) || (iIncrementPosition>= iHistAmountOfVersionLevels)) return sOldVersion

	sOldVersion = trimWhitespace(sOldVersion)
	//define the arrays
	string sArrayTemp[iHistAmountOfVersionLevels]
	//initialization of the array
	int i
	for(i = 0; i< (iHistAmountOfVersionLevels);i++) sArrayTemp[i] = "0"
	str_split_String(sHistVersionSeparator,sOldVersion , sArrayTemp)
	//Reset the arrayelements after the incremented Versionlevel.
	for(i = (iIncrementPosition+1); i< (iHistAmountOfVersionLevels);i++) sArrayTemp[i] = "0"

	//The Version is now separated in an arrays -> get the Level to increase
	string sIncrementationlevel = sArrayTemp[iIncrementPosition]
	int iIncrementationLevel = 0
	if (isValidInt sIncrementationlevel )
	{
		iIncrementationLevel = intOf(sIncrementationlevel)
		iIncrementationLevel++
		sArrayTemp[iIncrementPosition] = iIncrementationLevel ""
	}
	string sVersion = sArrayTemp [0]
	for(i = 1; i< (iHistAmountOfVersionLevels);i++)
	{
		sVersion = sVersion "." sArrayTemp[i]
	}
	return sVersion
}

string sHistoryEvaluateVersion5columns(string sOldVersion, int iIncrementPosition)
{
	//basic check
	if(isVoid_(sOldVersion)|| (iIncrementPosition <0) || (iIncrementPosition>= iHistAmountOfVersionLevels)) return sOldVersion

	sOldVersion = trimWhitespace(sOldVersion)
	//define the arrays
	string sArrayTemp[iHistAmountOfVersionLevels]
	//initialization of the array
	int i
	for(i = 0; i< (iHistAmountOfVersionLevels);i++) sArrayTemp[i] = "0"
	str_split_String(sHistVersionSeparator,sOldVersion , sArrayTemp)
	//Reset the arrayelements after the incremented Versionlevel.
	for(i = (iIncrementPosition+1); i< (iHistAmountOfVersionLevels);i++) sArrayTemp[i] = "0"

	//The Version is now separated in an arrays -> get the Level to increase
	string sIncrementationlevel = sArrayTemp[iIncrementPosition]
	int iIncrementationLevel = 0
	if (isValidInt sIncrementationlevel )
	{
		iIncrementationLevel = intOf(sIncrementationlevel)
		iIncrementationLevel++
		sArrayTemp[iIncrementPosition] = iIncrementationLevel ""
	}
	string sVersion = sArrayTemp [0]
	for(i = 1; i< (iHistAmountOfVersionLevels);i++)
	{
		sVersion = sVersion "." sArrayTemp[i]
	}
	sVersion = cutBeforeSearchString(sVersion, ".")
	return sVersion
}


/*
This function is adding the change Description to the table of the module. If the Table is not available
a moduleparamater is added and the change description is done in a string
parameter:
	Module mod: The module, where the changelog should be changed
	string sChangeDescription: The change description
	bool bUseTable: For those module, which don't have a table something else must be done
return - */
void vHistoryAddChangeDescription(Module mod, string sChangeDescription, int iIncrementPosition)
{
	//basic checks
	if((null mod) || (isVoid_(sChangeDescription)) || (iIncrementPosition <0) || (iIncrementPosition>= iHistAmountOfVersionLevels)) return
    //print "Mod: " name(mod) "\n"

	bool bChangeLogFound = false
	Object obj
	Baseline b
	string str_baseline_num = ""
	int major_num = 0
	int minor_num = 0

	for obj in top (mod) do{
		// Deleted objects are not evaluated
		if(isDeleted(obj)) continue
		//Table objects are not evaluated
		if(table(obj)) continue
		//row objects are not evaluated
		if(row(obj)) continue
		string sTemp = obj.sObjectHeading
		//The changelogKey was found
		if(sTemp == sHistChangeLogKey)
		{
			bChangeLogFound = true
			//print sTemp "\n"
			Object oTemp
			int count = 0
			//now check the table
			for oTemp in all obj do
			{
				if(isDeleted(oTemp)) continue
				Object oRow
				Object oCell
				//store the old Version
				string sOldVersion
				Object oLastValidCell
				//print name(mod) "\t" identifier(oTemp) "\n"
				for oRow in table(oTemp) do {
					count = 0
					for oCell in row(oRow) do {
        				//print oCell.sObjectText "\t"
						if(count == 0)
						{
							//only if the Cell is not deleted, the version will be used.
							if(!isDeleted(oCell)) sOldVersion = oCell.sObjectText
						}
						count++
					}
					if(!isDeleted(oCell))oLastValidCell = oCell
				}

				// we are now on the last cell-> add another one.
				oRow = appendRow(oLastValidCell)
				count = 0;
				//now fill the cells with the correct values
				filtering off
				string str_modPath = fullName(mod)
				
				//If module in project P06831
				if (matches ("P06831", str_modPath)) {
					for oCell in row(oRow) do {
						//	print oCell.sObjectText "\t"
						if(count == 0)
						{
							sHistFirstVersion = "0.0"
							if(sOldVersion != sHistChangeLogVersion)
							{
								b = getMostRecentBaseline(mod)
								//Get major number of current baseline
								major_num = (major b)
								//Get minor number of current baseline
								minor_num = (minor b)
								
								//print major_num"" "." minor_num""
								
								//if choose major increment 
								if(iIncrementPosition == 0){ 
									major_num = major_num + 1 
									minor_num = 0
								}
								//if choose minor increment
								else if(iIncrementPosition == 1){ 
									minor_num = minor_num + 1
								}
								
								str_baseline_num 	= major_num"" "." minor_num""
								oCell.sObjectText 	= str_baseline_num
								
								b = null
								/* old version 
								oCell.sObjectText = sHistoryEvaluateVersion5columns(sOldVersion,iIncrementPosition)
								//oCell.sObjectText = sHistoryEvaluateVersion(sOldVersion,iIncrementPosition)
								*/
							}
							else oCell.sObjectText = sHistFirstVersion
						}
						if(count == 1) oCell.sObjectText = "1"
						if(count == 2) oCell.sObjectText = sChangeDescription
						if(count == 3) oCell.sObjectText = username() ""
						if(count == 4) oCell.sObjectText = stringOf(today(),loc,"yyyy-MM-dd")
						//mark the Cell as irrelevant
						if (bAttributesIsAttributeAvailable(mod, sAttribLKContentType)) {
							//old version: oCell.sAttribLKContentType = sIsIrrelevant
							oCell.sAttribLKContentType = str_Annotation
						}
						else {
							//print name(mod) ": no attribute " sAttribLKContentType "\n"
						}
						count++
    				}
				}else { //If module not in project P06831
					for oCell in row(oRow) do {
						//	print oCell.sObjectText "\t"
						if(count == 0)
						{
							if(sOldVersion != sHistChangeLogVersion)
							{
								oCell.sObjectText = sHistoryEvaluateVersion(sOldVersion,iIncrementPosition)
							}
							else oCell.sObjectText = sHistFirstVersion
						}
						if(count == 1) oCell.sObjectText = sChangeDescription
						if(count == 2) oCell.sObjectText = username() ""
						if(count == 3) oCell.sObjectText = stringOf(today(),loc,"yyyy-MM-dd")
						//mark the Cell as irrelevant
						if (bAttributesIsAttributeAvailable(mod, sAttribLKContentType)) {
							//old version oCell.sAttribLKContentType = sIsIrrelevant
							oCell.sAttribLKContentType = str_Annotation
						}
						else {
							//print name(mod) ": no attribute " sAttribLKContentType "\n"
						}
						count++
					}
				}
			}
		}
	}

/*	If Changelog not found in current module  => do nothing

	if(bChangeLogFound != true)
	{
		//The Changelog is not available -> add the history to a  modulepropertie
		//Check the existence of the attribute
		bool bAvailable = bAttributesIsAttributeAvailable(mod,sAttribModuleHistory)
		
		//check if the Attribute is available
		if(bAttributesIsAttributeAvailable(mod,sAttribModuleHistory) != true)
		{
			//create one
			vAttributesCreateModuleHistory(mod)
		}
		//Replace the ; by ,, because this is used as separator
		sChangeDescription = sStrReplace(sChangeDescription,";",",")
		//Replace the \n by ," "
		//sChangeDescription = sStrReplace(sChangeDescription,"\n"," ")

		string sTemp = mod.sAttribModuleHistory
		if(isVoid_(sTemp))
		{
			// the Change description is empty
			sTemp = sHistFirstVersion sHistSaveSeparator
			sTemp = sTemp sChangeDescription sHistSaveSeparator
			sTemp = sTemp username() sHistSaveSeparator
			sTemp = sTemp "[" stringOf(today(),loc,"yyyy-MM-dd") "]"

			mod.sAttribModuleHistory = sTemp
		}
		else
		{
			//now find the correct version
			int iCount = iSplitCount(sHistSaveSeparator,sTemp)
			string sArrayHistory[iCount]
		  	str_split_String(sHistSaveSeparator,sTemp ,sArrayHistory)
			//for(i= 0; i < iCount; i++) print "Nr: " i " " sArrayHistory[i] "\n"
			string sOldVersion = trimWhitespace(sArrayHistory[iCount-4])

			sTemp = sTemp sHistSaveSeparator "\n" sHistoryEvaluateVersion(sOldVersion,iIncrementPosition) sHistSaveSeparator
			sTemp = sTemp sChangeDescription sHistSaveSeparator
			sTemp = sTemp username() sHistSaveSeparator
			sTemp = sTemp "[" stringOf(today(),loc,"yyyy-MM-dd") "]"

            //store the string
			mod.sAttribModuleHistory = sTemp
		}
	}
*/	
}

