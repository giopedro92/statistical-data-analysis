#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit;

void Ex2020_1() {
  // S e t u p   m o d e l
  // ---------------------

  // Declare variables x,mean,sigma with associated name, title, initial value and allowed range

  RooRealVar x{"x", "energy", -20, 20};

  RooRealVar m{"m", "mean", 14, 11, 17};
  RooRealVar s{"s", "width", 1, 0.1, 1};
  RooRealVar a{"a", "alpha", 1.5, 0, 20};
  RooRealVar n{"n", "n", 0.5, 0.1, 1};

  // Build gaussian pdf in terms of x,mean and sigma

  RooCBShape model{"model", "CB shape", x, m, s, a, n};

  s.setVal(0.3);

  // Construct plot frame in 'x'

  auto frame = x.frame();

  // P l o t   m o d e l   a n d   c h a n g e   p a r a m e t e r   v a l u e s
  // ---------------------------------------------------------------------------

  // Plot gauss in frame (i.e. in x)

  model.plotOn(frame);

  // Change the value of sigma to 3

  s.setVal(0.3);

  // Plot gauss in frame (i.e. in x) and draw frame on canvas

  model.plotOn(frame);

  frame->Draw();

  // G e n e r a t e   e v e n t s
  // -----------------------------

  // Generate a dataset of 1000 events in x from gauss

  auto data = model.generate(x, 10000); // RooDataSet*

  // Make a second plot frame in x and draw both the
  // data and the pdf in the frame

  // F i t   m o d e l   t o   d a t a
  // -----------------------------

  // Fit pdf to data

  model.fitTo(*data);

  // Print values of mean and sigma (that now reflect fitted values and errors)

  m.Print();
  s.Print();
  a.Print();
  n.Print();

  // Draw all frames on a canvas

  new TCanvas;
  auto frame2 = x.frame();
  data->plotOn(frame2);
  model.plotOn(frame2);
  frame2->Draw();
}