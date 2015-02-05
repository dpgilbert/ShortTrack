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

void SR::RemoveVar(std::string var_name){
  if(bins_.find(var_name) != bins_.end()) bins_.erase(var_name);
  else throw std::invalid_argument("Variable not added. Cannot remove!");
}

void SR::Clear(){
  srName_ = "";
  bins_.clear();
}
