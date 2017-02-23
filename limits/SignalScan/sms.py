from array import *

class sms():

    def __init__(self, modelname):
        if modelname.find("T1tttt") != -1: self.T1tttt()
        if modelname.find("T1bbbb") != -1: self.T1bbbb()
        if modelname.find("T1qqqq") != -1: self.T1qqqq()
        if modelname.find("T5qqqqWW") != -1: self.T5qqqqWW()
        if modelname.find("T2tt") != -1: self.T2tt()
        if modelname.find("T2bb") != -1: self.T2bb()
        if modelname.find("T2-4bd") != -1: self.T24bd()
        if modelname.find("Axial1p0") != -1: self.Axial1p0()
        if modelname.find("Axial0p25") != -1: self.Axial0p25()
        if modelname.find("Vector1p0") != -1: self.Vector1p0()
        if modelname.find("Vector0p25") != -1: self.Vector0p25()


    def T1tttt(self):
        # model name
        self.modelname = "T1tttt"
        # decay chain
        self.label= "pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow t #bar{t} #tilde{#chi}^{0}_{1}";
        self.label2= "";
        # scan range to plot
        self.Xmin = 600.
        self.Xmax = 2200.
        self.Ymin = 0.
        self.Ymax = 1800.
        self.Zmin = 0.001
        self.Zmax = 2.
        # produce sparticle
        self.sParticle = "m_{#tilde{g}} [GeV]"
        # LSP
        self.LSP = "m_{#tilde{#chi}_{1}^{0}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False
        
    def T1bbbb(self):
        # model name
        self.modelname = "T1bbbb"
        # decay chain
        self.label= "pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow b #bar{b} #tilde{#chi}^{0}_{1}";
        self.label2= "";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 600.
        self.Xmax = 2300.
        self.Ymin = 0.
        self.Ymax = 2000.
        self.Zmin = 0.0001
        self.Zmax = 2.
        # produce sparticle
        self.sParticle = "m_{#tilde{g}} [GeV]"
        # LSP
        self.LSP = "m_{#tilde{#chi}_{1}^{0}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False

    def T1qqqq(self):
        # model name
        self.modelname = "T1qqqq"
        # decay chain
        self.label= "pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow q #bar{q} #tilde{#chi}^{0}_{1}";
        self.label2= "";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 600.
        self.Xmax = 1950.
        self.Ymin = 0.
        self.Ymax = 1600.
        self.Zmin = 0.001
        self.Zmax = 2.
        # produce sparticle
        self.sParticle = "m_{#tilde{g}} [GeV]"
        # LSP
        self.LSP = "m_{#tilde{#chi}_{1}^{0}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False

    def T5qqqqWW(self):
        # model name
        self.modelname = "T5qqqqWW"
        # decay chain
        lsp_s = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
        chargino_s = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{#pm}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
        self.label= "pp #rightarrow #tilde{g} #tilde{g}, #tilde{g} #rightarrow q #bar{q} "+ chargino_s +", "+chargino_s+" #rightarrow f f' "+lsp_s;
        self.label2= "m("+chargino_s+") - m("+lsp_s+") = 20 GeV";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 600.
        self.Xmax = 1950.
        self.Ymin = 0.
        self.Ymax = 1600.
        self.Zmin = 0.01
        self.Zmax = 10.
        # produce sparticle
        self.sParticle = "m#kern[0.1]{_{#lower[-0.12]{#tilde{g}}}} [GeV]"
        # LSP
        self.LSP = "m#kern[0.1]{_{"+lsp_s+"}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False

    def T2tt(self):
        # model name
        self.modelname = "T2tt"
        # decay chain
        self.label= "pp #rightarrow #tilde{t} #tilde{t}, #tilde{t} #rightarrow t #tilde{#chi}^{0}_{1}";
        self.label2= "";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 150.
        self.Xmax = 1200.
        self.Ymin = 0.
        self.Ymax = 700.
        self.Zmin = 0.001
        self.Zmax = 100.
        # produce sparticle
        self.sParticle = "m_{#tilde{t}} [GeV]"
        # LSP
        self.LSP = "m_{#tilde{#chi}_{1}^{0}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False

    def T2bb(self):
        # model name
        self.modelname = "T2bb"
        # decay chain
        self.label= "pp #rightarrow #tilde{b} #tilde{b}, #tilde{b} #rightarrow b #tilde{#chi}^{0}_{1}";
        self.label2= "";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 300.
        self.Xmax = 1400.
        self.Ymin = 0.
        self.Ymax = 1000.
        self.Zmin = 0.001
        self.Zmax = 2.
        # produce sparticle
        self.sParticle = "m_{#tilde{b}} [GeV]"
        # LSP
        self.LSP = "m_{#tilde{#chi}_{1}^{0}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False

    def T2cc(self):
        # model name
        self.modelname = "T2cc"
        # decay chain
        self.label= "pp #rightarrow #tilde{t} #tilde{t}, #tilde{t} #rightarrow c #tilde{#chi}^{0}_{1}";
        self.label2= "";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 300.
        self.Xmax = 800.
        self.Ymin = 0.
        self.Ymax = 1000.
        self.Zmin = 0.001
        self.Zmax = 100.
        # produce sparticle
        self.sParticle = "m_{#tilde{t}} [GeV]"
        # LSP
        self.LSP = "m_{#tilde{#chi}_{1}^{0}} [GeV]"
        # turn off diagonal lines
        self.diagOn = True

    def T24bd(self):
        # model name
        self.modelname = "T2-4bd"
        # decay chain
        lsp_s = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}"
        self.label= "pp #rightarrow #tilde{t} #tilde{t}, #tilde{t} #rightarrow b f f' "+lsp_s;
        self.label2= "";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 100.
        self.Xmax = 600.
        self.Ymin = 0.
        self.Ymax = 750.
        self.Zmin = 0.5
        self.Zmax = 100.
        # produce sparticle
        self.sParticle = "m#kern[0.1]{_{#lower[-0.12]{#tilde{t}}}} [GeV]"
        # LSP
        self.LSP = "m#kern[0.1]{_{"+lsp_s+"}} [GeV]"
        # turn off diagonal lines
        self.diagOn = False

    def Axial1p0(self):
        # model name
        self.modelname = "Axial1p0"
        # decay chain
        self.label= "Axial 1p0";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 100.
        self.Xmax = 2000.
        self.Ymin = 0.
        self.Ymax = 500.
        self.Zmin = 0.001
        self.Zmax = 5.
        # produce sparticle
        self.sParticle = "m_{med} [GeV]"
        # LSP
        self.LSP = "m_{DM} [GeV]"
        # turn off diagonal lines
        self.diagOn = False

    def Axial0p25(self):
        # model name
        self.modelname = "Axial0p25"
        # decay chain
        self.label= "Axial 0p25";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 100.
        self.Xmax = 2000.
        self.Ymin = 0.
        self.Ymax = 500.
        self.Zmin = 0.001
        self.Zmax = 5.
        # produce sparticle
        self.sParticle = "m_{med} [GeV]"
        # LSP
        self.LSP = "m_{DM} [GeV]"
        # turn off diagonal lines
        self.diagOn = False

    def Vector1p0(self):
        # model name
        self.modelname = "Vector1p0"
        # decay chain
        self.label= "Vector 1p0";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 100.
        self.Xmax = 2000.
        self.Ymin = 0.
        self.Ymax = 500.
        self.Zmin = 0.001
        self.Zmax = 5.
        # produce sparticle
        self.sParticle = "m_{med} [GeV]"
        # LSP
        self.LSP = "m_{DM} [GeV]"
        # turn off diagonal lines
        self.diagOn = False


    def Vector0p25(self):
        # model name
        self.modelname = "Vector1p0"
        # decay chain
        self.label= "Vector 1p0";
        # plot boundary. The top 1/4 of the y axis is taken by the legend
        self.Xmin = 100.
        self.Xmax = 2000.
        self.Ymin = 0.
        self.Ymax = 500.
        self.Zmin = 0.001
        self.Zmax = 5.
        # produce sparticle
        self.sParticle = "m_{med} [GeV]"
        # LSP
        self.LSP = "m_{DM} [GeV]"
        # turn off diagonal lines
        self.diagOn = False
