/*
 * Author: N. Whallon <alokin@uw.edu>
 * Date: 2017-II
 * Description: a class to handle pixel models
 */

#ifndef PIXELMODEL
#define PIXELMODEL

#include <cstdint>

class PixelModel {
	public:
		PixelModel(float _Vthin_mean, float _Vthin_sigma, float _TDACVbp_mean, float _TDACVbp_sigma, float _noise_sigma_mean, float _noise_sigma_sigma);
		PixelModel(float _Vthin_mean, float _Vthin_sigma, float _Vthin_gauss, float _TDACVbp_mean, float _TDACVbp_sigma, float _TDACVbp_gauss, float _noise_sigma_mean, float _noise_sigma_sigma, float _noise_sigma_gauss);
		~PixelModel();

		float Vthin_mean;
		float Vthin_sigma;
		float Vthin_gauss;
		float TDACVbp_mean;
		float TDACVbp_sigma;
		float TDACVbp_gauss;
		float noise_sigma_mean;
		float noise_sigma_sigma;
		float noise_sigma_gauss;

		// functions for modeling pixel responses
		float calculateThreshold(uint32_t Vthin_fine, uint32_t Vthin_coarse, uint32_t TDAC) const;
		float calculateNoise() const;
		uint32_t calculateToT(float charge);

	private:
};

#endif
