#include <iostream>
#include <complex>
#include "TStyle.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TLine.h"

int converge(double x, double y, unsigned int maxIterations)
{
  // Check analytically if it is within main cardoid or the first bulb
  double q=(x-0.25)*(x-0.25)+y*y;
  if (q*(q+(x-0.25))<0.25*y*y) return maxIterations; // main cardoid
  if ((x+1)*(x+1)+y*y<0.0625) return maxIterations; // first bulb

  std::complex<double> c(x, y), z(x, y), ztemp;
  unsigned int i=0; 
  while (std::abs(z)<2 && i<maxIterations)
  {
    ztemp=z;
    z=z*z+c;
    if (z==ztemp) return maxIterations; // this will not diverge, so return maxIteration
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
  double xmin=-2.0, xmax=0.6, ymin=-1.3, ymax=1.3;
  // Seahorse valley
  // double xmin=-1.0, xmax=-0.5, ymin=0, ymax=ymin+(xmax-xmin);
  double granularity=1000;
  double maxIterations=1000;
  
  gStyle->SetPalette(1);
  Int_t color=gStyle->GetColorPalette(0);
  
  // TCanvas *c=new TCanvas("c", "c", granularity, granularity);
  // c->Range(xmin, ymin, xmax, ymax);
  TImage *img=new TImage(granularity, granularity);
  img->SetEditable();
  
  unsigned int i=0;
  unsigned int total=granularity*granularity;
  for (double x=xmin; x<=xmax; x+=(xmax-xmin)/granularity)
  {
    for (double y=ymin; y<=ymax; y+=(ymax-ymin)/granularity)
    {
      int iterations=converge(x, y, maxIterations);
      // TLine *point=new TLine(x, y, x, y);
      // point->SetLineColor(color+254.*returnColor(iterations, maxIterations));
      // point->Draw("same");
      if (iterations==maxIterations) img->PutPixel(x, y);
      ++i;
      if (i%(total/10)==0) std::cout<<"Completed "<<i*100./total<<"% "<<std::endl;
    }
  }
  img->Draw();
  
  std::cout<<"Writing to file"<<std::endl;
  img->WriteImage("MandelbrotSet.png");
  std::cout<<"Written"<<std::endl;
  
  // c->SaveAs("MandelbrotSet.png");
}
