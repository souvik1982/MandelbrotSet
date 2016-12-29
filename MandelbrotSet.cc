#include <iostream>
#include <complex>
#include "TStyle.h"
#include "TCanvas.h"
#include "TLine.h"

int converge(std::complex<double> c, unsigned int maxIterations)
{
  std::complex<double> z=c;
  unsigned int i=0; 
  while (std::abs(z)<2 && i<maxIterations)
  {
    z=z*z+c;
    ++i;
  }
  if (std::abs(z)>=2) return i; // return iteration if divergent
  else return maxIterations; // return maxIteration if convergent
}

double returnColor(unsigned int i, unsigned int max)
{
  double f=double(i)/double(max);
  double c=(exp(0.001*f)-1)/(exp(0.001)-1);
  return c;
}

void MandelbrotSet()
{
  // double xmin=-2, xmax=2, ymin=-2, ymax=2;
  // Seahorse valley
  double xmin=-1.0, xmax=-0.5, ymin=0, ymax=ymin+(xmax-xmin);
  double granularity=1000;
  double maxIterations=1000;
  
  gStyle->SetPalette(1);
  Int_t color=gStyle->GetColorPalette(0);
  
  TCanvas *c=new TCanvas("c", "c", granularity, granularity);
  c->Range(xmin, ymin, xmax, ymax);
  
  unsigned int i=0;
  unsigned int total=granularity*granularity;
  for (double x=xmin; x<=xmax; x+=(xmax-xmin)/granularity)
  {
    for (double y=ymin; y<=ymax; y+=(ymax-ymin)/granularity)
    {
      std::complex<double> c0(x, y);
      int iterations=converge(c0, maxIterations);
      TLine *point=new TLine(x, y, x, y);
      point->SetLineColor(color+254.*returnColor(iterations, maxIterations));
      point->Draw("same");
      ++i;
      if (i%(total/10)==0) std::cout<<"Completed "<<i*100./total<<"% "<<std::endl;
    }
  }
  
  c->SaveAs("MandelbrotSet.png");
}
