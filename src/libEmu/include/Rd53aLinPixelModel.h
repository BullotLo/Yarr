/*
 * Author: N. Whallon <alokin@uw.edu>
 * Date: 2017-X
 * Description: a class to handle rd53a lin pixel models
 */

#ifndef RD53ALINPIXELMODEL
#define RD53ALINPIXELMODEL

#include <cstdint>

class Rd53aLinPixelModel {
	public:
		Rd53aLinPixelModel(float _VthresholdLin_mean, float _VthresholdLin_sigma, float _noise_sigma_mean, float _noise_sigma_sigma);
		Rd53aLinPixelModel(float _VthresholdLin_mean, float _VthresholdLin_sigma, float _VthresholdLin_gauss, float _noise_sigma_mean, float _noise_sigma_sigma, float _noise_sigma_gauss);
		~Rd53aLinPixelModel();

		float VthresholdLin_mean;
		float VthresholdLin_sigma;
		float VthresholdLin_gauss;
		float noise_sigma_mean;
		float noise_sigma_sigma;
		float noise_sigma_gauss;

		// functions for modeling pixel responses
  float calculateThreshold(uint32_t VthresholdLin, int TDAC) const;
		float calculateNoise() const;
		uint32_t calculateToT(float charge);

	private:
};

#endif
