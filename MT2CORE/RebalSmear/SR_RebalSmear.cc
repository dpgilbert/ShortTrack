#include <stdexcept>
#include <iostream>
#include <string>
#include <cmath>

#include "SR_RebalSmear.h"

SRRS::SRRS(){
  Clear();
}

SRRS::~SRRS(){
}

void SRRS::SetName(std::string sr_name){
  srName_ = sr_name;
}

void SRRS::SetVar(std::string var_name, float lower_bound, float upper_bound){
  bins_[var_name] = std::pair<float, float>(lower_bound, upper_bound);
}

void SRRS::SetVarCRSL(std::string var_name, float lower_bound, float upper_bound){
  binsCRSL_[var_name] = std::pair<float, float>(lower_bound, upper_bound);
}

void SRRS::SetVarCRDY(std::string var_name, float lower_bound, float upper_bound){
  binsCRDY_[var_name] = std::pair<float, float>(lower_bound, upper_bound);
}

void SRRS::SetVarCRRSInvertDPhi(std::string var_name, float lower_bound, float upper_bound){
  binsCRRSInvertDPhi_[var_name] = std::pair<float, float>(lower_bound, upper_bound);
}

void SRRS::SetVarCRRSMT2SideBand(std::string var_name, float lower_bound, float upper_bound){
  binsCRRSMT2SideBand_[var_name] = std::pair<float, float>(lower_bound, upper_bound);
}

void SRRS::SetVarCRRSDPhiMT2(std::string var_name, float lower_bound, float upper_bound){
  binsCRRSDPhiMT2_[var_name] = std::pair<float, float>(lower_bound, upper_bound);
}

void SRRS::SetMT2Bins(int nbins, float* bins){
  n_mt2bins_ = nbins;
  mt2bins_ = new float [n_mt2bins_+1];
  for(int i=0; i<=nbins; i++){
    mt2bins_[i] = bins[i];
  }
}

std::string SRRS::GetName(){
  return srName_;
}

float SRRS::GetLowerBound(std::string var_name){
  return (bins_.at(var_name)).first;
}

float SRRS::GetUpperBound(std::string var_name){
  return (bins_.at(var_name)).second;
}

unsigned int SRRS::GetNumberOfVariables(){
  return (bins_.size());
}

std::vector<std::string> SRRS::GetListOfVariables(){
  std::vector<std::string> vars;
  for(std::map<std::string, std::pair<float, float> >::const_iterator it = bins_.begin(); it != bins_.end(); ++it) {
    vars.push_back(it->first);
  }
  return (vars);
}

float SRRS::GetLowerBoundCRSL(std::string var_name){
  return (binsCRSL_.at(var_name)).first;
}

float SRRS::GetUpperBoundCRSL(std::string var_name){
  return (binsCRSL_.at(var_name)).second;
}

unsigned int SRRS::GetNumberOfVariablesCRSL(){
  return (binsCRSL_.size());
}

std::vector<std::string> SRRS::GetListOfVariablesCRSL(){
  std::vector<std::string> vars;
  for(std::map<std::string, std::pair<float, float> >::const_iterator it = binsCRSL_.begin(); it != binsCRSL_.end(); ++it) {
    vars.push_back(it->first);
  }
  return (vars);
}

float SRRS::GetLowerBoundCRDY(std::string var_name){
  return (binsCRDY_.at(var_name)).first;
}

float SRRS::GetUpperBoundCRDY(std::string var_name){
  return (binsCRDY_.at(var_name)).second;
}

unsigned int SRRS::GetNumberOfVariablesCRDY(){
  return (binsCRDY_.size());
}

std::vector<std::string> SRRS::GetListOfVariablesCRDY(){
  std::vector<std::string> vars;
  for(std::map<std::string, std::pair<float, float> >::const_iterator it = binsCRDY_.begin(); it != binsCRDY_.end(); ++it) {
    vars.push_back(it->first);
  }
  return (vars);
}

float SRRS::GetLowerBoundCRRSInvertDPhi(std::string var_name){
  return (binsCRRSInvertDPhi_.at(var_name)).first;
}

float SRRS::GetUpperBoundCRRSInvertDPhi(std::string var_name){
  return (binsCRRSInvertDPhi_.at(var_name)).second;
}

unsigned int SRRS::GetNumberOfVariablesCRRSInvertDPhi(){
  return (binsCRRSInvertDPhi_.size());
}

std::vector<std::string> SRRS::GetListOfVariablesCRRSInvertDPhi(){
  std::vector<std::string> vars;
  for(std::map<std::string, std::pair<float, float> >::const_iterator it = binsCRRSInvertDPhi_.begin(); it != binsCRRSInvertDPhi_.end(); ++it) {
    vars.push_back(it->first);
  }
  return (vars);
}

float SRRS::GetLowerBoundCRRSMT2SideBand(std::string var_name){
  return (binsCRRSMT2SideBand_.at(var_name)).first;
}

float SRRS::GetUpperBoundCRRSMT2SideBand(std::string var_name){
  return (binsCRRSMT2SideBand_.at(var_name)).second;
}

unsigned int SRRS::GetNumberOfVariablesCRRSMT2SideBand(){
  return (binsCRRSMT2SideBand_.size());
}

std::vector<std::string> SRRS::GetListOfVariablesCRRSMT2SideBand(){
  std::vector<std::string> vars;
  for(std::map<std::string, std::pair<float, float> >::const_iterator it = binsCRRSMT2SideBand_.begin(); it != binsCRRSMT2SideBand_.end(); ++it) {
    vars.push_back(it->first);
  }
  return (vars);
}

float SRRS::GetLowerBoundCRRSDPhiMT2(std::string var_name){
  return (binsCRRSDPhiMT2_.at(var_name)).first;
}

float SRRS::GetUpperBoundCRRSDPhiMT2(std::string var_name){
  return (binsCRRSDPhiMT2_.at(var_name)).second;
}

unsigned int SRRS::GetNumberOfVariablesCRRSDPhiMT2(){
  return (binsCRRSDPhiMT2_.size());
}

std::vector<std::string> SRRS::GetListOfVariablesCRRSDPhiMT2(){
  std::vector<std::string> vars;
  for(std::map<std::string, std::pair<float, float> >::const_iterator it = binsCRRSDPhiMT2_.begin(); it != binsCRRSDPhiMT2_.end(); ++it) {
    vars.push_back(it->first);
  }
  return (vars);
}

float* SRRS::GetMT2Bins(){
  return mt2bins_;
}

int SRRS::GetNumberOfMT2Bins(){
  return n_mt2bins_;
}

bool SRRS::PassesSelection(std::map<std::string, float> values){
  float ep = 0.000001;
  if(GetNumberOfVariables() != values.size()){
    throw std::invalid_argument("Number of variables to cut on != number of variables in signal region");
  }
  for(std::map<std::string, float>::const_iterator it = values.begin(); it != values.end(); it++){
    if(bins_.find(it->first) != bins_.end()){ //check that we actually have bounds set for this variable
      float value = it->second;
      float cut_lower = GetLowerBound(it->first);
      float cut_upper = GetUpperBound(it->first);
      if(value < cut_lower) return false;
      if(( std::abs(cut_upper + 1.0) > ep ) && (value >= cut_upper)) return false;
    }
    else{
      throw std::invalid_argument("Cut values not set for this variable");
    }
  }
  return true;
}

bool SRRS::PassesSelectionCRSL(std::map<std::string, float> values){
  float ep = 0.000001;
  if(GetNumberOfVariablesCRSL() != values.size()){
    std::cout << "Number of variables to cut on != number of variables in CRSL region. Passed " << values.size() << ", expected " << GetNumberOfVariablesCRSL() << std::endl;
    throw std::invalid_argument("Number of variables to cut on != number of variables in CRSL region");
  }
  for(std::map<std::string, float>::const_iterator it = values.begin(); it != values.end(); it++){
    if(binsCRSL_.find(it->first) != binsCRSL_.end()){ //check that we actually have bounds set for this variable
      float value = it->second;
      float cut_lower = GetLowerBoundCRSL(it->first);
      float cut_upper = GetUpperBoundCRSL(it->first);
      if(value < cut_lower) return false;
      if(( std::abs(cut_upper + 1.0) > ep ) && (value >= cut_upper)) return false;
    }
    else{
      throw std::invalid_argument("Cut values not set for this variable");
    }
  }
  return true;
}

bool SRRS::PassesSelectionCRDY(std::map<std::string, float> values){
  float ep = 0.000001;
  if(GetNumberOfVariablesCRDY() != values.size()){
    std::cout << "Number of variables to cut on != number of variables in CRDY region. Passed " << values.size() << ", expected " << GetNumberOfVariablesCRDY() << std::endl;
    throw std::invalid_argument("Number of variables to cut on != number of variables in CRDY region");
  }
  for(std::map<std::string, float>::const_iterator it = values.begin(); it != values.end(); it++){
    if(binsCRDY_.find(it->first) != binsCRDY_.end()){ //check that we actually have bounds set for this variable
      float value = it->second;
      float cut_lower = GetLowerBoundCRDY(it->first);
      float cut_upper = GetUpperBoundCRDY(it->first);
      if(value < cut_lower) return false;
      if(( std::abs(cut_upper + 1.0) > ep ) && (value >= cut_upper)) return false;
    }
    else{
      throw std::invalid_argument("Cut values not set for this variable");
    }
  }
  return true;
}

bool SRRS::PassesSelectionCRRSInvertDPhi(std::map<std::string, float> values){
  float ep = 0.000001;
  if(GetNumberOfVariablesCRRSInvertDPhi() != values.size()){
    std::cout << "Number of variables to cut on != number of variables in CRRSInvertDPhi region. Passed " << values.size() << ", expected " << GetNumberOfVariablesCRRSInvertDPhi() << std::endl;
    throw std::invalid_argument("Number of variables to cut on != number of variables in CRRSInvertDPhi region");
  }
  for(std::map<std::string, float>::const_iterator it = values.begin(); it != values.end(); it++){
    if(binsCRRSInvertDPhi_.find(it->first) != binsCRRSInvertDPhi_.end()){ //check that we actually have bounds set for this variable
      float value = it->second;
      float cut_lower = GetLowerBoundCRRSInvertDPhi(it->first);
      float cut_upper = GetUpperBoundCRRSInvertDPhi(it->first);
      if(value < cut_lower) return false;
      if(( std::abs(cut_upper + 1.0) > ep ) && (value >= cut_upper)) return false;
    }
    else{
      throw std::invalid_argument("Cut values not set for this variable");
    }
  }
  return true;
}

bool SRRS::PassesSelectionCRRSMT2SideBand(std::map<std::string, float> values){
  float ep = 0.000001;
  if(GetNumberOfVariablesCRRSMT2SideBand() != values.size()){
    std::cout << "Number of variables to cut on != number of variables in CRRSMT2SideBand region. Passed " << values.size() << ", expected " << GetNumberOfVariablesCRRSMT2SideBand() << std::endl;
    throw std::invalid_argument("Number of variables to cut on != number of variables in CRRSMT2SideBand region");
  }
  for(std::map<std::string, float>::const_iterator it = values.begin(); it != values.end(); it++){
    if(binsCRRSMT2SideBand_.find(it->first) != binsCRRSMT2SideBand_.end()){ //check that we actually have bounds set for this variable
      float value = it->second;
      float cut_lower = GetLowerBoundCRRSMT2SideBand(it->first);
      float cut_upper = GetUpperBoundCRRSMT2SideBand(it->first);
      if(value < cut_lower) return false;
      if(( std::abs(cut_upper + 1.0) > ep ) && (value >= cut_upper)) return false;
    }
    else{
      throw std::invalid_argument("Cut values not set for this variable");
    }
  }
  return true;
}

bool SRRS::PassesSelectionCRRSDPhiMT2(std::map<std::string, float> values){
  float ep = 0.000001;
  if(GetNumberOfVariablesCRRSDPhiMT2() != values.size()){
    std::cout << "Number of variables to cut on != number of variables in CRRSDPhiMT2 region. Passed " << values.size() << ", expected " << GetNumberOfVariablesCRRSDPhiMT2() << std::endl;
    throw std::invalid_argument("Number of variables to cut on != number of variables in CRRSDPhiMT2 region");
  }
  for(std::map<std::string, float>::const_iterator it = values.begin(); it != values.end(); it++){
    if(binsCRRSDPhiMT2_.find(it->first) != binsCRRSDPhiMT2_.end()){ //check that we actually have bounds set for this variable
      float value = it->second;
      float cut_lower = GetLowerBoundCRRSDPhiMT2(it->first);
      float cut_upper = GetUpperBoundCRRSDPhiMT2(it->first);
      if(value < cut_lower) return false;
      if(( std::abs(cut_upper + 1.0) > ep ) && (value >= cut_upper)) return false;
    }
    else{
      throw std::invalid_argument("Cut values not set for this variable");
    }
  }
  return true;
}

void SRRS::RemoveVar(std::string var_name){
  if(bins_.find(var_name) != bins_.end()) bins_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SRRS::RemoveVarCRSL(std::string var_name){
  if(binsCRSL_.find(var_name) != binsCRSL_.end()) binsCRSL_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SRRS::RemoveVarCRDY(std::string var_name){
  if(binsCRDY_.find(var_name) != binsCRDY_.end()) binsCRDY_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SRRS::RemoveVarCRRSInvertDPhi(std::string var_name){
  if(binsCRRSInvertDPhi_.find(var_name) != binsCRRSInvertDPhi_.end()) binsCRRSInvertDPhi_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SRRS::RemoveVarCRRSMT2SideBand(std::string var_name){
  if(binsCRRSMT2SideBand_.find(var_name) != binsCRRSMT2SideBand_.end()) binsCRRSMT2SideBand_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SRRS::RemoveVarCRRSDPhiMT2(std::string var_name){
  if(binsCRRSDPhiMT2_.find(var_name) != binsCRRSDPhiMT2_.end()) binsCRRSDPhiMT2_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SRRS::Clear(){
  srName_ = "";
  bins_.clear();
  binsCRSL_.clear();
  binsCRDY_.clear();
  binsCRRSInvertDPhi_.clear();
  binsCRRSMT2SideBand_.clear();
  binsCRRSDPhiMT2_.clear();
}
