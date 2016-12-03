#include <stdexcept>
#include <iostream>
#include <string>
#include <cmath>

#include "SR.h"

SR::SR(){
  Clear();
}

SR::~SR(){
}

void SR::SetName(std::string sr_name){
  srName_ = sr_name;
}

void SR::SetVar(std::string var_name, float lower_bound, float upper_bound){
  bins_[var_name] = std::pair<float, float>(lower_bound, upper_bound);
}

void SR::SetVarCRSL(std::string var_name, float lower_bound, float upper_bound){
  binsCRSL_[var_name] = std::pair<float, float>(lower_bound, upper_bound);
}

void SR::SetVarCRDY(std::string var_name, float lower_bound, float upper_bound){
  binsCRDY_[var_name] = std::pair<float, float>(lower_bound, upper_bound);
}

void SR::SetVarCRQCD(std::string var_name, float lower_bound, float upper_bound){
  binsCRQCD_[var_name] = std::pair<float, float>(lower_bound, upper_bound);
}

void SR::SetMT2Bins(int nbins, float* bins){
  n_mt2bins_ = nbins;
  mt2bins_ = new float [n_mt2bins_+1];
  for(int i=0; i<=nbins; i++){
    mt2bins_[i] = bins[i];
  }
}

std::string SR::GetName(){
  return srName_;
}

float SR::GetLowerBound(std::string var_name){
  return (bins_.at(var_name)).first;
}

float SR::GetUpperBound(std::string var_name){
  return (bins_.at(var_name)).second;
}

unsigned int SR::GetNumberOfVariables(){
  return (bins_.size());
}

std::vector<std::string> SR::GetListOfVariables(){
  std::vector<std::string> vars;
  for(std::map<std::string, std::pair<float, float> >::const_iterator it = bins_.begin(); it != bins_.end(); ++it) {
    vars.push_back(it->first);
  }
  return (vars);
}

float SR::GetLowerBoundCRSL(std::string var_name){
  return (binsCRSL_.at(var_name)).first;
}

float SR::GetUpperBoundCRSL(std::string var_name){
  return (binsCRSL_.at(var_name)).second;
}

unsigned int SR::GetNumberOfVariablesCRSL(){
  return (binsCRSL_.size());
}

std::vector<std::string> SR::GetListOfVariablesCRSL(){
  std::vector<std::string> vars;
  for(std::map<std::string, std::pair<float, float> >::const_iterator it = binsCRSL_.begin(); it != binsCRSL_.end(); ++it) {
    vars.push_back(it->first);
  }
  return (vars);
}

float SR::GetLowerBoundCRDY(std::string var_name){
  return (binsCRDY_.at(var_name)).first;
}

float SR::GetUpperBoundCRDY(std::string var_name){
  return (binsCRDY_.at(var_name)).second;
}

unsigned int SR::GetNumberOfVariablesCRDY(){
  return (binsCRDY_.size());
}

std::vector<std::string> SR::GetListOfVariablesCRDY(){
  std::vector<std::string> vars;
  for(std::map<std::string, std::pair<float, float> >::const_iterator it = binsCRDY_.begin(); it != binsCRDY_.end(); ++it) {
    vars.push_back(it->first);
  }
  return (vars);
}

float SR::GetLowerBoundCRQCD(std::string var_name){
  return (binsCRQCD_.at(var_name)).first;
}

float SR::GetUpperBoundCRQCD(std::string var_name){
  return (binsCRQCD_.at(var_name)).second;
}

unsigned int SR::GetNumberOfVariablesCRQCD(){
  return (binsCRQCD_.size());
}

std::vector<std::string> SR::GetListOfVariablesCRQCD(){
  std::vector<std::string> vars;
  for(std::map<std::string, std::pair<float, float> >::const_iterator it = binsCRQCD_.begin(); it != binsCRQCD_.end(); ++it) {
    vars.push_back(it->first);
  }
  return (vars);
}

float* SR::GetMT2Bins(){
  return mt2bins_;
}

int SR::GetNumberOfMT2Bins(){
  return n_mt2bins_;
}

bool SR::PassesSelection(std::map<std::string, float> values){
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

bool SR::PassesSelectionCRSL(std::map<std::string, float> values){
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

bool SR::PassesSelectionCRDY(std::map<std::string, float> values){
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

bool SR::PassesSelectionCRQCD(std::map<std::string, float> values){
  float ep = 0.000001;
  if(GetNumberOfVariablesCRQCD() != values.size()){
    std::cout << "Number of variables to cut on != number of variables in CRQCD region. Passed " << values.size() << ", expected " << GetNumberOfVariablesCRQCD() << std::endl;
    throw std::invalid_argument("Number of variables to cut on != number of variables in CRQCD region");
  }
  for(std::map<std::string, float>::const_iterator it = values.begin(); it != values.end(); it++){
    if(binsCRQCD_.find(it->first) != binsCRQCD_.end()){ //check that we actually have bounds set for this variable
      float value = it->second;
      float cut_lower = GetLowerBoundCRQCD(it->first);
      float cut_upper = GetUpperBoundCRQCD(it->first);
      if(value < cut_lower) return false;
      if(( std::abs(cut_upper + 1.0) > ep ) && (value >= cut_upper)) return false;
    }
    else{
      throw std::invalid_argument("Cut values not set for this variable");
    }
  }
  return true;
}

void SR::RemoveVar(std::string var_name){
  if(bins_.find(var_name) != bins_.end()) bins_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SR::RemoveVarCRSL(std::string var_name){
  if(binsCRSL_.find(var_name) != binsCRSL_.end()) binsCRSL_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SR::RemoveVarCRDY(std::string var_name){
  if(binsCRDY_.find(var_name) != binsCRDY_.end()) binsCRDY_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SR::RemoveVarCRQCD(std::string var_name){
  if(binsCRQCD_.find(var_name) != binsCRQCD_.end()) binsCRQCD_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SR::Clear(){
  srName_ = "";
  bins_.clear();
  binsCRSL_.clear();
  binsCRDY_.clear();
  binsCRQCD_.clear();
}
