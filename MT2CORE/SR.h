#ifndef SR_h
#define SR_h

#include <string>
#include <map>
#include <vector>

#include "TH1.h"


class SR {

  public:

    SR();
    ~SR();

    void SetName(std::string sr_name);
    void SetVar(std::string var_name, float lower_bound, float upper_bound);

    std::string GetName();
    float GetLowerBound(std::string var_name);
    float GetUpperBound(std::string var_name);
    unsigned int GetNumberOfVariables();
    std::vector<std::string> GetListOfVariables();

    bool PassesSelection(std::map<std::string, float> values);
    void RemoveVar(std::string var_name);
    void Clear();

    //used for plotting
    std::map<std::string, TH1D*> srHistMap;
    std::map<std::string, TH1D*> crslHistMap;
    std::map<std::string, TH1D*> crslmuHistMap;
    std::map<std::string, TH1D*> crslelHistMap;
    std::map<std::string, TH1D*> crslhadHistMap;
    std::map<std::string, TH1D*> crgjHistMap;
    std::map<std::string, TH1D*> crdyHistMap;

  private:

    std::string srName_;
    std::map<std::string, std::pair<float, float> > bins_;

};

#endif
