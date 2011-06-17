#include "src/UATmvaConfig.cc"
#include "src/UATmvaTree.cc"
#include "src/UATmvaClassification.cc"
#include "src/figureOfMerit.cc"
#include "src/UATmvaTools.cc"
#include "src/UATmvaReader.cc"
#include "src/UATmvaSummary.cc"


void UATmva(){

  UATmvaConfig UATmvaConfigTest ;
  UATmvaConfigTest.ReadCfg("Config.cfg");
  UATmvaConfigTest.Print();

//  UATmvaTree UATmvaTreeTest(UATmvaConfigTest);

//  UATmvaClassification UATmva;
//  UATmva.Do(UATmvaConfigTest,UATmvaTreeTest);

  //UATmvaReader Reader;
  //Reader.SetNbin(10); 
  //Reader.Do(UATmvaConfigTest,UATmvaTreeTest); 
  
    UATmvaSummary Summary;
    Summary.Init(UATmvaConfigTest);
    Summary.Plots();

}
