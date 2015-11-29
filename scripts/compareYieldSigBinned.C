void compareYieldSigBinned( TString region = "ALL"){
  
  //Once you have two histograms, with one bin per topological region (grouped 11 by 11, one block per HT region)
  //Here, hestimate is MC and hdata is data-driven estimate. hestimate color is 401, for QCD MC. hdata color is kBlack.
  //In the bottom pad, we decided to replace the ratio with the pull distribution. h_Pull is the corresponding histogram.
  //hPull is filled with a SetBinContent (iBin, (estimate-MC)/sqrt(sigma(estimate)^2+sigma(MC)^2)), and a SetBinError(iBin, 1.0)
  
  int colors[5] = {4,3,6,2};

  //TString sample = "T1bbbb_900_825";
  //TString sample = "T1bbbb_900_850";
  TString sample = "T1bbbb_900_875";
  //TString sample = "T1bbbb_1400_900";
  //TString sample = "T1bbbb_1600_1";

  float ratio_ymin = 0.5; float ratio_ymax = 1.5;
  //  float ratio_ymin = 0.0; float ratio_ymax = 2.0;
  
  //  TString savename = "comp_btagsf_srBinned"+region+"_"+sample;
  TString savename = "comp_jec_srBinned"+region+"_"+sample;

  TFile* f1 = 0;
  f1 = new TFile("yield_histograms_SigBinned"+region+"_"+sample+".root", "READ");
    
  TH1D* hestimate = (TH1D*)f1->Get("h_sr_yields");
  TH1D* h_stat_err = (TH1D*)f1->Get("h_stat_err");
  std::vector<std::string> vars;
  std::vector<TH1D*> hists_vars;
  std::vector<TH1D*> ratios_vars;
  // vars.push_back("btagsf_UP");
  // vars.push_back("btagsf_DN");
  vars.push_back("jec_UP");
  vars.push_back("jec_DN");

  for (unsigned int i=0; i<vars.size(); ++i) {
    TH1D* h = (TH1D*) f1->Get(Form("h_sr_yields_%s",vars.at(i).c_str()));
    TH1D* h_ratio = (TH1D*) f1->Get(Form("h_ratio_%s",vars.at(i).c_str()));
    hists_vars.push_back(h);
    ratios_vars.push_back(h_ratio);
  }

  TCanvas* c2 = new TCanvas("c2", "", 1500, 600);
  c2->cd();
  
  TPad *pad1 = new TPad("pad1","pad1",0,0.35,1,1);
  pad1->SetBottomMargin(0.15);
  pad1->SetRightMargin(0.02);
  pad1->SetLeftMargin(0.05);
    gStyle->SetOptStat(0);
    
  pad1->Draw();
  pad1->cd();

  pad1->SetLogy();
  
  float yMax_1 = hestimate->GetMaximum()*1.5;
  float yMax_2 = 1.2*(hestimate->GetMaximum() + hestimate->GetBinError(hestimate->GetMaximumBin()));
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
  hestimate->GetYaxis()->SetRangeUser(yMin, yMax);
  hestimate->GetYaxis()->SetTitleOffset(0.4);
  hestimate->GetYaxis()->SetTitleSize(0.06);
  hestimate->GetYaxis()->SetLabelSize(0.06);
  hestimate->GetYaxis()->SetTitle("Entries");
  hestimate->GetXaxis()->LabelsOption("v");
  hestimate->GetXaxis()->SetLabelSize(0.04);
  hestimate->SetTitle("");
  hestimate->Draw("PE");

  TLegend* legend = new TLegend( 0.7, 0.9-(1+1)*0.06-0.06, 0.9, 0.9-0.06 );
  legend->SetBorderSize(0);
  legend->SetTextSize(0.038);
  legend->SetTextFont(42);
  legend->SetFillColor(0);
  legend->AddEntry( hestimate, "Nominal", "PL" );

  for (unsigned int i=0; i<vars.size(); ++i) {
    TH1D* h = hists_vars.at(i);
    h->SetMarkerColor(colors[i]);
    h->SetLineColor(colors[i]);
    h->Draw("PE same");
    legend->AddEntry( h, vars.at(i).c_str(), "PL");
  }
 
  legend->Draw("same");

  //TPaveText* labelTop = MT2DrawTools::getLabelTopSimulation(lumi);
  //labelTop->Draw("same");
  
  //TPaveText* labelTop = new TPaveText(.72,.91,.95,.95, "NDC");
  TPaveText* labelTop = new TPaveText(.7,.91,.95,.95, "NDC");
  labelTop->AddText("CMS Preliminary, 2.1 fb^{-1} at #sqrt{s} = 13 TeV");
  //labelTop->AddText("CMS Data, 0.579 fb^{-1} at #sqrt{s} = 13 TeV");
  labelTop->SetBorderSize(0);
  labelTop->SetFillColor(kWhite);
  labelTop->SetTextSize(0.038);
  labelTop->Draw("same");
  
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
  if (region == "VL") htRegion = "200 < HT < 450";
  if (region == "L") htRegion = "450 < HT < 575";
  if (region == "M") htRegion = "575 < HT < 1000";
  if (region == "H") htRegion = "1000 < HT < 1500";
  if (region == "UH") htRegion = "HT > 1500";
  if (region == "ALL") htRegion = "HT > 200";
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
  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.35);
  pad2->SetTopMargin(0.05);
  pad2->SetBottomMargin(0.1);
  pad2->SetRightMargin(0.02);
  pad2->SetLeftMargin(0.05);
  pad2->SetGridy();
  pad2->Draw();
  pad2->cd();

  TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, NBins, 10, ratio_ymin, ratio_ymax );
  //TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, 44, 10, -2.0, 2.0 );
//  TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, 44, 10, -3.0, 3.0 );
//  TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, NBins, 10, 0.0, 2.0 );
  //TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, 44, 10, -10.0, 10.0 );
  h2_axes_ratio->SetStats(0);
  h2_axes_ratio->GetXaxis()->SetLabelSize(0.00);
  h2_axes_ratio->GetXaxis()->SetTickLength(0.09);
  h2_axes_ratio->GetYaxis()->SetNdivisions(5,5,0);
  h2_axes_ratio->GetYaxis()->SetTitleSize(0.10);
  h2_axes_ratio->GetYaxis()->SetTitleOffset(0.2);
  h2_axes_ratio->GetYaxis()->SetLabelSize(0.1);
  //h2_axes_ratio->GetYaxis()->SetTitle("Pull");
  h2_axes_ratio->GetYaxis()->SetTitle("Var / Nom");

  //TLine* LineCentral = new TLine(0, 0., 44, 0.);
  TLine* LineCentral = new TLine(0, 1., NBins, 1.);
  LineCentral->SetLineColor(1);

  h2_axes_ratio->Draw("");
  LineCentral->Draw("same");
  
  h_stat_err->SetMarkerSize(0);
  h_stat_err->SetFillColor(kGray+2);
  h_stat_err->SetFillStyle(3244);
  h_stat_err->Draw("E2,same");

  for (unsigned int i=0; i<vars.size(); ++i) {
    TH1D* h_ratio = ratios_vars.at(i);
    h_ratio->SetMarkerColor(colors[i]);
    h_ratio->SetLineColor(colors[i]);
    h_ratio->SetStats(0);
    h_ratio->SetMarkerStyle(20);
    h_ratio->SetMarkerSize(1.2);
    h_ratio->Draw("hist p same");
  }

  

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
  c2->SaveAs( Form("%s.eps", savename.Data()) );
  c2->SaveAs( Form("%s.pdf", savename.Data()) );
}
