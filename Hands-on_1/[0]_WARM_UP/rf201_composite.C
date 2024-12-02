// Addition and convolution: composite pdf with signal and background component

// pdf = f_bkg * bkg(x,a0,a1) + (1-f_bkg) * (f_sig1 * sig1(x,m,s1) + (1-f_sig1) * sig2(x,m,s2)))

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
 
void rf201_composite()
{
   // S e t u p   c o m p o n e n t   p d f s
   // ---------------------------------------
 
   // Declare observable x
   RooRealVar x{"x", "x", 0, 10};
 
   // Create two Gaussian PDFs g1(x,mean,sigma1) anf g2(x,mean,sigma2) and their parameters for signal
   RooRealVar mean("mean", "mean of gaussians", 5);
   RooRealVar sigma1("sigma1", "width of gaussians", 0.5);
   RooRealVar sigma2("sigma2", "width of gaussians", 1);
 
   RooGaussian sig1("sig1", "Signal component 1", x, mean, sigma1);
   RooGaussian sig2("sig2", "Signal component 2", x, mean, sigma2);
 
   // Build Chebychev polynomial pdf for background
   RooRealVar a0("a0", "a0", 0.5, 0., 1.);
   RooRealVar a1("a1", "a1", 0.2, 0., 1.);
   RooChebychev bkg("bkg", "Background", x, RooArgSet(a0, a1));

   // PLOT
   TCanvas *c = new TCanvas("c", "Signal and Background", 2700, 1600);
    c->SetCanvasSize(2690, 1500);

   auto frame0 = x.frame(Title("Signal and Background"));

   sig1.plotOn(frame0, LineColor(kBlue));
   sig2.plotOn(frame0, LineColor(kRed));
   bkg.plotOn(frame0, LineColor(kGreen));


   // ---------------------------------------------
   // M E T H O D   1 - T w o   R o o A d d P d f s
   // =============================================
 
   // A d d   s i g n a l   c o m p o n e n t s
   // ------------------------------------------
 
   // Sum the signal components into a composite signal pdf
   RooRealVar sig1frac("sig1frac", "fraction of component 1 in signal", 0.8, 0., 1.); // f_sig1
   RooAddPdf sig("sig", "Signal", RooArgList(sig1, sig2), sig1frac);

    sig.plotOn(frame0, LineColor(43));

   frame0->Draw();
   gPad->BuildLegend(0.6,0.7, 0.9,0.9);

   c->Print("Signal and bkg.png");

   // A d d  s i g n a l   a n d   b a c k g r o u n d
   // ------------------------------------------------
 
   // Sum the composite signal and background
   RooRealVar bkgfrac("bkgfrac", "fraction of background", 0.5, 0., 1.);
   RooAddPdf model("model", "g1+g2+a", RooArgList(bkg, sig), bkgfrac);
 
   // S a m p l e ,   f i t   a n d   p l o t   m o d e l
   // ---------------------------------------------------
 
   // Generate a data sample of 1000 events in x from model
   std::unique_ptr<RooDataSet> data{model.generate(x, 1000)};
 
   // Fit model to data
   model.fitTo(*data, PrintLevel(-1));


   TCanvas *c1 = new TCanvas("c1", "Method 1", 2700, 1600);
    c1->SetCanvasSize(2690, 1500);

   auto frame1 = x.frame(Title("Example of composite pdf=(sig1+sig2)+bkg Method 1"));

   // Plot data and PDF overlaid
   data->plotOn(frame1);
   model.plotOn(frame1);
 
   // Overlay the background component of model with a dashed line
   model.plotOn(frame1, Components(bkg), LineStyle(kDashed));
 
   // Overlay the background+sig2 components of model with a dotted line
   model.plotOn(frame1, Components(RooArgSet(bkg, sig2)), LineStyle(kDotted));
 
   frame1->Draw();
   gPad->BuildLegend(0.55,0.7, 0.9,0.9);

   c->Print("Signal and bkg.png");

   // Print structure of composite pdf
   model.Print("t");


   // ---------------------------------------------------------------------------------------------
   // M E T H O D   2 - O n e   R o o A d d P d f   w i t h   r e c u r s i v e   f r a c t i o n s
   // =============================================================================================
 
   // Construct sum of models on one go using recursive fraction interpretations
   //
   //   model2 = bkg + (sig1 + sig2)
   //
   RooAddPdf model2("model", "g1+g2+a", RooArgList(bkg, sig1, sig2), RooArgList(bkgfrac, sig1frac), true);
 
   // NB: Each coefficient is interpreted as the fraction of the
   // left-hand component of the i-th recursive sum, i.e.
   //
   //   sum4 = A + ( B + ( C + D))  with fraction fA, fB and fC expands to
   //
   //   sum4 = fA*A + (1-fA)*(fB*B + (1-fB)*(fC*C + (1-fC)*D))
 
   // P l o t   r e c u r s i v e   a d d i t i o n   m o d e l
   // ---------------------------------------------------------
   TCanvas *c2 = new TCanvas("c2", "Method 2", 2700, 1600);
    c2->SetCanvasSize(2690, 1500);

   
   auto xframe = x.frame(Title("Example of composite pdf=(sig1+sig2)+bkg Method 2"));
   
   model2.plotOn(xframe, LineColor(kRed), LineStyle(kDashed));
   model2.plotOn(xframe, Components(RooArgSet(bkg, sig2)), LineColor(kRed), LineStyle(kDashed));

   model2.Print("t");
 
   // Draw the frame on the canvas
   gPad->SetLeftMargin(0.15);
   xframe->GetYaxis()->SetTitleOffset(1.4);
   xframe->Draw();

   gPad->BuildLegend(0.6,0.7, 0.9,0.9);

   c2->Print("rf201_composite.png");

    std::cout << "\n" << endl;
   model2.Print("t");
}