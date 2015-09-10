#ifndef SR_h
#define SR_h

#include <string>
#include <map>
#include <vector>

#include "TH1.h"
#include "RooDataSet.h"


class SR {

  public:

    SR();
    ~SR();

    void SetName(std::string sr_name);
    void SetVar(std::string var_name, float lower_bound, float upper_bound);
    void SetVarCRSL(std::string var_name, float lower_bound, float upper_bound);
    void SetMT2Bins(int nbins, float* bins);

    std::string GetName();

    float GetLowerBound(std::string var_name);
    float GetUpperBound(std::string var_name);
    unsigned int GetNumberOfVariables();
    std::vector<std::string> GetListOfVariables();

    float GetLowerBoundCRSL(std::string var_name);
    float GetUpperBoundCRSL(std::string var_name);
    unsigned int GetNumberOfVariablesCRSL();
    std::vector<std::string> GetListOfVariablesCRSL();

    float* GetMT2Bins();
    int GetNumberOfMT2Bins();

    bool PassesSelection(std::map<std::string, float> values);
    bool PassesSelectionCRSL(std::map<std::string, float> values);
    void RemoveVar(std::string var_name);
    void RemoveVarCRSL(std::string var_name);
    void Clear();

    //used for plotting
    std::map<std::string, TH1*> srHistMap;
    std::map<std::string, TH1*> crslHistMap;
    std::map<std::string, TH1*> crslmuHistMap;
    std::map<std::string, TH1*> crslelHistMap;
    std::map<std::string, TH1*> crslhadHistMap;
    std::map<std::string, TH1*> crgjHistMap;
    std::map<std::string, RooDataSet*> crgjRooDataSetMap;
    std::map<std::string, TH1*> crdyHistMap;
    std::map<std::string, TH1*> crrlHistMap;
    std::map<std::string, TH1*> crrlmuHistMap;
    std::map<std::string, TH1*> crrlelHistMap;

  private:

    std::string srName_;
    std::map<std::string, std::pair<float, float> > bins_;
    std::map<std::string, std::pair<float, float> > binsCRSL_;
    int n_mt2bins_;
    float *mt2bins_;

};

#endif
