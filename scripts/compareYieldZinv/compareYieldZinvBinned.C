#include "../CMS_lumi.C"

const int iPeriod = 4; // 13 tev
// iPos drives the position of the CMS logo in the plot
// iPos=11 : top-left, left-aligned
// iPos=33 : top-right, right-aligned
// iPos=22 : center, centered
// mode generally :
//   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
const int iPos = 3;


void compareYieldZinvBinned( TString region = "L", bool withMC = false){
  
  //Once you have two histograms, with one bin per topological region (grouped 11 by 11, one block per HT region)
  //Here, hestimate is MC and hdata is data-driven estimate. hestimate color is 401, for QCD MC. hdata color is kBlack.
  //In the bottom pad, we decided to replace the ratio with the pull distribution. h_Pull is the corresponding histogram.
  //hPull is filled with a SetBinContent (iBin, (estimate-MC)/sqrt(sigma(estimate)^2+sigma(MC)^2)), and a SetBinError(iBin, 1.0)
  

  TString savename = "comp_srBinned"+region;

  

  TFile* f1 = 0;
  f1 = new TFile("yield_histograms_ZinvBinned"+region+".root", "READ");
    
  TH1F* hestimate = (TH1F*)f1->Get("yields_MC");
  TH1F* hdata = (TH1F*)f1->Get("yields_Integrated");
  TH1F* hdataBin = (TH1F*)f1->Get("yields_Binned");
  //TH1F* h_Pull = (TH1F*)f1->Get("h_pull");
  TH1F* h_Pull = (TH1F*)f1->Get("h_ratioData");
  TH1F* h_RatioMC = (TH1F*)f1->Get("h_ratioMC");

  TCanvas* c2 = new TCanvas("c2", "", 1100, 600);
  c2->cd();
  
  TPad *pad1 = new TPad("pad1","pad1",0,0.33-0.1,1,1);
  pad1->SetBottomMargin(0.15);
  pad1->SetRightMargin(0.05);
  pad1->SetLeftMargin(0.15);
    gStyle->SetOptStat(0);
    
  pad1->Draw();
  pad1->cd();

  pad1->SetLogy();
  
  float yMax_1 = hdata->GetMaximum()*1.5;
  float yMax_2 = 1.2*(hdata->GetMaximum() + hdata->GetBinError(hestimate->GetMaximumBin()));
  float yMax1 = (yMax_1>yMax_2) ? yMax_1 : yMax_2;
  float yMax_3 = hestimate->GetMaximum()*1.5;
  float yMax_4 = 1.2*(hestimate->GetMaximum() + hestimate->GetBinError(hestimate->GetMaximumBin()));
  float yMax2 = (yMax_3>yMax_4) ? yMax_3 : yMax_4;
  float yMax = (yMax1>yMax2) ? yMax1 : yMax2;
  
  float yMin = 1e-2;
  //float yMin = 1e-3;
  //float yMin = 1e-5;
  yMax*=2.;
  
  int NBins = hestimate->GetNbinsX();
    
  //hestimate->GetXaxis()->SetRangeUser(0, 68);
  hdata->GetYaxis()->SetRangeUser(yMin, yMax);
  hdata->GetYaxis()->SetTitleOffset(0.95);
  hdata->GetYaxis()->SetTitleSize(0.052);
  hdata->GetYaxis()->SetLabelSize(0.042);
  hdata->GetYaxis()->SetTitle("Entries");
  hdata->GetXaxis()->LabelsOption("v");
  hdata->GetXaxis()->SetLabelSize(0.04);
  hdata->GetXaxis()->SetLabelFont(62);
  hdata->SetTitle("");
  hdata->Draw("PE");


  hdata->SetMarkerStyle(20);
  hdata->SetMarkerSize(1.0);
  hdata->SetLineColor( kBlue );
  hdata->SetMarkerColor( kBlue );
    hdataBin->SetMarkerStyle(20);
    hdataBin->SetMarkerSize(1.0);
    hdataBin->SetLineColor( kRed );
    hdataBin->SetMarkerColor( kRed );

  hestimate->SetMarkerStyle(20);
  hestimate->SetMarkerSize(1.0);
  hestimate->SetLineColor( 401 );
  hestimate->SetMarkerColor( 401 );
if (withMC)  hestimate->Draw("PE, same");
//else  hdata->Draw("PE");

    hdataBin->Draw("PE,same");

  TLatex labelZ;
  labelZ.SetNDC();
  labelZ.SetTextSize(0.039);
  labelZ.SetTextFont(62);
  labelZ.DrawLatex(0.7, 0.85, "Z #rightarrow #nu#bar{#nu} background");
  
  TLegend* legend = new TLegend( 0.7, 0.70, 0.93, 0.82 );
  legend->SetBorderSize(0);
  legend->SetTextSize(0.042);
  legend->SetTextFont(42);
  legend->SetFillColor(0);
  //legend->AddEntry( hdata, "RS Pred", "PL" );
    legend->AddEntry( hdata, "Standard", "PL" );
    legend->AddEntry( hdataBin, "Bin by bin", "PL" );
if (withMC)  legend->AddEntry( hestimate, "Z invisible MC", "PL" );
  //legend->AddEntry( hestimate, "Data", "PL" );

  legend->Draw("same");

  //TPaveText* labelTop = MT2DrawTools::getLabelTopSimulation(lumi);
  //labelTop->Draw("same");
  
  //TPaveText* labelTop = new TPaveText(.72,.91,.95,.95, "NDC");
//  TPaveText* labelTop = new TPaveText(.7,.91,.95,.95, "NDC");
//  labelTop->AddText("CMS Unpublished, 2.3 fb^{-1} (13 TeV)");
//  //labelTop->AddText("CMS Data, 0.579 fb^{-1} at #sqrt{s} = 13 TeV");
//  labelTop->SetBorderSize(0);
//  labelTop->SetFillColor(kWhite);
//  labelTop->SetTextSize(0.038);
//  labelTop->Draw("same");
  
  cmsText = "CMS Supplementary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "2.3 fb^{-1}";
  CMS_lumi( pad1, iPeriod, iPos );

  
  TLatex labelA;
  labelA.SetNDC();
  labelA.SetTextSize(0.042);
  labelA.SetTextFont(42);
  labelA.DrawLatex(0.5, 0.92, "arXiv:1603.04053");
  
//  TLine* lHT[5];
//  for( int iHT=0; iHT < 5; iHT++ ){
//    lHT[iHT-1] = new TLine(13+11*iHT, 0.0, 13+11*iHT, yMax );
//    lHT[iHT-1]->SetLineColor(kBlack);
//    lHT[iHT-1]->SetLineStyle(3);
//    lHT[iHT-1]->SetLineWidth(2);
//
//    lHT[iHT-1]->Draw("same");
//  }

//  int nHTRegions = 6;
//  std::vector< std::string > htRegions;
//  htRegions.push_back("1 Jet");
//  htRegions.push_back("very low H_{T}");
//  htRegions.push_back("low H_{T}");
//  htRegions.push_back("medium H_{T}");
//  htRegions.push_back("high H_{T}");
//  htRegions.push_back("extreme H_{T}");
//  
//  //TPaveText* htBox[nHTRegions];
//  TPaveText* htBox[5];
//  for( int iHT = 0; iHT < nHTRegions; ++iHT){
//    
//    //htBox[iHT] = new TPaveText(0.16+0.2*iHT, 0.9-0.06, 0.34+0.2*iHT, 0.9, "brNDC");
//    if (iHT==0) htBox[iHT] = new TPaveText(0.16+0.15*iHT, 0.9-0.06, 0.34+0.15*iHT, 0.85, "brNDC");
//    else htBox[iHT] = new TPaveText(0.16+0.13*iHT, 0.9-0.06, 0.34+0.13*iHT, 0.85, "brNDC");
//    htBox[iHT]->AddText( htRegions[iHT].c_str() );
//    
//    htBox[iHT]->SetBorderSize(0);
//    htBox[iHT]->SetFillColor(kWhite);
//    htBox[iHT]->SetTextSize(0.038);
//    htBox[iHT]->SetTextAlign(21); // align centered
//    htBox[iHT]->SetTextFont(62);
//    htBox[iHT]->Draw("same");
//
//  }
  
  TString htRegion = "";
  if (region == "J") htRegion = "1 jet";
  if (region == "VL") htRegion = "HT [200, 450] GeV";
  if (region == "L") htRegion = "HT [450, 575] GeV";
  if (region == "M") htRegion = "HT [575, 1000] GeV";
  if (region == "H") htRegion = "HT [1000, 1500] GeV";
  if (region == "UH") htRegion = "HT > 1500 GeV";
  TPaveText* htBox = new TPaveText(0.4, 0.83, 0.6, 0.85, "brNDC");
  htBox->AddText(htRegion);
  htBox->SetBorderSize(0);
  htBox->SetFillColor(kWhite);
  htBox->SetTextSize(0.038);
  htBox->SetTextAlign(21); // align centered
  htBox->SetTextFont(62);
  htBox->Draw("same");


  gPad->RedrawAxis();
  
  c2->cd();
  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.21);
  pad2->SetTopMargin(0.05);
  pad2->SetBottomMargin(0.1);
  pad2->SetRightMargin(0.05);
  pad2->SetLeftMargin(0.15);
  pad2->Draw();
  pad2->cd();

  h_Pull->SetStats(0);
  h_Pull->SetMarkerStyle(20);
  h_Pull->SetMarkerSize(1.0);
  h_Pull->SetLineColor(kRed);
  h_Pull->SetMarkerColor(kRed);
  h_Pull->GetXaxis()->SetLabelSize(0.00);
  h_Pull->GetXaxis()->SetTickLength(0.09);
  h_Pull->GetYaxis()->SetNdivisions(5,5,0);
  h_Pull->GetYaxis()->SetRangeUser(-2,2);
  h_Pull->GetYaxis()->SetTitleSize(0.17);
  h_Pull->GetYaxis()->SetTitleOffset(0.18);
  h_Pull->GetYaxis()->SetLabelSize(0.17);

  //TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, 44, 10, -2.0, 2.0 );
//  TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, 44, 10, -3.0, 3.0 );
  TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, NBins, 10, 0, 2.0 );
  //TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, 44, 10, -10.0, 10.0 );
  h2_axes_ratio->SetStats(0);
  h2_axes_ratio->GetXaxis()->SetLabelSize(0.00);
  h2_axes_ratio->GetXaxis()->SetTickLength(0.09);
  h2_axes_ratio->GetYaxis()->SetNdivisions(5,5,0);
  h2_axes_ratio->GetYaxis()->SetTitleSize(0.18);
  h2_axes_ratio->GetYaxis()->SetTitleOffset(0.26);
  h2_axes_ratio->GetYaxis()->SetLabelSize(0.17);
  //h2_axes_ratio->GetYaxis()->SetTitle("Pull");
  h2_axes_ratio->GetYaxis()->SetTitle("Bin / Std.");

  //TLine* LineCentral = new TLine(0, 0., 44, 0.);
  TLine* LineCentral = new TLine(0, 1., NBins, 1.);
  LineCentral->SetLineColor(1);

  h2_axes_ratio->Draw("");
  LineCentral->Draw("same");
  h_RatioMC->SetMarkerSize(0);
  h_RatioMC->SetFillColor (kGray+2);
  h_RatioMC->SetFillStyle (3244);
  h_RatioMC->Draw("E2,same");
  
  h_Pull->Draw("pe,same");


//  TLine* lHT_b[6];
//  for( int iHT=1; iHT < 6; iHT++ ){
//    //lHT_b[iHT-1] = new TLine(11*iHT,-2.0, 11*iHT, 2.0 );
////    lHT_b[iHT-1] = new TLine(6*iHT,-3.0, 6*iHT, 3.0 );
//    //lHT_b[iHT-1] = new TLine(11*iHT,0.0, 11*iHT, 5.0 );
//    //lHT_b[iHT-1] = new TLine(11*iHT,-10.0, 11*iHT, 10.0 );
//    lHT_b[iHT-1] = new TLine(13+11*(iHT-1), -3.0, 13+11*(iHT-1), 3.0 );
//
//    lHT_b[iHT-1]->SetLineColor(kBlack);
//    lHT_b[iHT-1]->SetLineStyle(3);
//    lHT_b[iHT-1]->SetLineWidth(2);
//
//    lHT_b[iHT-1]->Draw("same");
//  }


  gPad->RedrawAxis();

  c2->cd();
  if (withMC)  c2->SaveAs( Form("%sWithMC.pdf", savename.Data()) );
  else c2->SaveAs( Form("%s.pdf", savename.Data()) );
}
