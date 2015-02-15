#include "sigSelections.h"

namespace mt2 {

std::vector<SR> getSignalRegions2012(){

  std::vector<SR> temp_SR_vec;
  std::vector<SR> SRVec;
  SR sr;
  SR baseSR;

  //first set binning in njet-nbjet plane
  sr.SetName("1");
  sr.SetVar("njets", 2, 3);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("mt2", 200, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("2");
  sr.SetVar("njets", 2, 3);
  sr.SetVar("nbjets", 1, 3);
  sr.SetVar("mt2", 200, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("3");
  sr.SetVar("njets", 3, 6);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("mt2", 200, 400);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("4");
  sr.SetVar("njets", 3, 6);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("mt2", 200, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("5");
  sr.SetVar("njets", 3, 6);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("mt2", 200, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("6");
  sr.SetVar("njets", 6, -1);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("mt2", 200, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("7");
  sr.SetVar("njets", 6, -1);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("mt2", 200, 400);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("8");
  sr.SetVar("njets", 6, -1);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("mt2", 200, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("9");
  sr.SetVar("njets", 3, -1);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("mt2", 200, 400);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  //add HT and MET requirements
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "L");
    fullSR.SetVar("ht", 450, 750);
    fullSR.SetVar("met", 200, -1);
    SRVec.push_back(fullSR);
  }
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "M");
    fullSR.SetVar("ht", 750, 1200);
    fullSR.SetVar("met", 30, -1);
    SRVec.push_back(fullSR);
  }
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "H");
    fullSR.SetVar("ht", 1200, -1);
    fullSR.SetVar("met", 30, -1);
    SRVec.push_back(fullSR);
  }

  //define baseline selections commmon to all signal regions 
  //baseSR.SetVar("mt2", 200, -1);
  baseSR.SetVar("j1pt", 100, -1);
  baseSR.SetVar("j2pt", 100, -1);
  baseSR.SetVar("deltaPhiMin", 0.3, -1);
  baseSR.SetVar("diffMetMht", 0, 70);
  baseSR.SetVar("nlep", 0, 1);
  //baseSR.SetVar("passesHtMet", 1, 1);

  //add baseline selections to all signal regions 
  std::vector<std::string> vars = baseSR.GetListOfVariables();
  for(unsigned int i = 0; i < SRVec.size(); i++){
    for(unsigned int j = 0; j < vars.size(); j++){
      SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
    }
  }

  return SRVec;

}

std::vector<SR> getSignalRegions2015LowLumi(){//used for AN-15-009

  std::vector<SR> temp_SR_vec;
  std::vector<SR> SRVec;
  SR sr;
  SR baseSR;

  //first set binning in njet-nbjet plane
  sr.SetName("1");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("2");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("3");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("4");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("5");
  sr.SetVar("njets", 4, -1);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("6");
  sr.SetVar("njets", 4, -1);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("7");
  sr.SetVar("njets", 4, -1);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("8");
  sr.SetVar("njets", 4, -1);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("9");
  sr.SetVar("njets", 2, -1);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("10");
  sr.SetVar("njets", 2, -1);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  //add HT and MET requirements
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "L");
    fullSR.SetVar("ht", 450, 575);
    fullSR.SetVar("met", 200, -1);
    SRVec.push_back(fullSR);
  }
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "M");
    fullSR.SetVar("ht", 575, 1000);
    fullSR.SetVar("met", 200, -1);
    SRVec.push_back(fullSR);
  }
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "H");
    fullSR.SetVar("ht", 1000, -1);
    fullSR.SetVar("met", 30, -1);
    SRVec.push_back(fullSR);
  }

  //define baseline selections commmon to all signal regions 
  baseSR.SetVar("mt2", 200, -1);
  baseSR.SetVar("j1pt", 100, -1);
  baseSR.SetVar("j2pt", 100, -1);
  baseSR.SetVar("deltaPhiMin", 0.3, -1);
  baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
  baseSR.SetVar("nlep", 0, 1);
  //baseSR.SetVar("passesHtMet", 1, 1);

  //add baseline selections to all signal regions 
  std::vector<std::string> vars = baseSR.GetListOfVariables();
  for(unsigned int i = 0; i < SRVec.size(); i++){
    for(unsigned int j = 0; j < vars.size(); j++){
      SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
    }
  }

  return SRVec;

}


std::vector<SR> getSignalRegions2015ExtendedNJets(){

  std::vector<SR> temp_SR_vec;
  std::vector<SR> SRVec;
  SR sr;
  SR baseSR;

  //first set binning in njet-nbjet plane
  sr.SetName("1");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("2");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("3");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("4");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("5");
  sr.SetVar("njets", 4, 7);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("6");
  sr.SetVar("njets", 4, 7);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("7");
  sr.SetVar("njets", 4, 7);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("8");
  sr.SetVar("njets", 4, 7);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("9");
  sr.SetVar("njets", 7, 9);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("10");
  sr.SetVar("njets", 7, 9);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("11");
  sr.SetVar("njets", 7, 9);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("12");
  sr.SetVar("njets", 7, 9);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("13");
  sr.SetVar("njets", 9, -1);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("14");
  sr.SetVar("njets", 9, -1);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("15");
  sr.SetVar("njets", 9, -1);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("16");
  sr.SetVar("njets", 9, -1);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("17");
  sr.SetVar("njets", 2, 7);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("18");
  sr.SetVar("njets", 2, 7);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("19");
  sr.SetVar("njets", 7, -1);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("20");
  sr.SetVar("njets", 7, -1);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();


  //add HT and MET requirements
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "L");
    fullSR.SetVar("ht", 450, 575);
    fullSR.SetVar("met", 200, -1);
    SRVec.push_back(fullSR);
  }
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "M");
    fullSR.SetVar("ht", 575, 1000);
    fullSR.SetVar("met", 200, -1);
    SRVec.push_back(fullSR);
  }
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "H");
    fullSR.SetVar("ht", 1000, -1);
    fullSR.SetVar("met", 30, -1);
    SRVec.push_back(fullSR);
  }

  //define baseline selections commmon to all signal regions 
  baseSR.SetVar("mt2", 200, -1);
  baseSR.SetVar("j1pt", 100, -1);
  baseSR.SetVar("j2pt", 100, -1);
  baseSR.SetVar("deltaPhiMin", 0.3, -1);
  baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
  baseSR.SetVar("nlep", 0, 1);
  //baseSR.SetVar("passesHtMet", 1, 1);

  //add baseline selections to all signal regions 
  std::vector<std::string> vars = baseSR.GetListOfVariables();
  for(unsigned int i = 0; i < SRVec.size(); i++){
    for(unsigned int j = 0; j < vars.size(); j++){
      SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
    }
  }

  return SRVec;

}

std::vector<SR> getSignalRegions2015ExtendedNJets_UltraHighHT(){

  std::vector<SR> temp_SR_vec;
  std::vector<SR> SRVec;
  SR sr;
  SR baseSR;

  //first set binning in njet-nbjet plane
  sr.SetName("1");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("2");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("3");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("4");
  sr.SetVar("njets", 2, 4);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("5");
  sr.SetVar("njets", 4, 7);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("6");
  sr.SetVar("njets", 4, 7);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("7");
  sr.SetVar("njets", 4, 7);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("8");
  sr.SetVar("njets", 4, 7);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("9");
  sr.SetVar("njets", 7, 9);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("10");
  sr.SetVar("njets", 7, 9);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("11");
  sr.SetVar("njets", 7, 9);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("12");
  sr.SetVar("njets", 7, 9);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("13");
  sr.SetVar("njets", 9, -1);
  sr.SetVar("nbjets", 0, 1);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("14");
  sr.SetVar("njets", 9, -1);
  sr.SetVar("nbjets", 1, 2);
  sr.SetVar("lowMT", 0, -1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("15");
  sr.SetVar("njets", 9, -1);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("16");
  sr.SetVar("njets", 9, -1);
  sr.SetVar("nbjets", 2, 3);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("17");
  sr.SetVar("njets", 2, 7);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("18");
  sr.SetVar("njets", 2, 7);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("19");
  sr.SetVar("njets", 7, -1);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("lowMT", 1, 2);
  temp_SR_vec.push_back(sr);
  sr.Clear();

  sr.SetName("20");
  sr.SetVar("njets", 7, -1);
  sr.SetVar("nbjets", 3, -1);
  sr.SetVar("lowMT", 0, 1);
  temp_SR_vec.push_back(sr);
  sr.Clear();


  //add HT and MET requirements
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "L");
    fullSR.SetVar("ht", 450, 575);
    fullSR.SetVar("met", 200, -1);
    SRVec.push_back(fullSR);
  }
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "M");
    fullSR.SetVar("ht", 575, 1000);
    fullSR.SetVar("met", 200, -1);
    SRVec.push_back(fullSR);
  }
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "H");
    fullSR.SetVar("ht", 1000, 1500);
    fullSR.SetVar("met", 30, -1);
    SRVec.push_back(fullSR);
  }
  for(unsigned int iSR = 0; iSR < temp_SR_vec.size(); iSR++){
    SR fullSR = temp_SR_vec.at(iSR);  
    fullSR.SetName(fullSR.GetName() + "UH");
    fullSR.SetVar("ht", 1500, -1);
    fullSR.SetVar("met", 30, -1);
    SRVec.push_back(fullSR);
  }

  //define baseline selections commmon to all signal regions 
  baseSR.SetVar("mt2", 200, -1);
  baseSR.SetVar("j1pt", 100, -1);
  baseSR.SetVar("j2pt", 100, -1);
  baseSR.SetVar("deltaPhiMin", 0.3, -1);
  baseSR.SetVar("diffMetMhtOverMet", 0, 0.5);
  baseSR.SetVar("nlep", 0, 1);
  //baseSR.SetVar("passesHtMet", 1, 1);

  //add baseline selections to all signal regions 
  std::vector<std::string> vars = baseSR.GetListOfVariables();
  for(unsigned int i = 0; i < SRVec.size(); i++){
    for(unsigned int j = 0; j < vars.size(); j++){
      SRVec.at(i).SetVar(vars.at(j), baseSR.GetLowerBound(vars.at(j)), baseSR.GetUpperBound(vars.at(j)));
    }
  }

  return SRVec;

}

} // namespace mt2
