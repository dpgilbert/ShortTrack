# plot_defs.py
# collect the plot definitions for the various control regions
# format is:
#     (var_name, isLog, (xmin,xmax), (ymin,ymax), Rebin(optional), doOverflow(opt, def true))
# use None to use default params

lumi = 804
lumiUnit = "pb"

rl_plots = [
    ("ht",True,(200,1500),None),
    ("met",True,(200,1000),None),
    ("mt2",True,(200,1000),None),
    ("mt2bins",True,None,None),
    ("nJet30",True,(0,14),None),
    ("nBJet20",True,(0,6),None),
    ("leppt",True,None,None),
    ("lepeta",False,None,None),
    ("nlepveto",False,None,None),
]

sl_plots = [
    ("ht",True,(200,1500),None),
    ("met",True,(200,1000),None),
    ("mt2",True,(200,1000),None),
    ("mt2bins",True,None,None),
    ("nJet30",True,(0,14),None),
    ("nBJet20",True,(0,6),None),
    ("leppt",True,(0,400),None),
]

dy_plots = [
    ("ht",True,(200,1500),None),
    ("met",True,(200,1000),None),
    ("mt2",True,(200,1000),None),
    ("mt2bins",True,None,None),
    ("nJet30",True,(0,14),None),
    ("nBJet20",True,(0,6),None),
    ("zllmass",True,(70,120),None),
    ("nlepveto",False,None,None),
]

gj_plots = [
    ("ht",True,(200,1500),None),
    ("met",True,(200,1000),None),
    ("mt2",True,(200,1000),None),
    ("mt2bins",True,None,None),
    ("nJet30",True,(0,14),None),
    ("nBJet20",True,(0,6),None),
    ("gammaPt",True,None,None,4),
    ("gammaEta",False,None,None),
    ("nlepveto",False,None,None),
    ("chisoLoose",True,(0,20),None),
    ("chisoEELoose",True,(0,20),None),
    ("chisoEBLoose",True,(0,20),None),
    ("chisoLooseSieieSB",True,(0,20),None),
    ("chisoEELooseSieieSB",True,(0,20),None),
    ("chisoEBLooseSieieSB",True,(0,20),None),
    ("drMinParton",False,None,None),
    ("drMinParton_ht200to450",False,None,None),
    ("drMinParton_ht450to1000",False,None,None),
    ("drMinParton_ht1000toInf",False,None,None),
]

qcdJ_plots = [
    ("J1pt",False,(30,330),None,1,False)
]

mt2bins_only = [
    ("mt2bins",True,None,None)
]
