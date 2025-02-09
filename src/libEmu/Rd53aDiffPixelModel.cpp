/*
 * Author: N. Whallon <alokin@uw.edu>
 * Date: 2017-XI
 * Description: a class to handle rd53a diff pixel models
 */

#include "Rd53aDiffPixelModel.h"

#include "Gauss.h"

using namespace Gauss;

Rd53aDiffPixelModel::Rd53aDiffPixelModel(float _VthDiff_mean, float _VthDiff_sigma, float _noise_sigma_mean, float _noise_sigma_sigma)
{
	VthDiff_mean = _VthDiff_mean;
	VthDiff_sigma = _VthDiff_sigma;
	VthDiff_gauss = rand_normal(VthDiff_mean, VthDiff_sigma, 0);

	noise_sigma_mean = _noise_sigma_mean;
	noise_sigma_sigma = _noise_sigma_sigma;
	noise_sigma_gauss = rand_normal(noise_sigma_mean, noise_sigma_sigma, 0);

}
Rd53aDiffPixelModel::Rd53aDiffPixelModel(float _VthDiff_mean, float _VthDiff_sigma, float _VthDiff_gauss, float _noise_sigma_mean, float _noise_sigma_sigma, float _noise_sigma_gauss)
{
	VthDiff_mean = _VthDiff_mean;
	VthDiff_sigma = _VthDiff_sigma;
	VthDiff_gauss = _VthDiff_gauss;

	noise_sigma_mean = _noise_sigma_mean;
	noise_sigma_sigma = _noise_sigma_sigma;
	noise_sigma_gauss = _noise_sigma_gauss;
}
Rd53aDiffPixelModel::~Rd53aDiffPixelModel() = default;

// functions for modeling pixel responses
float Rd53aDiffPixelModel::calculateThreshold(uint32_t Vth1Diff, uint32_t Vth2Diff, int TDAC) const
{
  float modelVth = VthDiff_gauss/100. * (Vth1Diff - Vth2Diff);
  float modelTDAC = TDAC * 15;
  float threshold = modelVth + modelTDAC;
  
  if (threshold < 0) threshold = 0;

  return threshold;
}

float Rd53aDiffPixelModel::calculateNoise() const
{
	return rand_normal(0, noise_sigma_gauss, 1);
}

uint32_t Rd53aDiffPixelModel::calculateToT(float charge)
{
  return uint32_t(4.56053+4.04173*charge/10000.);
}
