#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <TSystem.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TTreeFormula.h>

#include "TMVA/Reader.h"

class InputData_t {
  public:
  InputData_t(){;}
  virtual ~InputData_t(){;}
  TString inDir ;
  TString FileName ;
  TString TreeName  ;
};  

class TreeFormula_t {
  public:
  TreeFormula_t(){ bEvaluated = false ;}
  virtual ~TreeFormula_t(){;}
  string        NickName   ;
  string        Expression ;
  private:
  TTreeFormula* Formula    ;        
  bool          bEvaluated ;
  Float_t       Result_    ; 
  public:
  void MakFormula (TTree *);
  void EvaFormula ()       ;
  void DelFormula ()       ;
  Float_t Result()         ;
};

class Mva_t {
  public:
  Mva_t(){;}
  virtual ~Mva_t(){;}
  TString MVAname ;
  TString MVAprefix ;
  TString MVApostfix ;  
  vector<TreeFormula_t> MVAvars;
  float VarVal[40] ;
  vector<TMVA::Reader*> Readers;
  float   MVAout[100];
};

// ---------------------- TreeFormula_t ----------------------------

void TreeFormula_t::MakFormula (TTree* Tree){
  Formula = new TTreeFormula(NickName.c_str(),Expression.c_str(),Tree);
}
void TreeFormula_t::EvaFormula (){
  if ( Formula ) {
    Result_    = Formula->EvalInstance() ;
    bEvaluated = true ;
  } else {
    cout << "[TreeFormula_t::EvaFormula] Formula not associated to a Tree: " << NickName << endl ;
    Result_ = 0. ; 
  }
}
void TreeFormula_t::DelFormula (){
  bEvaluated = false ;
  delete Formula;
}
Float_t TreeFormula_t::Result(){
  if ( bEvaluated ) { 
    return Result_ ;
  } else {
    cout << "[TreeFormula_t::EvaFormula] Formula not evaluated: " << NickName << endl ;
    return 0.;
  }
} 

// ---------------------- TOOLS ------------------------------------

vector<string> UATokenize(string input , char token){
  stringstream ssinput(input);
  string Element;
  vector<string> Elements;
  while (getline(ssinput, Element , token )) {
   Elements.push_back(Element);
  }
  return Elements;
}

// ------------------- mkTmvaFriendTrees ----------------------------

void mkTmvaFriendTrees( TString CfgName = "Config.cfg" ) {

  // Read Config

  vector<InputData_t> InputData ;
  vector<TString>     MassPoints  ;
  TString             outDir ;
  TString             jetBin ;
  vector<TFile*>      FileOut;
  vector<TTree*>      TreeOut; 
  vector<Mva_t>    Mva ;

  ifstream Cfg ;
  Cfg.open ( CfgName );

  if(!Cfg) {
    cout << "Cannot open input file.\n";
    return ;
  }

  char str[1000];
  while( Cfg ) {

    Cfg.getline(str,1000);
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

    // InputData
    if ( Elements.at(0) == "InputData" ) {
      InputData_t InDat;
      InDat.inDir     = Elements.at(1);  
      InDat.FileName  = Elements.at(2);
      InDat.TreeName  = Elements.at(3);
      InputData.push_back(InDat);  
    }

    // Mass Points
    if ( Elements.at(0) == "MassPoints" ) {
      vector<string> MP = UATokenize( Elements.at(1) , ':' );    
      for (vector<string>::iterator iMP = MP.begin() ; iMP != MP.end() ; ++iMP ) MassPoints.push_back( iMP->c_str() ) ;
    }
    
    // MVA2Add
    if ( Elements.at(0) == "addMVA" ) {
      Mva_t AM ;
      AM.MVAname     = Elements.at(1);
      AM.MVAprefix   = Elements.at(2);
      AM.MVApostfix  = Elements.at(3);
      vector<string> VAR = UATokenize( Elements.at(4) , ':' ); 
      for (vector<string>::iterator iVAR = VAR.begin() ; iVAR != VAR.end() ; ++iVAR ) {
         TreeFormula_t  varTTF ;
         varTTF.NickName    = iVAR->c_str() ;
         varTTF.Expression  = iVAR->c_str() ;
         (AM.MVAvars).push_back( varTTF )   ; 
      }
      Mva.push_back(AM); 
    }
    if ( Elements.at(0) == "outDir" ) { 
      outDir = Elements.at(1);
    }
    if ( Elements.at(0) == "jetBin" ) { 
      jetBin = Elements.at(1);
    }

  }


  // Internal variables

  TFile* FileIn ;
  TTree* TreeIn ;

  for ( vector<InputData_t>::iterator itD = InputData.begin() ; itD != InputData.end() ; ++itD ) {
 
 
    FileIn = new TFile(itD->inDir+itD->FileName,"READ");
    TreeIn = (TTree*) FileIn->Get(itD->TreeName);
    TreeIn->SetBranchStatus("*",1);
 
    // Init


    for ( vector<Mva_t>::iterator itMva = Mva.begin() ; itMva != Mva.end() ; ++itMva ) {
      for ( vector<TreeFormula_t>::iterator itVar = (itMva->MVAvars).begin() ; itVar != (itMva->MVAvars).end() ; ++itVar ) itVar->MakFormula(TreeIn);
      int iMP = 0 ; 
      for ( vector<TString>::iterator itMP = MassPoints.begin() ; itMP !=  MassPoints.end() ; ++itMP , ++iMP ) {
        (itMva->Readers).push_back(new TMVA::Reader( "!Color:!Silent" ));
        for ( int iVar = 0 ; iVar < (signed) (itMva->MVAvars).size() ; ++iVar ) {
           ((itMva->Readers).at(iMP))->AddVariable( ((itMva->MVAvars).at(iVar)).NickName , &(itMva->VarVal)[iVar] );
        } 
        ((itMva->Readers).at(iMP))->BookMVA(  (itMva->MVAname)+*itMP , (itMva->MVAprefix)+*itMP+(itMva->MVApostfix) ) ;
      }
    }

    cout << "Doing: " << itD->inDir << "/" << itD->FileName << endl;

    int iMP = 0 ; 
    for ( vector<TString>::iterator itMP = MassPoints.begin() ; itMP !=  MassPoints.end() ; ++itMP , ++iMP ) {
      TString Dir = outDir+"/ntupleMVA_MH"+*itMP+"_"+jetBin+"jet/";
      if (!gSystem->OpenDirectory(Dir)) gSystem->MakeDirectory(Dir);

      FileOut.push_back(new TFile(Dir+itD->FileName,"RECREATE"));  
      (FileOut.at(iMP))->cd();
      TreeOut.push_back(new TTree("latinobdt", "latino MVA tree"));
      (TreeOut.at(iMP))->SetAutoSave(10000000);  // autosave when 10 Mbyte written
      for ( vector<Mva_t>::iterator itMva = Mva.begin() ; itMva != Mva.end() ; ++itMva ) {
        (TreeOut.at(iMP))->Branch( itMva->MVAname , &(itMva->MVAout[iMP]) , itMva->MVAname+"/F");
      }
    } 

    // Loop 

    
    Int_t nEntries = TreeIn->GetEntries();
    for (Int_t jEntry = 0 ;  jEntry < nEntries ; ++jEntry) { 
      TreeIn->GetEntry(jEntry);
      for ( vector<Mva_t>::iterator itMva = Mva.begin() ; itMva != Mva.end() ; ++itMva ) {
        int iVar = 0 ;
        for ( vector<TreeFormula_t>::iterator itVar = (itMva->MVAvars).begin() ; itVar != (itMva->MVAvars).end() ; ++itVar , ++iVar ) {
          itVar->EvaFormula();
          (itMva->VarVal)[iVar] = itVar->Result(); 
        }
        int iMP = 0 ; 
        for ( vector<TString>::iterator itMP = MassPoints.begin() ; itMP !=  MassPoints.end() ; ++itMP , ++iMP ) {
          itMva->MVAout[iMP] = ((itMva->Readers).at(iMP))->EvaluateMVA( (itMva->MVAname)+*itMP );
        }   
      }
      int iMP = 0 ; 
      for ( vector<TString>::iterator itMP = MassPoints.begin() ; itMP !=  MassPoints.end() ; ++itMP , ++iMP ) (TreeOut.at(iMP))->Fill();
    }

    // End -> Cleaning

    for ( vector<Mva_t>::iterator itMva = Mva.begin() ; itMva != Mva.end() ; ++itMva ) {
      for ( vector<TreeFormula_t>::iterator itVar = (itMva->MVAvars).begin() ; itVar != (itMva->MVAvars).end() ; ++itVar ) itVar->DelFormula() ;
      int iMP = 0 ; 
      for ( vector<TString>::iterator itMP = MassPoints.begin() ; itMP !=  MassPoints.end() ; ++itMP , ++iMP ) {
        delete ((itMva->Readers).at(iMP));
      }  
      (itMva->Readers).clear();
    }

    iMP = 0 ; 
    for ( vector<TString>::iterator itMP = MassPoints.begin() ; itMP !=  MassPoints.end() ; ++itMP , ++iMP ) {
      (FileOut.at(iMP))->Write();
      delete (TreeOut.at(iMP));
      (FileOut.at(iMP))->Close(); 
    }
    TreeOut.clear();
    FileOut.clear();

 
    delete TreeIn;
    FileIn->Close();
    delete FileIn ; 

  }

}
