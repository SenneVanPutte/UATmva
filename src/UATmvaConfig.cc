
#include "../includes/UATmvaConfig.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
//#include <boost/foreach>
//#include <boost/tokenizer>
//#include <boost/algorithm/string.hpp>

using namespace std;

// ----------------------


// ----------------------


void UATmvaConfig::Reset(){

  InitDone = false ;

  InputType = -1;
  TreeName  = "";
  InputData.clear();
  InputVar.clear();

  TmvaType           = "ANN" ;
  TmvaWeight         = "" ;
  TmvaVar.clear();

  ANNCycles          = 500 ;
  ANNVarNumRemove    = 0   ;
  ANNHiddenLayersMin = 1   ;
  ANNHiddenLayersMax = 2   ;
  ANNHiddenNodesMin  = 0   ;
  ANNHiddenNodesMax  = 3   ;
  ANNFracTrain       = .5  ; 
  ANNFracTest        = .5  ;
  ANNFracValidate    = 1.  ;

}

// ----------------------

void UAError(string mess){
  cout << mess;
  exit(1);
}

vector<string> UATokenize(string input , char token){
  stringstream ssinput(input);
  string Element;
  vector<string> Elements;
  while (getline(ssinput, Element , token )) {
   Elements.push_back(Element);
  }
  return Elements;
}

// ----------------------

void UATmvaConfig::ReadCfg(TString CfgName) {

  ifstream Cfg ;
  Cfg.open ( CfgName );

  if(!Cfg) {
    cout << "Cannot open input file.\n";
    return ;
  }

  char str[255];
  while( Cfg ) {

    Cfg.getline(str, 255);  
    if(!Cfg) continue;  
    istringstream iss(str);
    if (iss.str().find("#") != string::npos ) continue; 
    vector<string> Elements;
    do
    {
        string sub;
        iss >> sub;
        if(sub.size()>0) Elements.push_back(sub);
    } while (iss);
    if ( ! (Elements.size() > 0) ) continue;

    // TmvaName
    if ( Elements.at(0) == "TmvaName" ) {
      if   ( Elements.size() == 2 )  TmvaName = Elements.at(1) ;
      else UAError("[UATmvaConfig] Wrong TmvaName Input !");
    }

    // InputType
    if ( Elements.at(0) == "InputType" ) {
      if   ( Elements.size() == 2 )  InputType = atoi(Elements.at(1).c_str()) ;
      else UAError("[UATmvaConfig] Wrong InputType Input !");
    }

    // TreeName
    if ( Elements.at(0) == "TreeName" ) {
      if   ( Elements.size() == 2 ) TreeName = Elements.at(1) ;
      else UAError("[UATmvaConfig] Wrong InputType Input !");
    }


    // InputData
    if ( Elements.at(0) == "InputData" ) {
       if  ( Elements.size() == 7 ) {
          InputData_t InDat;
          InDat.NickName  = Elements.at(1);
          InDat.FileName  = Elements.at(6);
          InDat.SigTrain  = atoi(Elements.at(2).c_str()) ;
          InDat.BkgdTrain = atoi(Elements.at(3).c_str()) ;
          InDat.TrueData  = atoi(Elements.at(4).c_str()) ;
          InDat.BkgdData  = atoi(Elements.at(5).c_str()) ;
          InputData.push_back(InDat);
       }
       else UAError("[UATmvaConfig] Wrong InputData Input !");
    }

    // InputVar
    if ( Elements.at(0) == "InputVar" ) {
      if   ( Elements.size() == 2 ) SetInputVar(Elements.at(1),InputVar) ;
      else UAError("[UATmvaConfig] Wrong InputType Input !");
    }

    if ( Elements.at(0) == "TmvaType" ) {
      if   ( Elements.size() == 2 ) TmvaType = Elements.at(1) ;
      else UAError("[UATmvaConfig] Wrong TmvaType Input !");
    }

    if ( Elements.at(0) == "TmvaWeight" ) {
      if   ( Elements.size() == 2 ) TmvaWeight = Elements.at(1) ;
      else UAError("[UATmvaConfig] Wrong TmvaWeight Input !");
    }

    if ( Elements.at(0) == "TmvaVar" ) {
      if   ( Elements.size() == 2 ) SetInputVar(Elements.at(1),TmvaVar) ;
      else UAError("[UATmvaConfig] Wrong TmvaVar Input !");
    }

 

 

  }
  InitDone = true;
 
  Cfg.close();
}

// ----------------------

void UATmvaConfig::SetInputVar(string sVarList, vector<InputVar_t>& vVarList){
   
   vector<string> Var = UATokenize( sVarList, ':' );
  
   for (vector<string>::iterator iVar = Var.begin() ; iVar != Var.end() ; ++iVar){
     vector<string> VarSplit = UATokenize( *iVar , '/' );
     if (VarSplit.size() == 1) {VarSplit.push_back("F");}
     InputVar_t InVar;
     InVar.VarName = VarSplit.at(0);
     InVar.VarType = VarSplit.at(1)[0];
     InVar.Active  = true; 
     vVarList.push_back(InVar);
   } 
   
}

// ----------------------

void UATmvaConfig::Print(){

  cout << "----------------------------------------------------------" << endl;
  cout << "----------------[UATmvaConfig::Print()] ------------------" << endl;
  cout << "----------------------------------------------------------" << endl;
  cout << "TmvaName   = " << TmvaName  << endl;
  cout << "----------------------------------------------------------" << endl;
  cout << "InputType  = " << InputType << endl;
  cout << "TreeName   = " << TreeName  << endl;
  cout << "----------------------------------------------------------" << endl;
  for ( vector<InputData_t>::iterator iD = InputData.begin() ; iD != InputData.end() ; ++iD ) {
    cout << "InputData  = " << iD->NickName << endl;
    cout << " --> FileName: " << iD->FileName << endl;
    cout << " --> SigTrain: "<<iD->SigTrain<<" BkgdTrain: "<< iD->SigTrain ;
    cout <<     " TrueData: "<<iD->TrueData<<" BkgdData : "<< iD->BkgdData << endl;
  }
  cout << "----------------------------------------------------------" << endl;
//  cout << "InputVar    = " << InputVarRaw << endl;
  for (vector<InputVar_t>::iterator iVar = InputVar.begin() ; iVar != InputVar.end() ; ++iVar ) {
    cout << "InputVar : " << iVar->VarName << " " << iVar->VarType << " " << iVar->Active << endl;
  }
  cout << "----------------------------------------------------------" << endl;
  cout << "TmvaType    = " << TmvaType << endl;
  cout << "TmvaWeight  = " << TmvaWeight << endl;
  for (vector<InputVar_t> ::iterator iVar = TmvaVar.begin() ; iVar != TmvaVar.end() ; ++iVar ) {
     cout << "TmvaVar : " << iVar->VarName << " " << iVar->VarType << " " << iVar->Active << endl;
  }

  if ( TmvaType == "ANN" ) {
    cout << "--------------------- TMVA: ANN ---------------------------" << endl;
    cout << "ANNCycles       = " << ANNCycles << endl;
    cout << "ANNVarNumRemove = " << ANNVarNumRemove << endl;
    cout << "ANNHiddenLayers = " << ANNHiddenLayersMin << " " << ANNHiddenLayersMax << endl;
    cout << "ANNHiddenNodes  = " << ANNHiddenNodesMin  << " " << ANNHiddenNodesMax  << endl;
    cout << "ANNFracTrain    = " << ANNFracTrain << endl;
    cout << "ANNFracTest     = " << ANNFracTest << endl;
    cout << "ANNFracValidate = " << ANNFracValidate << endl;
  }

}
