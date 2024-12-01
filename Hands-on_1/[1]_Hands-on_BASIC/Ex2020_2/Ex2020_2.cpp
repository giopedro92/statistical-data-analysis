#include "RooRealVar.h"
#include "RooAbsArg.h"
#include "RooAbsReal.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"
#include "RooAddPdf.h"
#include "RooMinimizer.h"
#include "RooFitResult.h"
#include "RooHistFunc.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "RooFormulaVar.h"
#include "RooDataHist.h"
#include "RooGenericPdf.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "TStyle.h"
#include <iostream>

using namespace RooFit;

void Ex2020_2() {
  // S e t u p   m o d e l
  // ---------------------

  // Declare variables x,mean,sigma with associated name, title, initial value and allowed range

  RooRealVar x{"x", "x", -10, 10};

  RooRealVar tau{"tau", "Mean life", 3};

  RooFormulaVar rate{"rate", "Rate", "-1 / @0", RooArgList(tau)};

  // Build gaussian pdf in terms of x, mean and sigma

  RooExponential model{"model", "Exponential shape", x, rate};

  // Construct plot frame in 'x'

  auto frame = x.frame();

  // P l o t   m o d e l   a n d   c h a n g e   p a r a m e t e r   v a l u e s
  // ---------------------------------------------------------------------------

  // Plot gauss in frame (i.e. in x)

  model.plotOn(frame);

  // Change the value of sigma to 3

  // Plot gauss in frame (i.e. in x) and draw frame on canvas

  // G e n e r a t e   e v e n t s
  // -----------------------------

  // Generate a dataset of 1000 events in x from gauss

  x.setBins(200);

  RooDataHist *data = model.generateBinned(x, 1000);

  // Make a second plot frame in x and draw both the
  // data and the pdf in the frame

  // F i t   m o d e l   t o   d a t a
  // -----------------------------

  // Fit pdf to data

  model.fitTo(*data);

  // Print values of mean and sigma (that now reflect fitted values and errors)

  rate.Print();

  // Draw all frames on a canvas

  auto frame2 = x.frame();
  data->plotOn(frame2);
  model.plotOn(frame2);

  TCanvas *c = new TCanvas("c", "Exponential shape", 2700, 1500);
  c->Divide(2);

  c->cd(1);
  gPad->SetLeftMargin(0.15);
  frame->Draw();

  c->cd(2);
  gPad->SetLeftMargin(0.15);
  frame2->Draw();

  c->Print("Exponential.png");
}