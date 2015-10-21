{
  
  //Once you have two histograms, with one bin per topological region (grouped 11 by 11, one block per HT region)
  //Here, hestimate is MC and hdata is data-driven estimate. hestimate color is 401, for QCD MC. hdata color is kBlack.
  //In the bottom pad, we decided to replace the ratio with the pull distribution. h_Pull is the corresponding histogram.
  //hPull is filled with a SetBinContent (iBin, (estimate-MC)/sqrt(sigma(estimate)^2+sigma(MC)^2)), and a SetBinError(iBin, 1.0)
  
  bool doSR = false;
  bool doInvertDPhi = false;
  bool doMT2SideBand = false;
  bool doDPhiMT2 = false;

  doSR = true;
  //doInvertDPhi = true;
  //doMT2SideBand = true;
  //doDPhiMT2 = true;

  std::string savename = "default";
  if(doSR) savename = "comp_sr";
  if(doInvertDPhi) savename = "comp_InvertDPhi";
  if(doMT2SideBand) savename = "comp_MT2SideBand";
  if(doDPhiMT2) savename = "comp_DPhiMT2";

  TFile* f1 = 0;
  if(doSR) f1 = new TFile("yield_histograms_sr.root", "READ");
  else if(doInvertDPhi) f1 = new TFile("yield_histograms_InvertDPhi.root", "READ");
  else if(doMT2SideBand) f1 = new TFile("yield_histograms_MT2SideBand.root", "READ");
  else if(doDPhiMT2) f1 = new TFile("yield_histograms_DPhiMT2.root", "READ");
  TH1F* hestimate = (TH1F*)f1->Get("yields_MC");
  TH1F* hdata = (TH1F*)f1->Get("yields_RS");
  TH1F* h_Pull = (TH1F*)f1->Get("h_pull");
  //TH1F* h_Pull = (TH1F*)f1->Get("h_ratio");

  TCanvas* c2 = new TCanvas("c2", "", 1200, 600);
  c2->cd();
  
  TPad *pad1 = new TPad("pad1","pad1",0,0.3-0.1,1,1);
  pad1->SetBottomMargin(0.15);
  pad1->SetRightMargin(0.05);
  pad1->SetLeftMargin(0.15);
  pad1->Draw();
  pad1->cd();

  //pad1->SetLogy();
  
  float yMax_1 = hdata->GetMaximum()*1.5;
  float yMax_2 = 1.2*(hdata->GetMaximum() + hdata->GetBinError(hestimate->GetMaximumBin()));
  float yMax1 = (yMax_1>yMax_2) ? yMax_1 : yMax_2;
  float yMax_3 = hestimate->GetMaximum()*1.5;
  float yMax_4 = 1.2*(hestimate->GetMaximum() + hestimate->GetBinError(hestimate->GetMaximumBin()));
  float yMax2 = (yMax_3>yMax_4) ? yMax_3 : yMax_4;
  float yMax = (yMax1>yMax2) ? yMax1 : yMax2;
  
  //float yMin = 1e-1;
  //float yMin = 1e-3;
  //float yMin = 1e-5;
  yMax*=2.;
  
  hestimate->GetXaxis()->SetRangeUser(0, 4*11);
  hestimate->GetYaxis()->SetRangeUser(yMin, yMax);
  hestimate->GetYaxis()->SetTitleOffset(1.15);
  hestimate->GetYaxis()->SetTitleSize(0.052);
  hestimate->GetYaxis()->SetLabelSize(0.04);
  hestimate->GetYaxis()->SetTitle("Entries");
  hestimate->GetXaxis()->LabelsOption("v");
  hestimate->SetTitle("");
  hestimate->Draw("PE");

  hdata->SetMarkerStyle(20);
  hdata->SetMarkerSize(1.6);
  hdata->SetLineColor( kBlack );
  hdata->SetMarkerColor( kBlack );

  hestimate->SetMarkerStyle(20);
  hestimate->SetMarkerSize(1.6);
  hestimate->SetLineColor( 401 );
  hestimate->SetMarkerColor( 401 );

  hdata->Draw("PE,same");

  TLegend* legend = new TLegend( 0.8, 0.9-(1+1)*0.06-0.06, 0.9, 0.9-0.06 );
  legend->SetBorderSize(0);
  legend->SetTextSize(0.038);
  legend->SetTextFont(42);
  legend->SetFillColor(0);
  //legend->AddEntry( hdata, "RS Pred", "PL" );
  legend->AddEntry( hdata, "data-driven", "PL" );
  legend->AddEntry( hestimate, "QCD MC", "PL" );
  //legend->AddEntry( hestimate, "Data", "PL" );

  legend->Draw("same");

  //TPaveText* labelTop = MT2DrawTools::getLabelTopSimulation(lumi);
  //labelTop->Draw("same");
  
  //TPaveText* labelTop = new TPaveText(.72,.91,.95,.95, "NDC");
  TPaveText* labelTop = new TPaveText(.7,.91,.95,.95, "NDC");
  labelTop->AddText("CMS Simulation, 1.0 fb^{-1} at #sqrt{s} = 13 TeV");
  //labelTop->AddText("CMS Data, 0.579 fb^{-1} at #sqrt{s} = 13 TeV");
  labelTop->SetBorderSize(0);
  labelTop->SetFillColor(kWhite);
  labelTop->SetTextSize(0.038);
  labelTop->Draw("same");
  
  TLine* lHT[3];
  for( int iHT=1; iHT < 4; iHT++ ){
    lHT[iHT-1] = new TLine(11*iHT, 0.0, 11*iHT, yMax );
    lHT[iHT-1]->SetLineColor(kBlack);
    lHT[iHT-1]->SetLineStyle(3);
    lHT[iHT-1]->SetLineWidth(2);

    lHT[iHT-1]->Draw("same");
  }

  int nHTRegions = 4;
  std::vector< std::string > htRegions;
  htRegions.push_back("low H_{T}");
  htRegions.push_back("medium H_{T}");
  htRegions.push_back("high H_{T}");
  htRegions.push_back("extreme H_{T}");
  
  //TPaveText* htBox[nHTRegions];
  TPaveText* htBox[4];
  for( int iHT = 0; iHT < nHTRegions; ++iHT){
    
    //htBox[iHT] = new TPaveText(0.16+0.2*iHT, 0.9-0.06, 0.34+0.2*iHT, 0.9, "brNDC");
    htBox[iHT] = new TPaveText(0.16+0.2*iHT, 0.9-0.06, 0.34+0.2*iHT, 0.85, "brNDC");
    htBox[iHT]->AddText( htRegions[iHT].c_str() );
    
    htBox[iHT]->SetBorderSize(0);
    htBox[iHT]->SetFillColor(kWhite);
    htBox[iHT]->SetTextSize(0.038);
    htBox[iHT]->SetTextAlign(21); // align centered
    htBox[iHT]->SetTextFont(62);
    htBox[iHT]->Draw("same");

  }

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
  h_Pull->SetMarkerSize(1.6);
  h_Pull->SetLineColor(1);
  h_Pull->GetXaxis()->SetLabelSize(0.00);
  h_Pull->GetXaxis()->SetTickLength(0.09);
  h_Pull->GetYaxis()->SetNdivisions(5,5,0);
  h_Pull->GetYaxis()->SetRangeUser(-2,2);
  h_Pull->GetYaxis()->SetTitleSize(0.17);
  h_Pull->GetYaxis()->SetTitleOffset(0.4);
  h_Pull->GetYaxis()->SetLabelSize(0.17);

  //TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, 44, 10, -2.0, 2.0 );
  TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, 44, 10, -3.0, 3.0 );
  //TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, 44, 10, 0, 5.0 );
  //TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 10, 0, 44, 10, -10.0, 10.0 );
  h2_axes_ratio->SetStats(0);
  h2_axes_ratio->GetXaxis()->SetLabelSize(0.00);
  h2_axes_ratio->GetXaxis()->SetTickLength(0.09);
  h2_axes_ratio->GetYaxis()->SetNdivisions(5,5,0);
  h2_axes_ratio->GetYaxis()->SetTitleSize(0.20);
  h2_axes_ratio->GetYaxis()->SetTitleOffset(0.3);
  h2_axes_ratio->GetYaxis()->SetLabelSize(0.17);
  h2_axes_ratio->GetYaxis()->SetTitle("Pull");
  //h2_axes_ratio->GetYaxis()->SetTitle("MC/Pred");

  TLine* LineCentral = new TLine(0, 0., 44, 0.);
  //TLine* LineCentral = new TLine(0, 1., 44, 1.);
  LineCentral->SetLineColor(1);

  h2_axes_ratio->Draw("");
  LineCentral->Draw("same");
  h_Pull->Draw("pe,same");

  TLine* lHT_b[3];
  for( int iHT=1; iHT < 4; iHT++ ){
    //lHT_b[iHT-1] = new TLine(11*iHT,-2.0, 11*iHT, 2.0 );
    lHT_b[iHT-1] = new TLine(11*iHT,-3.0, 11*iHT, 3.0 );
    //lHT_b[iHT-1] = new TLine(11*iHT,0.0, 11*iHT, 5.0 );
    //lHT_b[iHT-1] = new TLine(11*iHT,-10.0, 11*iHT, 10.0 );
    lHT_b[iHT-1]->SetLineColor(kBlack);
    lHT_b[iHT-1]->SetLineStyle(3);
    lHT_b[iHT-1]->SetLineWidth(2);

    lHT_b[iHT-1]->Draw("same");
  }


  gPad->RedrawAxis();

  c2->cd();
  c2->SaveAs( Form("%s.pdf", savename.c_str()) );
  
}
