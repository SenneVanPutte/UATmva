
#include "src/UATmvaConfig.cc"
#include "src/UATmvaTree.cc"
#include "src/UATmvaClassification.cc"
#include "src/figureOfMerit.cc"
#include "src/UATmvaTools.cc"
#include "src/UATmvaReader.cc"


void UATmva(){
  
  UATmvaConfig UATmvaConfigTest ;
  UATmvaConfigTest.ReadCfg("Config.cfg");
  UATmvaConfigTest.Print();

  UATmvaTree UATmvaTreeTest(UATmvaConfigTest);

  UATmvaClassification UATmva;
  UATmva.Do(UATmvaConfigTest,UATmvaTreeTest);

  UATmvaReader Reader;
  Reader.Do(UATmvaConfigTest,UATmvaTreeTest); 
  
}
