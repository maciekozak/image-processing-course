#include <algorithm>
#include <cmath>
#include <iostream>
#include <chrono>

#include <queue>   // added for task 3
#include <complex> // added for task 4


#include "CLI11.hpp"
#include "CImg.h"

using namespace cimg_library;


namespace dmimg {
	inline void error(const std::string& s)
	{
		throw std::runtime_error(s);
	}
	using namespace cimg_library;
	// get image width
	template <class T>
	inline int width(const CImg<T>& img) {
		return img.width();
	}
	// get image height
	template <class T>
	inline int height(const CImg<T>& img) {
		return img.height();
	}
	// obtaining R, G & B values from given pixel
	template <class T>
	inline T get_r(const CImg<T>& img, int x, int y) {
		return img(x, y);
	}
	template <class T>
	inline T get_g(const CImg<T>& img, int x, int y) {
		return img(x, y, 0, 1);
	}
	template <class T>
	inline T get_b(const CImg<T>& img, int x, int y) {
		return img(x, y, 2);
	}
	// setting R, G & B values to given pixel
	template <class T>
	inline void set_r(CImg<T>& img, int x, int y, int v) {
		img(x, y) = v;
	}
	template <class T>
	inline void set_g(CImg<T>& img, int x, int y, int v) {
		img(x, y, 0, 1) = v;
	}
	template <class T>
	inline void set_b(CImg<T>& img, int x, int y, int v) {
		img(x, y, 2) = v;
	}
	// setting R, G & B values to given pixel but within the range of 0 to 255
	// if it exceeds clip it to 0 or 255
	template <class T>
	inline void set_r_safe(CImg<T>& img, int x, int y, int v) {
		if (v > 255) {
			img(x, y) = 255;
			return;
		}
		if (v < 0) {
			img(x, y) = 0;
			return;
		}
		img(x, y) = v;
	}
	template <class T>
	inline void set_g_safe(CImg<T>& img, int x, int y, int v) {
		if (v > 255) {
			img(x, y, 0, 1) = 255;
			return;
		}
		if (v < 0) {
			img(x, y, 0, 1) = 0;
			return;
		}
		img(x, y, 0, 1) = v;
	}
	template <class T>
	inline void set_b_safe(CImg<T>& img, int x, int y, int v) {
		if (v > 255) {
			img(x, y, 2) = 255;
			return;
		}
		if (v < 0) {
			img(x, y, 2) = 0;
			return;
		}
		img(x, y, 2) = v;
	}
	// set R, G & B to a certain value
	template <class T>
	inline void set_rgb(CImg<T>& img, int x, int y, int v) {
		img(x, y) = v;
		img(x, y, 0, 1) = v;
		img(x, y, 2) = v;
	}
	// set R, G & B to a certain value and make sure that final vale will be between 0 and 255
	template <class T>
	inline void set_rgb_safe(CImg<T>& img, int x, int y, int v) {
		if (v > 255) {
			img(x, y) = 255;
			img(x, y, 0, 1) = 255;
			img(x, y, 2) = 255;
			return;
		}
		if (v < 0) {
			img(x, y) = 0;
			img(x, y, 0, 1) = 0;
			img(x, y, 2) = 0;
			return;
		}
		img(x, y) = v;
		img(x, y, 0, 1) = v;
		img(x, y, 2) = v;
	}
	//// set R, G & B to a certain values of R, G, B
	template <class T>
	void set_rgb_color(CImg<T>& img, int x, int y, int value_r, int value_g, int value_b) {
		img(x, y) = value_r;
		img(x, y, 0, 1) = value_g;
		img(x, y, 2) = value_b;
	}
	// copy a 3x3 array values to another 3x3 array
	inline void copy_2d_3x3_array(int source[3][3], int destination[3][3]) {
		for (int x = 0; x < 3; x++) {
			for (int y = 0; y < 3; y++) {
				destination[y][x] = source[y][x];
			}
		}
	}

	// TASK 1
	// B
	// modify image brightness by the given value
	template <class T>
	void brightness(CImg<T>& img, int v) {
		// our new mapping because
		// r, g, b vary only from 0 to 255
		// so we can calculate it once
		std::vector<int> new_values = {};
		for (int i = 0; i < 256; i++) {
			int new_value = i + v;
			if (new_value < 0) {
				new_values.push_back(0);
				continue;
			}
			else if (new_value > 255) {
				new_values.push_back(255);
				continue;
			}
			new_values.push_back(new_value);
		}
		// now change brightness based on pre-calculated new values
		for (int x = 0; x < dmimg::width(img); x++) {
			for (int y = 0; y < dmimg::height(img); y++) {
				dmimg::set_r(img, x, y, new_values[dmimg::get_r(img, x, y)]);
				dmimg::set_g(img, x, y, new_values[dmimg::get_g(img, x, y)]);
				dmimg::set_b(img, x, y, new_values[dmimg::get_b(img, x, y)]);
			}
		}
	}
	// contrast adjustment using adjustment of the linear fuction 
	template <class T>
	void contrast(CImg<T>& img, int v) {
		// image correction factor
		float f = (259.0 * (v + 255.0)) / (255.0 * (259.0 - v));
		// our new mapping because
		// r, g, b vary only from 0 to 255
		// so we can calculate it once
		std::vector<int> new_values = {};
		for (int i = 0; i < 256; i++) {
			int new_value = static_cast<int>(f * (i - 128) + 128);
			if (new_value < 0) {
				new_values.push_back(0);
				continue;
			}
			if (new_value > 255) {
				new_values.push_back(255);
				continue;
			}
			new_values.push_back(new_value);
		}
		// now change contrast based on pre-calculated new values
		for (int x = 0; x < dmimg::width(img); x++) {
			for (int y = 0; y < dmimg::height(img); y++) {
				dmimg::set_r(img, x, y, new_values[dmimg::get_r(img, x, y)]);
				dmimg::set_g(img, x, y, new_values[dmimg::get_g(img, x, y)]);
				dmimg::set_b(img, x, y, new_values[dmimg::get_b(img, x, y)]);
			}
		}
	}
	// image negative
	template <class T>
	void negative(CImg<T>& img) {
		// assuming that we work on unsigned char's we simply need to
		// perform new_pixel = 255 - old_pixel
		for (int x = 0; x < dmimg::width(img); x++) {
			for (int y = 0; y < dmimg::height(img); y++) {
				dmimg::set_r(img, x, y, 255 - dmimg::get_r(img, x, y));
				dmimg::set_g(img, x, y, 255 - dmimg::get_g(img, x, y));
				dmimg::set_b(img, x, y, 255 - dmimg::get_b(img, x, y));
			}
		}
	}
	//---------------------------------------------------------
	// 	GEOMETRIC OPERATIONS //add swap instead of this solution
	template <class T>
	void hflip(CImg<T>& input) {

		CImg<T> output = input;
		for (int x = 0; x < input.width(); ++x) {
			for (int y = 0; y < input.height(); ++y) {
				//dmimg::set_rgb_color(output, x, y, dmimg::get_r(input, input.width() - 1 - x, y), dmimg::get_g(input, input.width() - 1 - x, y), dmimg::get_b(input, input.width() - 1 - x, y));
				dmimg::set_r(output, x, y, dmimg::get_r(input, input.width() - 1 - x, y));
				dmimg::set_g(output, x, y, dmimg::get_g(input, input.width() - 1 - x, y));
				dmimg::set_b(output, x, y, dmimg::get_b(input, input.width() - 1 - x, y));
			}
		}
		input = output;
	}

	template <class T>
	void vflip(CImg<T>& input) {
		CImg<T> output = input;

		for (int x = 0; x < input.width(); ++x) {
			for (int y = 0; y < input.height(); ++y) {
				//dmimg::set_rgb_color(output, x, input.height() - 1 - y, dmimg::get_r(input, x, y), dmimg::get_g(input, x, y), y, dmimg::get_b(input, x, y));
				dmimg::set_r(output, x, input.height() - 1 - y, dmimg::get_r(input, x, y));
				dmimg::set_g(output, x, input.height() - 1 - y, dmimg::get_g(input, x, y));
				dmimg::set_b(output, x, input.height() - 1 - y, dmimg::get_b(input, x, y));
			}
		}
		input = output;
	}

	template <class T>
	void dflip(CImg<T>& input) {
		CImg<T> output = input;
		for (int x = 0; x < input.width(); ++x) {
			for (int y = 0; y < input.height(); ++y) {
				//dmimg::set_rgb_color(output, x, input.height() - 1 - y, dmimg::get_r(input, input.width() - 1 - x, y), dmimg::get_g(input, input.width() - 1 - x, y), dmimg::get_b(input, input.width() - 1 - x, y));
				dmimg::set_r(output, x, input.width() - 1 - y, dmimg::get_r(input, input.width() - 1 - x, y));
				dmimg::set_g(output, x, input.width() - 1 - y, dmimg::get_g(input, input.width() - 1 - x, y));
				dmimg::set_b(output, x, input.width() - 1 - y, dmimg::get_b(input, input.width() - 1 - x, y));
			}
		}
		input = output;
	}

	template <class T>
	void shrink(CImg<T>& input) {
		CImg<T> output(input.width() / 2, input.height() / 2, 1, 3);

		for (int x = 0; x < input.width(); ++x) {
			for (int y = 0; y < input.height(); ++y) {
				dmimg::set_rgb_color(output, (x / 2), (y / 2), dmimg::get_r(input, x, y), dmimg::get_g(input, x, y), dmimg::get_b(input, x, y));
			}
		}
		input = output;
	}

	template <class T>
	void enlarge(CImg<T>& input) {
		CImg<T> output(input.width() * 2, input.height() * 2, 1, 3);

		for (int x = 0; x < input.width(); ++x) {
			for (int y = 0; y < input.height(); ++y) {
				dmimg::set_rgb_color(output, 2 * x, 2 * y, dmimg::get_r(input, x, y), dmimg::get_g(input, x, y), dmimg::get_b(input, x, y));
				dmimg::set_rgb_color(output, (2 * x) + 1, 2 * y, dmimg::get_r(input, x, y), dmimg::get_g(input, x, y), dmimg::get_b(input, x, y));
				dmimg::set_rgb_color(output, 2 * x, (2 * y) + 1, dmimg::get_r(input, x, y), dmimg::get_g(input, x, y), dmimg::get_b(input, x, y));
				dmimg::set_rgb_color(output, (2 * x) + 1, (2 * y) + 1, dmimg::get_r(input, x, y), dmimg::get_g(input, x, y), dmimg::get_b(input, x, y));
			}
		}
		input = output;
	}
	//---------------------------------------------------------
	// TASK 1
	// N 4
	// midpoint filter
	template <class T>
	void mid(CImg<T>& img) {
		// a copy is needed because as the loops progress
		// the original img is being changed and we need to
		// feed the algorithm with unaltered data
		CImg<T> img_cpy = img;
		int reds[9] = {};
		int greens[9] = {};
		int blues[9] = {};
		// loops for going over all the pixels EXCEPT the very border
		for (int x = 1; x < dmimg::width(img) - 1; x++) {
			for (int y = 1; y < dmimg::height(img) - 1; y++) {
				// finding the biggest and lowest value in the neighbourhood
				// used to count the current index in arrays
				unsigned char counter = 0;
				for (int x2 = -1; x2 <= 1; x2++) {
					for (int y2 = -1; y2 <= 1; y2++) {
						reds[counter] = dmimg::get_r(img_cpy, x + x2, y + y2);
						greens[counter] = dmimg::get_g(img_cpy, x + x2, y + y2);
						blues[counter] = dmimg::get_b(img_cpy, x + x2, y + y2);
						counter++;
					}
					//counter++;
				}
				// sort the arrays
				std::sort(reds, reds + 9);
				std::sort(greens, greens + 9);
				std::sort(blues, blues + 9);
				// set new value
				dmimg::set_r(img, x, y, (reds[0] + reds[8]) / 2);
				dmimg::set_g(img, x, y, (greens[0] + greens[8]) / 2);
				dmimg::set_b(img, x, y, (blues[0] + blues[8]) / 2);
				// and now we are going to take a look at the next pixel
			}
		}
	}
	// arithmetic mean filter
	template <class T>
	void amean(CImg<T>& img) {
		// a copy is needed because as the loops progress
		// the original img is being changed and we need to
		// feed the algorithm with unaltered data
		CImg<T> img_cpy = img;
		// loops for going over all the pixels EXCEPT the very border
		for (int x = 1; x < dmimg::width(img) - 1; x++) {
			for (int y = 1; y < dmimg::height(img) - 1; y++) {
				// summing up all values in the neighbourhood
				// INCLUDING the pixel in the middle
				int sum_r = 0;
				int sum_g = 0;
				int sum_b = 0;
				for (int x2 = -1; x2 <= 1; x2++) {
					for (int y2 = -1; y2 <= 1; y2++) {
						sum_r += dmimg::get_r(img_cpy, x + x2, y + y2);
						sum_g += dmimg::get_g(img_cpy, x + x2, y + y2);
						sum_b += dmimg::get_b(img_cpy, x + x2, y + y2);
					}
				}
				// compute the avarage
				sum_r /= 9;
				sum_g /= 9;
				sum_b /= 9;
				// set new value
				dmimg::set_r(img, x, y, sum_r);
				dmimg::set_g(img, x, y, sum_g);
				dmimg::set_b(img, x, y, sum_b);
				// and now we are going to take a look at the next pixel
			}
		}
	}
	//---------------------------------------------------------
	// TASK 1 E
	// MSE, PMSE, SNR, PSNR, MD
	// helper function
	template <class T> // returns => (F1(x,y)-F2(x,y))^2
	double f1minusf2(CImg<T>& img1, CImg<T>& img2) {
		// check if the operation is even possible
		if (!((img1.width() == img2.width()) && (img1.height() == img2.height()))) {
			dmimg::error("Upss...sorry but the sizes of the images are not equal.");
		}

		double result = 0;
		long double sum_red = 0, sum_green = 0, sum_blue = 0;

		for (int x = 0; x < img1.width(); ++x) {
			for (int y = 0; y < img1.height(); ++y) {
				double diff_red = dmimg::get_r(img1, x, y) - dmimg::get_r(img2, x, y);
				double diff_green = dmimg::get_g(img1, x, y) - dmimg::get_g(img2, x, y);
				double diff_blue = dmimg::get_b(img1, x, y) - dmimg::get_b(img2, x, y);
				sum_red += diff_red * diff_red;
				sum_green += diff_green * diff_green;
				sum_blue += diff_blue * diff_blue;
			}
		}
		result = sum_red + sum_green + sum_blue;
		return result / 3.0;
	}
	// Mean square error
	template <class T>
	double mse(CImg<T>& img1, CImg<T>& img2) {
		double sizes = static_cast<double>(img1.width())* static_cast<double>(img1.height());
		return (dmimg::f1minusf2(img1, img2) / sizes);
	}
	// Peak mean square error
	template <class T>
	double pmse(CImg<T>& img1, CImg<T>& img2, const double max_value = 255) {
		const double mse_value = dmimg::mse(img1, img2);
		return (mse_value != 0) ? (mse_value / (max_value * max_value)) : (0);
	}

	// Signal to noise ratio [dB]:
	// img1 is P_signal, img2 is P_noise
	template <class T>
	double snr(CImg<T>& img1, CImg<T>& img2) {
		const double f1minusf2 = dmimg::f1minusf2(img1, img2);
		if (f1minusf2 == 0) dmimg::error("Signal to noise ratio: division by 0, the compared images are probably the same.");

		long double signal_value = 0;
		double diff_red, diff_green, diff_blue;
		long double sum_red = 0, sum_green = 0, sum_blue = 0;
		for (int x = 0; x < img1.width(); ++x) {
			for (int y = 0; y < img1.height(); ++y) {
				diff_red = dmimg::get_r(img1, x, y);
				diff_green = dmimg::get_g(img1, x, y);
				diff_blue = dmimg::get_b(img1, x, y);
				sum_red += diff_red * diff_red;
				sum_green += diff_green * diff_green;
				sum_blue += diff_blue * diff_blue;
			}
		}
		signal_value = (sum_red + sum_green + sum_blue) / 3.0;
		return (f1minusf2 != 0) ? static_cast<double>(10.0 * std::log10((signal_value) / f1minusf2)) : (0);
	}

	// Peak signal to noise ratio [dB]
	template <class T>
	double psnr(CImg<T>& img1, CImg<T>& img2, const double max_value = 255) {
		const double f1minusf2 = dmimg::f1minusf2(img1, img2);
		if (f1minusf2 == 0) dmimg::error("Peak signal to noise ratio: division by 0, the compared images are probably the same.");
		return (f1minusf2 != 0) ? static_cast<double>(10 * std::log10((max_value * max_value) / f1minusf2)) : (0);
	}
	// Maximum difference
	template <class T>
	void md(CImg<T>& img1, CImg<T>& img2) {
		double resultR = 0;
		double resultG = 0;
		double resultB = 0;
		double diff_red, diff_green, diff_blue;
		for (int x = 0; x < img1.width(); ++x) {
			for (int y = 0; y < img1.height(); ++y) {
				diff_red = dmimg::get_r(img1, x, y) - dmimg::get_r(img2, x, y);
				diff_green = dmimg::get_g(img1, x, y) - dmimg::get_g(img2, x, y);
				diff_blue = dmimg::get_b(img1, x, y) - dmimg::get_b(img2, x, y);
				// absolute value of the difference
				diff_red = std::abs(diff_red);
				diff_green = std::abs(diff_green);
				diff_blue = std::abs(diff_blue);
				// finding if the current differece is bigger than previous ones
				if (resultR < diff_red) resultR = diff_red;
				if (resultG < diff_green) resultG = diff_green;
				if (resultB < diff_blue) resultB = diff_blue;
			}
		}
		std::cout << "Maximum difference for each channel:" << std::endl;
		std::cout << "R " << resultR << " G " << resultG << " B " << resultB << std::endl;
	}
	// ######################################################################
	// TASK 2
	// ASSIGNED VARIANT: H4 C5 S1 O5

	// Generate histogram of img of specified channel z: 0 - red, 1 - green, 2 -blue, if gray scale img use 0
	template <class T>
	void histogram(CImg<T>& img, int z) {
		double sum;
		CImg<unsigned char> image(img);
		int height = image.height();
		int width = image.width();
		CImg<unsigned char>histogram(256, 256, 1, 3); //256x256 pixels, 2D, 3 channels
		int occurence[256] = {};

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				occurence[image(x, y, z)]++;                //there are 0-255 cell indexes
															//increment value in one cell that correspods to an amount of pixels present on an image
			}
		}

		bool graph;
		for (int x = 0; x < 256; x++)
		{
			graph = false;
			for (int y = 0; y < 256; y++)
			{
				for (int s = 0; s < image.spectrum(); s++)
				{
					if (s == z)     //if value of chosen RGB channel is equal to current channel from spectrum
					{
						if (y == (int)((occurence[x]) / 30) || graph == true) //y value (0-255) is equal to occurence[x]/30
						{
							histogram(x, 255 - y, s) = 0;       //assign "black color" to an image histogram(x,255-y,channel)

							if (graph == false) graph = true;
						}
						else histogram(x, 255 - y, s) = 255; //255-y because of horizontal mirror flip
					}
					else histogram(x, 255 - y, s) = 0; //assign black not our channel;
				}
			}
		}
		/*
		CImgDisplay main_disp(histogram);
		while (!main_disp.is_closed())
		{
			main_disp.wait();
		}*/
		img = histogram; // .save("histogram.bmp");
	}

	// H4 TASK 2 
	// Power 2/3 final probability density function
	template <class T>
	void hpower(CImg<T>& img, int minBrightness, int maxBrightness) {
		double width = img.width();
		double height = img.height();
		double num_of_pixels = width * height;
		double new_min = static_cast<double>(minBrightness);
		double new_max = static_cast<double>(maxBrightness);

		unsigned int histogram[256] = {};
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				histogram[dmimg::get_r(img, x, y)]++;
			}
		}

		unsigned int sum[256] = {};
		for (int all_values = 0; all_values < 256; all_values++) {
			for (int m = 0; m <= all_values; m++) {
				sum[all_values] += histogram[m];
			}
		}

		new_min = pow(new_min, 0.33333);
		new_max = pow(new_max, 0.33333);

		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int new_value = pow(new_min + (new_max - new_min) * (1.0 / num_of_pixels) * static_cast<double>(sum[dmimg::get_r(img, x, y)]), 3.0);

				dmimg::set_r_safe(img, x, y, new_value);
				dmimg::set_g_safe(img, x, y, new_value);
				dmimg::set_b_safe(img, x, y, new_value);
			}
		}

	}

	// C5 TASK 2
	// Variation coefficient II 
	template <class T>
	void cvarcoii(CImg<T>& img) {
		int height = img.height();
		int width = img.width();
		long double p_sum = 0;
		short int channels = img.spectrum();

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				for (int channel = 0; channel < channels; channel++) {
					p_sum = p_sum + (std::pow((int)img(x, y, channel), 2));
				}
			}
		}
		std::cout << "Variation coefficient II is equal to " << ((p_sum) / (std::pow(height, 2) * std::pow(width, 2))) << std::endl;
	}

	// S1 TASK 2
	// apply convolution mask filter
	// it takes an two dimensional array which represents the mask
	// int divider is used to divide each pixel by a value after applying a filter
	// int divider defaults to 1 - leaving the result unchanged
	template <class T>
	void conv_mask(CImg<T>& img, const int mask[3][3], int divider = 1) {
		// a copy is needed because as the loops progress
		// the original img is being changed and we need to
		// feed the algorithm with unaltered data
		CImg<T> img_cpy = img;
		// loops for going over all the pixels EXCEPT the very border
		for (int x = 1; x < dmimg::width(img) - 1; x++) {
			for (int y = 1; y < dmimg::height(img) - 1; y++) {
				// variables for storing convolution of neighbourhood of pixel and the mask
				int result_r = 0;
				int result_g = 0;
				int result_b = 0;
				for (int x2 = -1; x2 <= 1; x2++) {
					for (int y2 = -1; y2 <= 1; y2++) {
						// we need to add one to coordinates to get proper
						// mask index
						// remember that coordinates are [y][x]
						// and counted from the upper left corner of the mask
						result_r += (mask[y2 + 1][x2 + 1] * dmimg::get_r(img_cpy, x + x2, y + y2));
						result_g += (mask[y2 + 1][x2 + 1] * dmimg::get_g(img_cpy, x + x2, y + y2));
						result_b += (mask[y2 + 1][x2 + 1] * dmimg::get_b(img_cpy, x + x2, y + y2));
					}
				}
				// set new value but within 0 to 255
				dmimg::set_r_safe(img, x, y, result_r / divider);
				dmimg::set_g_safe(img, x, y, result_g / divider);
				dmimg::set_b_safe(img, x, y, result_b / divider);
				// and now we are going to take a look at the next pixel
			}
		}
	}

	// S1 low-pass filter - optimized variant 2
	template <class T>
	void slowpass_optimized(CImg<T>& img) {
		//	variant_2 = {  
		//	{1, 1, 1}
		//	{1, 2, 1}
		//	{1, 1, 1}
		//
		// copy is needed for we want to have original data and modify original on the fly
		CImg<T> img_cpy = img;
		// loops for going over all the pixels EXCEPT the very border
		for (int x = 1; x < dmimg::width(img) - 1; x++) {
			for (int y = 1; y < dmimg::height(img) - 1; y++) {
				int sum_r = 0;
				int sum_g = 0;
				int sum_b = 0;
				// first row
				sum_r += dmimg::get_r(img_cpy, x - 1, y - 1);
				sum_g += dmimg::get_g(img_cpy, x - 1, y - 1);
				sum_b += dmimg::get_b(img_cpy, x - 1, y - 1);
				sum_r += dmimg::get_r(img_cpy, x, y - 1);
				sum_g += dmimg::get_g(img_cpy, x, y - 1);
				sum_b += dmimg::get_b(img_cpy, x, y - 1);
				sum_r += dmimg::get_r(img_cpy, x + 1, y - 1);
				sum_g += dmimg::get_g(img_cpy, x + 1, y - 1);
				sum_b += dmimg::get_b(img_cpy, x + 1, y - 1);
				// second (middle) row
				sum_r += dmimg::get_r(img_cpy, x - 1, y);
				sum_g += dmimg::get_g(img_cpy, x - 1, y);
				sum_b += dmimg::get_b(img_cpy, x - 1, y);
				sum_r += (dmimg::get_r(img_cpy, x, y) * 2);
				sum_g += (dmimg::get_g(img_cpy, x, y) * 2);
				sum_b += (dmimg::get_b(img_cpy, x, y) * 2);
				sum_r += dmimg::get_r(img_cpy, x + 1, y);
				sum_g += dmimg::get_g(img_cpy, x + 1, y);
				sum_b += dmimg::get_b(img_cpy, x + 1, y);
				// third row
				sum_r += dmimg::get_r(img_cpy, x - 1, y + 1);
				sum_g += dmimg::get_g(img_cpy, x - 1, y + 1);
				sum_b += dmimg::get_b(img_cpy, x - 1, y + 1);
				sum_r += dmimg::get_r(img_cpy, x, y + 1);
				sum_g += dmimg::get_g(img_cpy, x, y + 1);
				sum_b += dmimg::get_b(img_cpy, x, y + 1);
				sum_r += dmimg::get_r(img_cpy, x + 1, y + 1);
				sum_g += dmimg::get_g(img_cpy, x + 1, y + 1);
				sum_b += dmimg::get_b(img_cpy, x + 1, y + 1);
				// compute the avarage
				sum_r /= 10;
				sum_g /= 10;
				sum_b /= 10;
				// set new value
				dmimg::set_r(img, x, y, sum_r);
				dmimg::set_g(img, x, y, sum_g);
				dmimg::set_b(img, x, y, sum_b);
				// and now we are going to take a look at the next pixel
			}
		}
	}
	// TASK 2 (O5) Rosenfeld operator (--orosenfeld)
	// p is a parameter which can only take the values of 1, 2, 4, 8, 16, ...
	template <class T>
	void orosenfeld(CImg<T>& img, int p = 1) {
		// check if p is correct
		// if it not a power of 2 or it is lower or equal to zero give error
		// allow ewentually for p = 1
		if (!(std::ceil(log2(p)) == std::floor(log2(p))) or p <= 0) {
			if (p != 1) dmimg::error("Rosenfeld operator: p can only take the values of 1, 2, 4, 8, 16, ...");
		}
		// a copy is needed because as the loops progress
		// the original img is being changed and we need to
		// feed the algorithm with unaltered data
		CImg<T> img_cpy = img;
		// loops for going over all the pixels EXCEPT the very border
		for (int x = p; x < dmimg::width(img) - p; x++) {
			// we sample only on the x axis so we have full range of y's
			for (int y = 0; y < dmimg::height(img); y++) {
				int result_r = 0;
				int result_g = 0;
				int result_b = 0;
				// first part of the formula
				// add to the sum pixels from the right side of the pixel and the pixel itself
				for (int x2 = p; x2 > 0; x2--) {
					result_r += dmimg::get_r(img_cpy, x + x2 - 1, y);
					// std::cerr << " + " << static_cast<int>(dmimg::get_r(img_cpy, x + x2 - 1, y));
					result_g += dmimg::get_g(img_cpy, x + x2 - 1, y);
					result_b += dmimg::get_b(img_cpy, x + x2 - 1, y);
				}
				// subtract from the sum the pixel pixels from the left
				for (int x2 = p; x2 > 0; x2--) {
					result_r -= dmimg::get_r(img_cpy, x - x2, y);
					// std::cerr << " - " << static_cast<int>(dmimg::get_r(img_cpy, x - x2, y));
					result_g -= dmimg::get_g(img_cpy, x - x2, y);
					result_b -= dmimg::get_b(img_cpy, x - x2, y);
				}

				// set new value (divided by p) and within 0 to 255
				// std::cerr << " result: " << (result_r / p);
				dmimg::set_r_safe(img, x, y, result_r / p);
				dmimg::set_g_safe(img, x, y, result_g / p);
				dmimg::set_b_safe(img, x, y, result_b / p);
				// and now we are going to take a look at the next pixel
			}
		}
	}

	// ######################################################################
	// TASK 3
	// ASSIGNED VARIANT: M5
	// We assume that black means the foreground element
	// black = 1
	// white = 0
	const int FG = 0;
	const int BG = 255;
	// gray
	const int GR = 127;

	// structural elements will have this structure:
	// the origin of coordinates will have x=0 and y=0
	// the following elements will have coordinates based on this point
	struct xyval {
		int x;
		int y;
		int value;
	};

	xyval new_xyval(int my_x, int my_y, int my_value) {
		xyval temp;
		temp.x = my_x;
		temp.y = my_y;
		temp.value = my_value;
		return temp;
	}

	// function returns structural element of given number
	std::vector<xyval> get_structural_element(int argument) {
		// first entry should be the origin of coordinates!!! (0,0, something)
		std::vector<xyval> str_el_1 = { new_xyval(0,0,FG), new_xyval(1,0,FG) };
		std::vector<xyval> str_el_2 = { new_xyval(0,0,FG), new_xyval(0,1,FG) };
		std::vector<xyval> str_el_3 = { new_xyval(0,0,FG), new_xyval(0,-1,FG), new_xyval(1,-1,FG),
										new_xyval(-1,0,FG), new_xyval(-1,-1,FG), new_xyval(1,0,FG),
										new_xyval(-1,1,FG), new_xyval(0,1,FG), new_xyval(1,1,FG),
		};
		std::vector<xyval> str_el_4 = { new_xyval(0,0,FG), new_xyval(0,-1,FG), new_xyval(-1,0,FG),
									new_xyval(1,0,FG), new_xyval(0,1,FG) };
		std::vector<xyval> str_el_5 = { new_xyval(0,0,FG), new_xyval(1,0,FG), new_xyval(0,1,FG) };
		std::vector<xyval> str_el_6 = { new_xyval(0,0,BG), new_xyval(1,0,FG), new_xyval(0,1,FG) };
		std::vector<xyval> str_el_7 = { new_xyval(0,0,FG), new_xyval(-1,0,FG), new_xyval(1,0,FG) };
		std::vector<xyval> str_el_8 = { new_xyval(0,0,BG), new_xyval(-1,0,FG), new_xyval(1,0,FG) };
		std::vector<xyval> str_el_9 = { new_xyval(0,0,FG), new_xyval(-1,0,FG), new_xyval(-1,1,FG) };
		std::vector<xyval> str_el_10 = { new_xyval(0,0,FG), new_xyval(0,-1,FG), new_xyval(1,-1,FG) };
		// xi <11,14>
		std::vector<xyval> str_el_11 = { new_xyval(0,0,BG),
										new_xyval(-1,-1,FG), new_xyval(-1,0,FG), new_xyval(-1,1,FG), // left column
										new_xyval(0,-1,GR), new_xyval(0,1,GR),	// middle column
										new_xyval(1,-1,GR), new_xyval(1,0,GR), new_xyval(1,1,GR) // right column
		};
		std::vector<xyval> str_el_12 = { new_xyval(0,0,BG),
										new_xyval(-1,-1,FG), new_xyval(-1,0,GR), new_xyval(-1,1,GR),
										new_xyval(0,-1,FG), new_xyval(0,1,GR),
										new_xyval(1,-1,FG), new_xyval(1,0,GR), new_xyval(1,1,GR)
		};
		std::vector<xyval> str_el_13 = { new_xyval(0,0,BG),
										new_xyval(-1,-1,GR), new_xyval(-1,0,GR), new_xyval(-1,1,GR),
										new_xyval(0,-1,GR), new_xyval(0,1,GR),
										new_xyval(1,-1,FG), new_xyval(1,0,FG), new_xyval(1,1,FG)
		};
		std::vector<xyval> str_el_14 = { new_xyval(0,0,BG),
										new_xyval(-1,-1,GR), new_xyval(-1,0,GR), new_xyval(-1,1,FG),
										new_xyval(0,-1,GR), new_xyval(0,1,FG),
										new_xyval(1,-1,GR), new_xyval(1,0,GR), new_xyval(1,1,FG)
		};
		// xii <15, 22>
		std::vector<xyval> str_el_15 = { new_xyval(0,0,FG),
										new_xyval(-1,-1,BG), new_xyval(-1,0,GR), new_xyval(-1,1,FG), // left column
										new_xyval(0,-1,BG), new_xyval(0,1,FG),	// middle column
										new_xyval(1,-1,BG), new_xyval(1,0,GR), new_xyval(1,1,FG) // right column
		};
		std::vector<xyval> str_el_16 = { new_xyval(0,0,FG),
										new_xyval(-1,-1,GR), new_xyval(-1,0,FG), new_xyval(-1,1,FG),
										new_xyval(0,-1,BG), new_xyval(0,1,FG),
										new_xyval(1,-1,BG), new_xyval(1,0,BG), new_xyval(1,1,GR)
		};
		std::vector<xyval> str_el_17 = { new_xyval(0,0,FG),
										new_xyval(-1,-1,FG), new_xyval(-1,0,FG), new_xyval(-1,1,FG),
										new_xyval(0,-1,GR), new_xyval(0,1,GR),
										new_xyval(1,-1,BG), new_xyval(1,0,BG), new_xyval(1,1,BG)
		};
		std::vector<xyval> str_el_18 = { new_xyval(0,0,FG),
										new_xyval(-1,-1,FG), new_xyval(-1,0,FG), new_xyval(-1,1,GR),
										new_xyval(0,-1,FG), new_xyval(0,1,BG),
										new_xyval(1,-1,GR), new_xyval(1,0,BG), new_xyval(1,1,BG)
		};

		std::vector<xyval> str_el_19 = { new_xyval(0,0,FG),
										new_xyval(-1,-1,FG), new_xyval(-1,0,GR), new_xyval(-1,1,BG),
										new_xyval(0,-1,FG), new_xyval(0,1,BG),
										new_xyval(1,-1,FG), new_xyval(1,0,GR), new_xyval(1,1,BG)
		};
		std::vector<xyval> str_el_20 = { new_xyval(0,0,FG),
										new_xyval(-1,-1,GR), new_xyval(-1,0,BG), new_xyval(-1,1,BG),
										new_xyval(0,-1,FG), new_xyval(0,1,BG),
										new_xyval(1,-1,FG), new_xyval(1,0,FG), new_xyval(1,1,GR)
		};
		std::vector<xyval> str_el_21 = { new_xyval(0,0,FG),
										new_xyval(-1,-1,BG), new_xyval(-1,0,BG), new_xyval(-1,1,BG),
										new_xyval(0,-1,GR), new_xyval(0,1,GR),
										new_xyval(1,-1,FG), new_xyval(1,0,FG), new_xyval(1,1,FG)
		};
		std::vector<xyval> str_el_22 = { new_xyval(0,0,FG),
										new_xyval(-1,-1,BG), new_xyval(-1,0,BG), new_xyval(-1,1,GR),
										new_xyval(0,-1,BG), new_xyval(0,1,FG),
										new_xyval(1,-1,GR), new_xyval(1,0,FG), new_xyval(1,1,FG)
		};

		std::vector<std::vector<xyval>> structural_elements = { str_el_1, str_el_2, str_el_3, str_el_4, str_el_5, str_el_6, str_el_7, str_el_8, str_el_9, str_el_10,
																str_el_11, str_el_12, str_el_13, str_el_14, str_el_15, str_el_16, str_el_17, str_el_18, str_el_19, str_el_20,
																str_el_21, str_el_22
		};
		// number of structural elemet starts from 1
		if (1 > argument or argument > structural_elements.size()) dmimg::error("wrong argument!");
		// - 1 because in vector we have indexes from 0
		return structural_elements[argument - 1];
	}

	// assume that we get b&w image as input
	template <class T>
	void erosion(CImg<T>& img, std::vector<xyval> structural_el) {
		CImg<T> img_cpy = img;
		int structural_el_size = structural_el.size();
		// we assume that each structural element is no more than a grid of 3x3
		// loops for going over all the pixels EXCEPT the very border
		for (int x = 1; x < dmimg::width(img) - 1; x++) {
			for (int y = 1; y < dmimg::height(img) - 1; y++) {
				int checks = 0;
				// check if from this pixel structural element is contained nearby
				for (int i = 0; i < structural_el_size; i++) {
					if (int(dmimg::get_r(img_cpy, x + structural_el[i].x, y + structural_el[i].y)) == structural_el[i].value) {
						checks++;
					}
				}
				if (checks == structural_el_size) {
					dmimg::set_rgb(img, x, y, FG);
				}
				else {
					dmimg::set_rgb(img, x, y, BG);
				}
			}
		}	// and now we are going to take a look at the next pixel
	}
	// assume that we get b&w image as input
	template <class T>
	void dilation(CImg<T>& img, std::vector<xyval> structural_el) {
		CImg<T> img_cpy = img;
		int structural_el_size = structural_el.size();
		// we assume that each structural element is no more than a grid of 3x3
		// loops for going over all the pixels EXCEPT the very border
		for (int x = 1; x < dmimg::width(img) - 1; x++) {
			for (int y = 1; y < dmimg::height(img) - 1; y++) {
				// check if this pixel is a foreground pixel, if so apply the structuring element
				if (dmimg::get_r(img_cpy, x, y) == FG) {
					for (int i = 0; i < structural_el_size; i++) {
						dmimg::set_rgb(img, x + structural_el[i].x, y + structural_el[i].y, FG);
					}
				}
			}	// and now we are going to take a look at the next pixel
		}
	}
	// NON-optimized version of opening
	template <class T>
	void opening_slow(CImg<T>& img, std::vector<xyval> structural_el) {
		dmimg::erosion(img, structural_el);
		dmimg::dilation(img, structural_el);
	}
	// optimized version of opening
	template <class T>
	void opening(CImg<T>& img, std::vector<xyval> structural_el) {
		CImg<T> img_cpy = img;
		img.fill(BG);
		int structural_el_size = structural_el.size();
		// we assume that each structural element is no more than a grid of 3x3
		// loops for going over all the pixels EXCEPT the very border
		for (int x = 1; x < dmimg::width(img) - 1; x++) {
			for (int y = 1; y < dmimg::height(img) - 1; y++) {
				int checks = 0;
				// check if from this pixel structural element is contained nearby
				for (int i = 0; i < structural_el_size; i++) {
					if (int(dmimg::get_r(img_cpy, x + structural_el[i].x, y + structural_el[i].y)) == structural_el[i].value) {
						checks++;
					}
				}
				if (checks == structural_el_size) {
					// take care of structural element 6 and 8
					int counter = 0;
					if (structural_el[0].value == BG) {
						counter = 1;
					}

					// we need to set the foreground for all the pixels defined by the structural element
					while (counter < structural_el_size) {
						if (structural_el[counter].value == FG) {
							dmimg::set_rgb(img, x + structural_el[counter].x, y + structural_el[counter].y, FG);
						}
						counter++;
					}
				}
				else {
					// nothing as image is of background colour by default
				}

			}	// and now we are going to take a look at the next pixel
		}
	}
	// closing operation with the use of first dilation then erosion
	template <class T>
	void closing(CImg<T>& img, std::vector<xyval> structural_el) {
		dmimg::dilation(img, structural_el);
		dmimg::erosion(img, structural_el);
	}
	// HMT transformation
	template <class T>
	void hmt(CImg<T>& img, std::vector<xyval> structural_el) {
		CImg<T> img_cpy = img;
		int structural_el_size = structural_el.size();
		// we assume that each structural element is no more than a grid of 3x3
		// loops for going over all the pixels EXCEPT the very border
		for (int x = 1; x < dmimg::width(img) - 1; x++) {
			for (int y = 1; y < dmimg::height(img) - 1; y++) {
				int checks = 0;
				// check if from this pixel structural element meets the criteria
				// FG - must match with the image's pixel
				// GR - must be missed
				// BG - does not matter
				for (int i = 0; i < structural_el_size; i++) {
					// if we need to miss check if current pixel is BG
					if (structural_el[i].value == BG) {
						if (dmimg::get_r(img_cpy, x + structural_el[i].x, y + structural_el[i].y) == BG) {
							checks++;
						}
					}
					else if (structural_el[i].value == FG) {
						if (dmimg::get_r(img_cpy, x + structural_el[i].x, y + structural_el[i].y) == FG) {
							checks++;
						}
						// source pixel does not matter in that case so checks++
					}
					else if (structural_el[i].value == GR) {
						checks++;
					}

				}
				// now we have checked
				if (checks == structural_el_size) {
					dmimg::set_rgb(img, x, y, FG);
				}
				else {
					dmimg::set_rgb(img, x, y, BG);
				}
			}	// and now we are going to take a look at the next pixel
		} // and now another column of pixels
	}
	// M5 task variant helper function
	template <class T>
	bool are_bw_images_equal(CImg<T>& img1, CImg<T>& img2) {
		for (int x = 1; x < dmimg::width(img1) - 1; x++) {
			for (int y = 1; y < dmimg::height(img1) - 1; y++) {
				// if pixels are different we can return false here already 
				if (!(get_r(img1, x, y) == get_r(img2, x, y)))
					return false;
			}
		}
		return true;
	}
	// M5 task variant
	// N(A,B) = A - (A HMT with B)
	//			img - img_cpy
	template <class T>
	void m5(CImg<T>& img) {
		CImg<T> img_beginning = img;
		CImg<T> img_cpy = img;

		bool no_changes_made = false;
		// do while there are changes to be made
		while (!(no_changes_made)) {
			// main loop - applying 8 structural elements
			for (int s = 0; s < 8; s++) {
				// apply hmt
				// xii are <15, 22>
				dmimg::hmt(img_cpy, get_structural_element(15 + s));
				// subtract source from the hmt
				for (int x = 1; x < dmimg::width(img) - 1; x++) {
					for (int y = 1; y < dmimg::height(img) - 1; y++) {
						// if we have foreground check if the other image has foreground here too and if so remove this pixel
						if (get_r(img, x, y) == FG and get_r(img_cpy, x, y) == FG) {
							set_rgb(img, x, y, BG);
						}
					}
				} // end of substraction
				// restore img_cpy to be able to perform hmt transform properly again
				img_cpy = img_beginning;
			} // do next structural element
			// check if there were any changes
			no_changes_made = are_bw_images_equal(img_beginning, img);
			// prepare for the next round of applying the formula
			img_beginning = img;
			img_cpy = img;
		}
	}

	// R1 region growing (merging)
	struct Coordinates {
		int x;
		int y;
	};

	template <class T>
	bool is_processed(CImg<T>& img_copy, int x, int y) {
		bool is_red = img_copy(x, y, 0) == (unsigned char)255 &&
			img_copy(x, y, 1) == 0 &&
			img_copy(x, y, 2) == 0;
		return is_red;
	}

	// sqrt( (x1 - x2)^2 + (y1 - y2)^2 ) = distance
	double get_euclidean_distance(int r1, int g1, int b1, int r2, int g2, int b2) {
		return sqrt(pow(r2 - r1, 2) + pow(g2 - g1, 2) + pow(b2 - b1, 2));
	}

	template <class T>
	bool is_pixel_within_threshold(CImg<T>& image, int x1, int y1, int x2, int y2, int threshold) {
		double distance = dmimg::get_euclidean_distance(image(x1, y1), image(x1, y1, 0, 1), image(x1, y1, 2), image(x2, y2), image(x2, y2, 1), image(x2, y2, 2));
		return distance <= threshold;
	}

	template <class T>
	void color_red(CImg<T>& img_copy, int x, int y) {
		img_copy(x, y, 0) = (unsigned char)255;
		img_copy(x, y, 1) = 0;
		img_copy(x, y, 2) = 0;
	}

	template <class T>
	std::vector<Coordinates>  get_surrounding_pixels(CImg<T>& image, CImg<T>& img_copy, int x, int y, int seedX, int seedY, int threshold) {
		std::vector<Coordinates> valid_pixels;

		// ... ? ... : ... ; -> it prevents from going outside of the boarder
		for (int i = x > 1 ? x - 1 : 0; i <= x + 1 && i < dmimg::width(image); i++) {
			for (int j = y > 1 ? y - 1 : 0; j <= y + 1 && j < dmimg::height(image); j++) {
				if (dmimg::is_processed(img_copy, i, j)) continue; // if processed omit bottom part

				if (dmimg::is_pixel_within_threshold(image, seedX, seedY, i, j, threshold)) {
					dmimg::color_red(img_copy, i, j);
					valid_pixels.push_back({ i, j });
				}
			}
		}

		return valid_pixels;
	}

	template <class T>
	void perform_merging(CImg<T>& img, int seedX, int seedY, int threshold) {
		CImg<T> img_copy = img;
		std::queue<Coordinates> seed_points; // FIFO, first-in, first-out, like in the store
		std::vector<Coordinates> surrounding_pixels;
		Coordinates seed_pixel;
		seed_points.push({ seedX, seedY });

		while (!seed_points.empty()) {
			seed_pixel = seed_points.front(); // gets next element in the line
			seed_points.pop(); // removes next element in the line

			surrounding_pixels = dmimg::get_surrounding_pixels(img, img_copy, seed_pixel.x, seed_pixel.y, seedX, seedY, threshold);
			for (int i = 0; i < surrounding_pixels.size(); i++) {
				seed_points.push(surrounding_pixels[i]);
			}
			surrounding_pixels.clear();
		}

		img = img_copy;
	}
	// ######################################################################
	// TASK 4
	// fft
	using std::shared_ptr;
	using std::vector;
	using std::complex;

	constexpr float value_pi = 3.14159265358979323846; // approximate value of PI
	constexpr double value_pi_double = 3.141592653589793238460; // approximate value of PI
	// just check limits not to go over
	int check_value(int value) {
		if (value < 0) return 0;
		if (value > 255) return 255;

		return value;
	}

	template <class T>
	std::vector<std::vector<std::complex<double>>>& perform_slow_normal_descrete_fourier_transform(CImg<T>& original) {
		auto* output = new std::vector<std::vector<std::complex<double>>>;
		std::vector<std::vector<std::complex<double>>> temp;
		
		for (int y = 0; y < original.height(); y++) {
			std::vector<std::complex<double>> row; // before loop
			temp.push_back(row);
			for (int x = 0; x < original.width(); x++) {
				std::complex<double> sum = (0.0, 0.0);
				for (int xx = 0; xx < original.width(); xx++) {
					std::complex<double> W(cos(2 * value_pi * xx * x / original.width()), -sin(2 * value_pi * xx * x / original.width()));
					sum += static_cast<double>(original(y,xx))* W; // 
				}
				temp[y].push_back(sum);
			}
		}
		
		for (int x = 0; x < original.width(); x++) {
			std::vector<std::complex<double>> col;
			(*output).push_back(col);
			for (int y = 0; y < original.height(); y++) {
				std::complex<double> sum = (0.0, 0.0);
				for (int yy = 0; yy < original.height(); yy++) {
					std::complex<double> W(cos(2 * value_pi * yy * y / original.height()), -sin(2 * value_pi * yy * y / original.height()));
					sum += temp[yy][x] * W;
				}
				(*output)[x].push_back(sum);
			}
		}

		return *output;
	}

	CImg<unsigned char>& perform_slow_inversed__descrete_fourier_transform(std::vector<std::vector<std::complex<double>>>& original) {
		auto* output = new CImg<unsigned char>(original[0].size(), original.size(), 1, 3, 0);
		std::vector<std::vector<std::complex<double>>> temp;

		for (int x = 0; x < (*output).width(); x++) {
			std::vector<std::complex<double>> col;
			temp.push_back(col);
			for (int y = 0; y < (*output).height(); y++) {
				std::complex<double> sum = (0.0, 0.0);
				for (int yy = 0; yy < (*output).height(); yy++) {
					std::complex<double> comp(cos(2 * value_pi * yy * y / (*output).height()), sin(2 * value_pi * yy * y / (*output).height()));
					sum += original[x][yy] * comp;
				}
				temp[x].push_back(sum / (double)(*output).height());
			}
		}

		for (int y = 0; y < (*output).height(); y++) {
			std::vector<std::complex<double>> row;
			temp.push_back(row);
			for (int x = 0; x < (*output).width(); x++) {
				std::complex<double> sum = (0.0, 0.0);
				for (int xx = 0; xx < (*output).width(); xx++) {
					std::complex<double> comp(cos(2 * value_pi * xx * x / (*output).width()), sin(2 * value_pi * xx * x / (*output).height()));
					sum += temp[xx][y] * comp;
				}
				for (int c = 0; c < 3; c++) {
					(*output)(y, x, c) = dmimg::check_value(abs(sum) / original.size());
				}
			}
		}

		return *output;
	}

	std::vector<std::vector<std::complex<double>>>& swap_quarters(std::vector<std::vector<std::complex<double>>>& original) {
		auto* output = new std::vector<std::vector<std::complex<double>>>;

		for (int x = 0; x < original.size(); x++) {
			std::vector<std::complex<double>> column;
			for (int y = 0; y < original.size(); y++) {
				column.push_back(original[x][y]);
			}
			(*output).push_back(column); column.clear();
		}

		for (int x = 0; x < original.size() / 2; x++) {
			for (int y = 0; y < original[0].size() / 2; y++) {
				std::swap((*output)[x][y], (*output)[original.size() / 2 + x][original[0].size() / 2 + y]);
				std::swap((*output)[original.size() / 2 + x][y], (*output)[x][original[0].size() / 2 + y]);
			}
		}

		return *output;
	}


	CImg<unsigned char>& visualisation_image(std::vector<std::vector<std::complex<double>>>& original) { // only for vis.
		// this will be created on heap so deletion is required afterwards, at the end of the program
		// smart pointer may be used
		auto* output = new CImg<unsigned char>(original[0].size(), original.size(), 1, 3, 0);

		for (int x = 0; x < original[0].size(); x++) {
			for (int y = 0; y < original.size(); y++) {
				for (int c = 0; c < 3; c++) {
					(*output)(x, y, c) = dmimg::check_value(log(abs(original[y][x])) * 15.0); // magnitude spect.
				}
			}
		}

		return *output;
	}

	CImg<unsigned char> image_from_complex(vector<vector<complex<double>>>& original) {
		CImg<unsigned char> output(original[0].size(), original.size(), 1, 3, 0);

		for (int x = 0; x < original[0].size(); x++) {
			for (int y = 0; y < original.size(); y++) {
				for (int c = 0; c < 3; c++) {
					output(x, y, c) = dmimg::check_value(log(abs(original[y][x])) * 15.0); // magnitude spect.
				}
			}
		}
		return output;
	}

	// helper fuction
	void print_complex_array(vector<vector<complex<double>>>& input) {
		for (int y = 0; y < input.size(); y++) {
			for (int x = 0; x < input[0].size(); x++) {
				std::cerr << "abs: " << std::abs(input[y][x]) << " ";
			}
		}
	}

	bool is_power_of_two(int n)
	{
		return (std::ceil(log2(n)) == std::floor(log2(n)));
	}

	// changes quarters of the array of complex
	shared_ptr<vector<vector<complex<double>>>> swap_quarters_ptr(vector<vector<complex<double>>>& original) {
		auto output = std::make_shared<vector<vector<complex<double>>>>();

		for (int x = 0; x < original.size(); x++) {
			std::vector<std::complex<double>> column;
			for (int y = 0; y < original.size(); y++) {
				column.push_back(original[x][y]);
			}
			(*output).push_back(column); column.clear();
		}

		for (int x = 0; x < original.size() / 2; x++) {
			for (int y = 0; y < original[0].size() / 2; y++) {
				std::swap((*output)[x][y], (*output)[original.size() / 2 + x][original[0].size() / 2 + y]);
				std::swap((*output)[original.size() / 2 + x][y], (*output)[x][original[0].size() / 2 + y]);
			}
		}

		return output;
	}

	// transforms CImg image to array of complex numbers
	template <class T>
	shared_ptr<vector<vector<complex<double>>>> image_to_complex(CImg<T>& input_img) {
		auto output = std::make_shared<vector<vector<complex<double>>>>();
		for (int y = 0; y < input_img.height(); y++) {
			vector<complex<double>> temp;
			for (int x = 0; x < input_img.width(); x++) {
				temp.push_back(get_r(input_img, x, y));
			}
			output->push_back(temp);
		}
		return output;
	}

	// fast Fourier transform with decimation in spatial domain
	// we assume compatibility with grayscale images only

	void fast_fourier_1D(vector<complex<double>>& input) {
		int size = input.size();
		if (size <= 1) return;

		// divide
		vector<complex<double>> even;
		for (int i = 0; i < size; i = i + 2) {
			even.push_back(input[i]);
		}
		vector<complex<double>> odd;
		for (int i = 1; i < size; i = i + 2) {
			odd.push_back(input[i]);
		}

		// conquer
		fast_fourier_1D(even);
		fast_fourier_1D(odd);

		// combine the parts
		for (int k = 0; k < size / 2; k++)
		{
			complex<double> t = std::polar(1.0, -2 * value_pi_double * k / size) * odd[k];
			input[k] = even[k] + t;
			input[k + size / 2] = even[k] - t;
		}
	}

	void fast_fourier_2D(vector<vector<complex<double>>>& input) {
		if (!(is_power_of_two(input.size()))) dmimg::error("Can't perfrom fft if height is not a power of 2.");
		if (!(is_power_of_two(input[0].size()))) dmimg::error("Can't perfrom fft if width is not a power of 2.");
		// perform rows
		for (int row = 0; row < input.size(); row++) {
			fast_fourier_1D(input[row]);
		}
		// perform columns
		for (int column = 0; column < input[0].size(); column++) {
			// gather one column and perform fft
			vector<complex<double>> temp;
			for (int y = 0; y < input.size(); y++) {
				// read values into temp
				temp.push_back(input[y][column]);
			}
			fast_fourier_1D(temp);
			// save fft of a column
			for (int y = 0; y < input.size(); y++) {
				input[y][column] = temp[y];
			}
		}
	}

	// takes input image and computes fast Fourier transform with decimation in spatial domain
	// returns smart pointer of vector of vectors of complex numbers
	template <class T>
	shared_ptr<vector<vector<complex<double>>>> fft_spatial_domain(CImg<T>& input_img) {
		auto output = std::make_shared<vector<vector<complex<double>>>>();

		// check if width and height are a power of 2
		if (!(is_power_of_two(input_img.width()))) dmimg::error("Can't perfrom fft if width is not a power of 2.");
		if (!(is_power_of_two(input_img.height()))) dmimg::error("Can't perfrom fft if height is not a power of 2.");

		// perform rows
		for (int row = 0; row < input_img.height(); row++) {
			vector<complex<double>> temp;
			for (int x = 0; x < input_img.width(); x++) {
				temp.push_back(get_r(input_img, x, row));
			}
			fast_fourier_1D(temp);
			output->push_back(temp);
		}
		// perform columns
		for (int column = 0; column < input_img.width(); column++) {
			// gather one column and perform fft
			vector<complex<double>> temp;
			for (int y = 0; y < input_img.height(); y++) {
				// read values into temp
				temp.push_back((*output)[y][column]);
			}
			// do fft
			fast_fourier_1D(temp);
			// save fft into output
			for (int y = 0; y < input_img.height(); y++) {
				// read values into temp
				(*output)[y][column] = temp[y];
			}
		}

		return output;
	}

	void fft_inverse_2D(vector<vector<complex<double>>>& input) {
		// check if dimensions are a power of 2
		if (!(is_power_of_two(input.size()))) dmimg::error("Can't perfrom fft if height is not a power of 2.");
		if (!(is_power_of_two(input[0].size()))) dmimg::error("Can't perfrom fft if width is not a power of 2.");

		// conjugate the complex numbers for the first time
		for (int y = 0; y < input.size(); y++) {
			for (int x = 0; x < input[0].size(); x++) {
				input[y][x] = std::conj(input[y][x]);
			}
		}
		// forward fft
		fast_fourier_2D(input);

		// conjugate the complex numbers again
		for (int y = 0; y < input.size(); y++) {
			for (int x = 0; x < input[0].size(); x++) {
				input[y][x] = std::conj(input[y][x]);
			}
		}
		// scale the numbers properly
		double sizes_multiplied = input.size() * input[0].size();
		for (int y = 0; y < input.size(); y++) {
			for (int x = 0; x < input[0].size(); x++) {
				input[y][x] = input[y][x] / sizes_multiplied;
			}
		}
	}

	// takes a table of complex numbers, performs inverse fft and returns an image
	// input is not a reference because we need to modify (we need a copy) it to get an image
	CImg<unsigned char> fft_inverse_2D_img(vector<vector<complex<double>>> input) {
		fft_inverse_2D(input);
		CImg<unsigned char> output(input[0].size(), input.size(), 1, 3);
		for (int y = 0; y < input.size(); y++) {
			for (int x = 0; x < input[0].size(); x++) {
				dmimg::set_rgb_safe(output, x, y, int(std::abs(input[y][x])));
			}
		}
		return output;
	}

	// filters

	shared_ptr<vector<vector<complex<double>>>> high_pass_filter_mask(int width, int height, double band_size) {
		auto mask = std::make_shared<vector<vector<complex<double>>>>();

		int half_width = width / 2;
		int half_height = height / 2;
		double one = 1;
		double zero = 0;

		for (int y = 0; y < height; y++) {
			vector<complex<double>> temp_row;
			for (int x = 0; x < width; x++) {
				// distance to point
				int distance = std::sqrt((x - half_width) * (x - half_width) + (y - half_height) * (y - half_height));
				if (distance > band_size) {
					temp_row.push_back(one);
				}
				else {
					temp_row.push_back(zero);
				}
			}
			mask->push_back(temp_row);
		}
		return mask;
	}

	void high_pass_filter(vector<vector<complex<double>>>& input, int band_size) {
		shared_ptr<vector<vector<complex<double>>>> mask = high_pass_filter_mask(input[0].size(), input.size(), band_size);
		for (int y = 0; y < input.size(); y++) {
			for (int x = 0; x < input[0].size(); x++) {
				input[y][x] = input[y][x] * (*mask)[y][x];
				//std::cerr << (*mask)[y][x] << " ";
			}
		}
	}

	shared_ptr<vector<vector<complex<double>>>> high_pass_edge_detection_filter_mask(int width, int height, int angle, int detection_width, int radius, bool zeroAtXaxxis) {
		auto mask = std::make_shared<vector<vector<complex<double>>>>();
		if (!(-45 <= angle and angle <= 45)) dmimg::error("invalid angle: required range <-45,45>");
		if (!(0 < detection_width and detection_width < 90)) dmimg::error("invalid width: required range (-90,90)");
		// radius is not checked as we run through all positions and check if they belong or not to the circle

		int half_width = width / 2;
		int half_height = height / 2;
		double one = 1;
		double zero = 0;
		// precision of mask calculation, for very high resolution images the value should be lowered
		double mask_precision = 0.001;

		// initialize the mask
		for (int y = 0; y < height; y++) {
			vector<complex<double>> temp_row;
			for (int x = 0; x < width; x++) {
				temp_row.push_back(zero);
			}
			mask->push_back(temp_row);
		}

		double angle_base = (angle)*value_pi_double / 180.0;
		double a_min = std::tan(angle_base - (detection_width / 2) * value_pi_double / 180.0);
		double a_max = std::tan(angle_base + (detection_width / 2) * value_pi_double / 180.0);

		if (zeroAtXaxxis == true) {
			for (double a = a_min; a < a_max; a += mask_precision) {
				// do the function for parameter a
				for (int x = 0; x < width; x++) {
					double y_double = a * (x - half_width) + half_height;
					int y = int(y_double);
					if (0 <= y and y < height) {
						(*mask)[y][x] = one;
					}
				}
			}
		}
		else // we need to take the y axis as base
		{
			for (double a = a_min; a < a_max; a += mask_precision) {
				// do the function for parameter a
				for (int y = 0; y < height; y++) {
					double x_double = a * (y - half_height) + half_width;
					int x = int(x_double);
					if (0 <= x and x < width) {
						(*mask)[y][x] = one;
					}
				}
			}
		}
		// now, remove the low-frequencies
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				// distance to point
				int distance = std::sqrt((x - half_width) * (x - half_width) + (y - half_height) * (y - half_height));
				if (distance < radius) {
					(*mask)[y][x] = zero;
				}
			}
		}
		// done
		return mask;
	}

	void high_pass_filter_edge_detection(vector<vector<complex<double>>>& input, int angle, int detection_width, int radius, int zeroAtXaxxis) {
		if (!(zeroAtXaxxis == 0 or zeroAtXaxxis == 1)) dmimg::error("invalid parameter: zeroAtXaxxis can be 0 or 1");
		bool zeroAtXaxxisBool = (zeroAtXaxxis == 1) ? true : false;
		shared_ptr<vector<vector<complex<double>>>> mask = high_pass_edge_detection_filter_mask(input[0].size(), input.size(), angle, detection_width, radius, zeroAtXaxxisBool);
		for (int y = 0; y < input.size(); y++) {
			for (int x = 0; x < input[0].size(); x++) {
				input[y][x] = input[y][x] * (*mask)[y][x];
			}
		}
	}

	shared_ptr<vector<vector<complex<double>>>> phase_modifying_filter_mask(int width, int height, int l, int k) {
		auto mask = std::make_shared<vector<vector<complex<double>>>>();
		using namespace std::complex_literals;

		for (int y = 0; y < height; y++) {
			vector<complex<double>> temp_row;
			for (int x = 0; x < width; x++) {
				complex<double> p, inside;
				inside = ((-1.0 * x) * double(k) * 2.0 * value_pi_double) / width +
					((-1.0 * y) * double(l) * 2.0 * value_pi_double) / height +
					double(k + l) * value_pi_double;
				inside *= 1i;
				p = exp(inside);
				temp_row.push_back(p);
			}
			mask->push_back(temp_row);
		}
		// done
		return mask;
	}

	void phase_modifying_filter(vector<vector<complex<double>>>& input, int l, int k) {
		shared_ptr<vector<vector<complex<double>>>> mask = phase_modifying_filter_mask(input[0].size(), input.size(), l, k);
		for (int y = 0; y < input.size(); y++) {
			for (int x = 0; x < input[0].size(); x++) {
				// we are interested only in imaginary part as real part of the mask should be 1
				input[y][x] = input[y][x] * (*mask)[y][x];
			}
		}
	}
	
	// commented out to fit in shr_ptr not raw ptr
	void lowpass_filter(vector<vector<complex<double>>>& original, int threshold) {
		//auto* edited = new vector<vector<complex<double>>>;
		//for (const auto& i : original) (*edited).push_back(i);

		for (int x = 1; x < original.size(); x++) {
			for (int y = 1; y < original[1].size(); y++) {
				double d = sqrt(pow(x - original.size() / 2.0, 2.0) + pow(y - original[1].size() / 2.0, 2.0));
				if (d > threshold) {
					//edited->at(y).at(x) = (0.0, 0.0);
					original[y][x] = (0.0, 0.0);
				}
			}
		}
	//	original = *edited;
	}

	// commented out to fit in shr_ptr not raw ptr
	void bandpass_filter(vector<vector<complex<double>>>& original, int threshold, int width) {
		//auto* edited = new vector<vector<complex<double>>>;
		//for (const auto& i : original) (*edited).push_back(i);

		for (int x = 0; x < original.size(); x++) {
			for (int y = 0; y < original[0].size(); y++) {
				double d = sqrt(pow(x - original.size() / 2.0, 2.0) + pow(y - original[0].size() / 2.0, 2.0));
				if ((threshold - width / 2.0 > d) || (threshold + width / 2.0 < d)) {
					//edited->at(y).at(x) = (0.0, 0.0);
					original[y][x] = (0.0, 0.0);
				}
			}
		}
		//original = *edited;
	}

	void bandcut_filter(vector<vector<complex<double>>>& original, int threshold, int width) {
		//auto* edited = new vector<vector<complex<double>>>;
		//for (const auto& i : original) (*edited).push_back(i);

		for (int x = 0; x < original.size(); x++) {
			for (int y = 0; y < original[0].size(); y++) {
				double d = sqrt(pow(x - original.size() / 2.0, 2.0) + pow(y - original[0].size() / 2.0, 2.0));
				if ((threshold - width / 2.0 <= d) && (threshold + width / 2.0 >= d)) {
					//edited->at(y).at(x) = (0.0, 0.0);
					original[y][x] = (0.0, 0.0);
				}
			}
		}
		//original = *edited;
		
	}


} // end of dmimg namespace

int main(int argc, char** argv) {
	// program invocation can look like this
	// dmimg.exe images/24bit_color/lenac.bmp lenac_output.bmp --brightness 25

	CLI::App app{ "Welcome! This program by Dominik Czerwoniuk 230446 and Maciej Kopa 230451 allows for several interesting image manupulations." };
	// fetching input and output filenames
	std::string source_file = "";
	app.add_option("filename, -s, --source", source_file, "Path to first image (usually source image)");

	std::string output_file = "";
	app.add_option("filename, -o, --output", output_file, "Path to second image (usually output image)");


	// groups of operations --brightness --contrast etc. but only one can be applied
	auto operations = app.add_option_group("Operations", "The program allows for several image manipulations techniques.");
	operations->require_option(1);
	// variable which is set when an operation needs an argument
	std::vector<int> argument;
	//	int argument = 332;
	//	int argument_2 = 346;
	//	int argument_3 = 0;
		// ######################################################################
		// Task 1 - B
	auto brightness = operations->add_option_group("brightness", "Brightness modification");
	brightness->add_option("--brightness", argument, "Alter the brightness by a given argument");
	brightness->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::brightness(img, argument[0]);
		img.save(output_file.c_str());
		});
	auto contrast = operations->add_option_group("contrast", "Contrast modification");
	contrast->add_option("--contrast", argument, "Alter the contrast by a given amount");
	contrast->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::contrast(img, argument[0]);
		img.save(output_file.c_str());
		});
	auto negative = operations->add_option_group("negative", "Inverse the image");
	negative->add_flag("--negative", "Output image will be inversed");
	negative->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::negative(img);
		img.save(output_file.c_str());
		});
	// Task 1 - G 
	auto hflip = operations->add_option_group("horizontal flip", "Horizontal flip of the image");
	hflip->add_flag("--hflip", "Flip the img horizontally");
	hflip->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::hflip(img);
		img.save(output_file.c_str());
		});
	auto vflip = operations->add_option_group("vertical flip", "Vertical flip of the image");
	vflip->add_flag("--vflip", "Flip the img vertically");
	vflip->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::vflip(img);
		img.save(output_file.c_str());
		});
	auto dflip = operations->add_option_group("diagonal flip", "Diagonal flip of the image");
	dflip->add_flag("--dflip", "Diagonal flip of the image");
	dflip->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::dflip(img);
		img.save(output_file.c_str());
		});
	auto shrink = operations->add_option_group("shrink", "Shrinks the image x2");
	shrink->add_flag("--shrink", "Shrink the image x2");
	shrink->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::shrink(img);
		img.save(output_file.c_str());
		});
	auto enlarge = operations->add_option_group("enlarge", "Scales the image x2");
	enlarge->add_flag("--enlarge", "Scale the img x2");
	enlarge->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::enlarge(img);
		img.save(output_file.c_str());
		});
	// Task 1 - N4
	auto mid = operations->add_option_group("midpoint filter", "Applies midpoint filter");
	mid->add_flag("--mid", "Apply midpoint filter to an image");
	mid->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::mid(img);
		img.save(output_file.c_str());
		});
	auto amean = operations->add_option_group("arithmetic mean filter", "Applies arithmetic mean filter");
	amean->add_flag("--amean", "Apply arithmetic mean filter to an image");
	amean->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::amean(img);
		img.save(output_file.c_str());
		});
	// Task 1 - E
	auto mse = operations->add_option_group("mean squared error", "Computes mean squared error of img1 & img2");
	mse->add_flag("--mse", "Compute mean squared error (mse) of img1 & img2");
	mse->callback([&]() {
		CImg<unsigned char> img1(source_file.c_str());
		CImg<unsigned char> img2(output_file.c_str());
		std::cout << "mse: " << dmimg::mse(img1, img2) << std::endl;
		});
	auto pmse = operations->add_option_group("peak mean squared error", "Computes peak mean squared error of img1 & img2");
	pmse->add_flag("--pmse", "Compute peak mean squared error (pmse) of img1 & img2");
	pmse->callback([&]() {
		CImg<unsigned char> img1(source_file.c_str());
		CImg<unsigned char> img2(output_file.c_str());
		std::cout << "pmse: " << dmimg::pmse(img1, img2) << std::endl;
		});
	auto snr = operations->add_option_group("signal to noise ratio", "Computes signal to noise ratio of img1 & img2");
	snr->add_flag("--snr", "Compute signal to noise ratio (snr) of img1 & img2 where img1 is P_signal and img2 is P_noise");
	snr->callback([&]() {
		CImg<unsigned char> img1(source_file.c_str());
		CImg<unsigned char> img2(output_file.c_str());
		std::cout << "snr: " << dmimg::snr(img1, img2) << std::endl;
		});
	auto psnr = operations->add_option_group("peak signal to noise ratio", "Computes peak signal to noise ratio of img1 & img2");
	psnr->add_flag("--psnr", "Compute peak signal to noise ratio (psnr) of img1 & img2");
	psnr->callback([&]() {
		CImg<unsigned char> img1(source_file.c_str());
		CImg<unsigned char> img2(output_file.c_str());
		std::cout << "psnr: " << dmimg::psnr(img1, img2) << std::endl;
		});
	auto md = operations->add_option_group("maximum difference", "Computes  maximum difference of img1 & img2");
	md->add_flag("--md", "Compute maximum difference (md) of img1 & img2");
	md->callback([&]() {
		CImg<unsigned char> img1(source_file.c_str());
		CImg<unsigned char> img2(output_file.c_str());
		dmimg::md(img1, img2);
		});
	// ######################################################################
	// Task 2
	int slowpass_argument = 1;
	auto slowpass = operations->add_option_group("Low-pass filter", "Creates low pass filter image");
	slowpass->add_option("--slowpass", slowpass_argument, "Applies low-pass filter to an image. There are 3 variants chosen with the argument (from 1 to 3)");
	slowpass->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		int mask[3][3];
		int mask_1[3][3] = {
			{1, 1, 1} ,   /*  initializers for row indexed by 0 */
			{1, 1, 1} ,   /*  initializers for row indexed by 1 */
			{1, 1, 1}   /*  initializers for row indexed by 2 */
		};
		int mask_2[3][3] = {
			{1, 1, 1} ,
			{1, 2, 1} ,
			{1, 1, 1}
		};
		int mask_3[3][3] = {
			{1, 2, 1} ,
			{2, 4, 2} ,
			{1, 2, 1}
		};
		int divider = 1; // dividing each pixel by a given value after applying the mask
		switch (slowpass_argument) {
		case 1:
		{
			divider = 9;
			dmimg::copy_2d_3x3_array(mask_1, mask);
			break;
		}
		case 2:
		{
			divider = 10;
			dmimg::copy_2d_3x3_array(mask_2, mask);
			break;
		}
		case 3:
		{
			divider = 16;
			dmimg::copy_2d_3x3_array(mask_3, mask);
			break;
		}
		default:
			dmimg::error("Low-pass filter: wrong argument!");
			break;
		}
		// start measuring time
		auto start = std::chrono::high_resolution_clock::now();
		dmimg::conv_mask(img, mask, divider);
		// stop the timer
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "Low-pass filter applied in: " << duration.count() << " microseconds." << std::endl;

		img.save(output_file.c_str());
		});
	auto slowpass_optimized = operations->add_option_group("Low-pass filter optimized", "Creates low pass filter image with variant 2");
	slowpass_optimized->add_flag("--slowpass_optimized", "Applies optimized low-pass filter to an image - variant 2");
	slowpass_optimized->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		// start measuring time
		auto start = std::chrono::high_resolution_clock::now();

		dmimg::slowpass_optimized(img);

		// stop the timer
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "Optimized low-pass filter applied in: " << duration.count() << " microseconds." << std::endl;

		img.save(output_file.c_str());
		});
	// Rosenfeld operator
	auto orosenfeld = operations->add_option_group("orosenfeld", "Rosenfeld operator");
	orosenfeld->add_option("--orosenfeld", argument, "Apply Rosenfeld operator with an p argument which can take following values p = 1, 2, 4, 8, 16, ...");
	orosenfeld->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::orosenfeld(img, argument[0]);
		img.save(output_file.c_str());
		});
	// Histogram
	auto histogram = operations->add_option_group("histogram", "Histogram");
	histogram->add_option("--histogram", argument, "Generate Histogram");
	histogram->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::histogram(img, argument[0]);
		img.save(output_file.c_str());
		});
	// Power 2/3 final probability density function
	auto hpower = operations->add_option_group("hpower", "Power 2/3 final probability density function");
	hpower->add_option("--hpower", argument, "Computes power 2/3 final probability density function");
	hpower->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::hpower(img, argument[0], argument[1]);
		img.save(output_file.c_str());
		});
	// Variation coefficient II
	auto cvarcoii = operations->add_option_group("cvarcoii", "Variation coefficient II");
	cvarcoii->add_flag("--cvarcoii", "Computes variation coefficient II of image");
	cvarcoii->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		dmimg::cvarcoii(img);
		});
	// ######################################################################
	// Task 3

	// erosion
	auto erosion = operations->add_option_group("erosion", "erosion operation");
	erosion->add_option("--erosion", argument, "Apply erosion of the image with specific structural element");
	erosion->callback([&]() {
		using namespace dmimg;
		CImg<unsigned char> img(source_file.c_str());
		if (1 > argument[0] or argument[0] > 10) dmimg::error("Wrong argument! This operation can take structural element from 1 to 10");
		dmimg::erosion(img, dmimg::get_structural_element(argument[0]));
		img.save(output_file.c_str());
		});
	// dilation
	auto dilation = operations->add_option_group("dilation", "dilation operation");
	dilation->add_option("--dilation", argument, "Apply dilation of the image with specific structural element");
	dilation->callback([&]() {
		using namespace dmimg;
		CImg<unsigned char> img(source_file.c_str());
		if (1 > argument[0] or argument[0] > 10) dmimg::error("Wrong argument! This operation can take structural element from 1 to 10");
		dmimg::dilation(img, dmimg::get_structural_element(argument[0]));
		img.save(output_file.c_str());
		});
	// slow version of opening - first dilation then erosion
	auto opening_slow = operations->add_option_group("opening", "opening operation - slow version");
	opening_slow->add_option("--opening_slow", argument[0], "Apply slow version of the opening of the image with specific structural element");
	opening_slow->callback([&]() {
		using namespace dmimg;
		CImg<unsigned char> img(source_file.c_str());
		if (1 > argument[0] or argument[0] > 10) dmimg::error("Wrong argument! This operation can take structural element from 1 to 10");

		// start measuring time
		auto start = std::chrono::high_resolution_clock::now();

		dmimg::opening_slow(img, dmimg::get_structural_element(argument[0]));

		// stop the timer
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "Applied in: " << duration.count() << " microseconds." << std::endl;

		img.save(output_file.c_str());
		});
	// opening
	auto opening = operations->add_option_group("opening", "opening operation");
	opening->add_option("--opening", argument, "Apply opening of the image with specific structural element");
	opening->callback([&]() {
		using namespace dmimg;
		CImg<unsigned char> img(source_file.c_str());
		if (1 > argument[0] or argument[0] > 10) dmimg::error("Wrong argument! This operation can take structural element from 1 to 10");

		// start measuring time
		auto start = std::chrono::high_resolution_clock::now();

		dmimg::opening(img, dmimg::get_structural_element(argument[0]));

		// stop the timer
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "Applied in: " << duration.count() << " microseconds." << std::endl;

		img.save(output_file.c_str());
		});
	// closing
	auto closing = operations->add_option_group("closing", "closing operation");
	closing->add_option("--closing", argument, "Apply closing of the image with specific structural element");
	closing->callback([&]() {
		using namespace dmimg;
		CImg<unsigned char> img(source_file.c_str());
		if (1 > argument[0] or argument[0] > 10) dmimg::error("Wrong argument! This operation can take structural element from 1 to 10");

		// start measuring time
		auto start = std::chrono::high_resolution_clock::now();

		dmimg::closing(img, dmimg::get_structural_element(argument[0]));

		// stop the timer
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "Applied in: " << duration.count() << " microseconds." << std::endl;

		img.save(output_file.c_str());
		});
	// closing
	auto hmt = operations->add_option_group("HMT transform", "Apply HMT transform with selected structuring element");
	hmt->add_option("--hmt", argument, "Apply closing of the image with specific structural element");
	hmt->callback([&]() {
		using namespace dmimg;
		CImg<unsigned char> img(source_file.c_str());
		// start measuring time
		auto start = std::chrono::high_resolution_clock::now();
		// the function will return an error when the argument is out of range as hmt transform can use any structural element
		dmimg::hmt(img, dmimg::get_structural_element(argument[0]));

		// stop the timer
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "Applied in: " << duration.count() << " microseconds." << std::endl;

		img.save(output_file.c_str());
		});
	// m5
	auto m5 = operations->add_option_group("M5", "Apply M5");
	m5->add_flag("--m5", "Apply M5");
	m5->callback([&]() {
		using namespace dmimg;
		CImg<unsigned char> img(source_file.c_str());
		// start measuring time
		auto start = std::chrono::high_resolution_clock::now();
		// the function will return an error when the argument is out of range as hmt transform can use any structural element
		dmimg::m5(img);

		// stop the timer
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "Applied in: " << duration.count() << " microseconds." << std::endl;

		img.save(output_file.c_str());
		});
	// region growin (merging)
	auto merging = operations->add_option_group("merging", "Region merging");
	merging->add_option("--merging", argument, "Region merging");
	merging->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());
		// void (CImg<T>& img, int seedX, int seedY, int threshold)
		dmimg::perform_merging(img, argument[0], argument[1], argument[2]);
		img.save(output_file.c_str());
		});
	// ######################################################################
	// Task 4 
	//
	auto slow_normal_descrete_fourier_transform = operations->add_option_group("sndft", "Perform slow normal descrete Fourier Transform");
	slow_normal_descrete_fourier_transform->add_flag("--sndft", "Transform image using slow normal descrete Fourier Transform");
	slow_normal_descrete_fourier_transform->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());

		std::vector<std::vector<std::complex<double>>> fourier;
		fourier = dmimg::perform_slow_normal_descrete_fourier_transform(img);
		fourier = dmimg::swap_quarters(fourier);
		img = dmimg::visualisation_image(fourier);

		img.save(output_file.c_str());
		});
	// 
	auto slow_inversed__descrete_fourier_transform = operations->add_option_group("sidft", "Perform slow inverse descrete Fourier Transform");
	slow_inversed__descrete_fourier_transform->add_flag("--sidft", "Convert image using slow inverse descrete Fourier Transform");
	slow_inversed__descrete_fourier_transform->callback([&]() {
		CImg<unsigned char> img1(source_file.c_str());
		CImg<unsigned char> img2(source_file.c_str());

		std::vector<std::vector<std::complex<double>>> fourier;
		fourier = dmimg::perform_slow_normal_descrete_fourier_transform(img1);
		fourier = dmimg::swap_quarters(fourier);
		img1 = dmimg::visualisation_image(fourier);

		img1.save("DFT_comparison.bmp");

		img2 = dmimg::perform_slow_inversed__descrete_fourier_transform(fourier);
		//img2 = dmimg::visualisation_image(fourier);

		img2.save(output_file.c_str());
		});
	auto fft = operations->add_option_group("fft", "Perform Fast Fourier Transform");
	fft->add_flag("--fft", "Transform image using Fast Fourier Transform");
	fft->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());

		auto fourier = std::make_shared<std::vector<std::vector<std::complex<double>>>>();

		fourier = dmimg::fft_spatial_domain(img);
		fourier = dmimg::swap_quarters_ptr(*fourier);
		img = dmimg::image_from_complex(*fourier);

		img.save(output_file.c_str());
		});
	auto fft_inverse = operations->add_option_group("fft_inverse", "Perform Inverse Fast Fourier Transform");
	fft_inverse->add_flag("--ffti", "Transform image using inverse Fast Fourier Transform");
	fft_inverse->callback([&]() {

		CImg<unsigned char> img1(source_file.c_str());
		CImg<unsigned char> img2(source_file.c_str());

		auto fourier = std::make_shared<std::vector<std::vector<std::complex<double>>>>();

		fourier = dmimg::fft_spatial_domain(img1);
		fourier = dmimg::swap_quarters_ptr(*fourier);
		img1 = dmimg::image_from_complex(*fourier);

		img1.save("FFT_comparison.bmp");
		img2 = dmimg::fft_inverse_2D_img(*fourier);

		img2.save(output_file.c_str());

		});
	auto fft_hp = operations->add_option_group("ffthp", "Perform High Pass Filter with Fast Fourier Transform");
	fft_hp->add_option("--ffthp", argument, "Transform image using Fast Fourier Transform - high pass");
	fft_hp->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());

		auto fourier = std::make_shared<std::vector<std::vector<std::complex<double>>>>();

		fourier = dmimg::fft_spatial_domain(img);
		fourier = dmimg::swap_quarters_ptr(*fourier);
		dmimg::high_pass_filter(*fourier, argument[0]);
		img = dmimg::fft_inverse_2D_img(*fourier);
		//img = dmimg::image_from_complex(*fourier);

		img.save(output_file.c_str());
		});
	auto fft_hped = operations->add_option_group("ffthped", "Perform High Pass Filter with edge detection");
	fft_hped->add_option("--ffthped", argument, "Transform image using Fast Fourier Transform - high pass with edge detection. Function takes three arguments: angle (degrees), width (degrees), radius (units), is X axxis the base ( 1 or 0 )");
	fft_hped->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());

		auto fourier = std::make_shared<std::vector<std::vector<std::complex<double>>>>();

		fourier = dmimg::fft_spatial_domain(img);
		fourier = dmimg::swap_quarters_ptr(*fourier);
		dmimg::high_pass_filter_edge_detection(*fourier, argument[0], argument[1], argument[2], argument[3]);
		img = dmimg::fft_inverse_2D_img(*fourier);
		//img = dmimg::image_from_complex(*fourier);

		img.save(output_file.c_str());
		});
	auto fft_pmf = operations->add_option_group("fftpmf", "Perform Phase Modifying Filter");
	fft_pmf->add_option("--fftpmf", argument, "Transform image using Fast Fourier Transform - phase modifying fiter. Function takes two arguments: l and k");
	fft_pmf->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());

		auto fourier = std::make_shared<std::vector<std::vector<std::complex<double>>>>();

		fourier = dmimg::fft_spatial_domain(img);
		fourier = dmimg::swap_quarters_ptr(*fourier);
		dmimg::phase_modifying_filter(*fourier, argument[0], argument[1]);
		img = dmimg::fft_inverse_2D_img(*fourier);
		//img = dmimg::image_from_complex(*fourier);

		img.save(output_file.c_str());
		});
	auto fft_lpf = operations->add_option_group("fftlpf", "Perform Low-pass filter");
	fft_lpf->add_option("--fftlpf", argument, "Transform image using Fast Fourier Transform -> low-pass fiter. Takes 1 argument = threshold");
	fft_lpf->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());

		auto fourier = std::make_shared<std::vector<std::vector<std::complex<double>>>>();

		fourier = dmimg::fft_spatial_domain(img);
		fourier = dmimg::swap_quarters_ptr(*fourier);
		dmimg::lowpass_filter(*fourier, argument[0]);
		img = dmimg::fft_inverse_2D_img(*fourier);
		//img = dmimg::image_from_complex(*fourier);

		img.save(output_file.c_str());
		});
	auto fft_bpf = operations->add_option_group("fftbpf", "Perform Band-pass filter");
	fft_bpf->add_option("--fftbpf", argument, "Transform image using Fast Fourier Transform -> band-pass fiter. Takes 2 arguments: threshold and width.");
	fft_bpf->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());

		auto fourier = std::make_shared<std::vector<std::vector<std::complex<double>>>>();

		fourier = dmimg::fft_spatial_domain(img);
		fourier = dmimg::swap_quarters_ptr(*fourier);
		dmimg::bandpass_filter(*fourier, argument[0], argument[1]);
		img = dmimg::fft_inverse_2D_img(*fourier);
		//img = dmimg::image_from_complex(*fourier);

		img.save(output_file.c_str());
		});
	auto fft_bcf = operations->add_option_group("fftbcf", "Perform Band-cut filter");
	fft_bcf->add_option("--fftbcf", argument, "Transform image using Fast Fourier Transform -> band-cut fiter. Takes 2 arguments: threshold and width.");
	fft_bcf->callback([&]() {
		CImg<unsigned char> img(source_file.c_str());

		auto fourier = std::make_shared<std::vector<std::vector<std::complex<double>>>>();

		fourier = dmimg::fft_spatial_domain(img);
		fourier = dmimg::swap_quarters_ptr(*fourier);
		dmimg::bandcut_filter(*fourier, argument[0], argument[1]);
		img = dmimg::fft_inverse_2D_img(*fourier);
		//img = dmimg::image_from_complex(*fourier);

		img.save(output_file.c_str());
		});
	//	
	//---------------------------------------------------------

	// quiet mode of delivering exceptions, window will not be launched
	cimg::exception_mode(0);
	try {

		app.parse(argc, argv);

	}
	catch (const CLI::ParseError & e) {
		return app.exit(e);
		return 1;
	}
	catch (std::exception & e) {
		std::cerr << "exception: " << e.what() << std::endl;
		return 2;
	}
	catch (...) {
		std::cerr << "exception\n";
		return 3;
	}

	return 0;
} //end ;)


