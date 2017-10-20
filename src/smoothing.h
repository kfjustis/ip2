# pragma once

#include <cmath>
#include <iostream>
#include <map>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace ip2 {
/*
 * Determines the average of the values in the matrix and returns it.
 * Dimensions must be square for this to work.
 * PARAMS: const cv::Mat* src - reference to the source matrix
 * RETURN: int - the resulting average from the passed matrix, -1 on failure
 */
int AverageMatrix(const cv::Mat* src);
void TEST_AverageMatrix();

/*
 * Checks that the dimensions of the slice are a perfect square.
 * PARAMS: const cv::Mat* src - reference to the source matrix
 * RETURN: bool - true on success, false on failure
 */
bool CheckSliceDimensions(const cv::Mat* src);
void TEST_CheckSliceDimensions();

/*
 * Applies Gaussian smoothing filter to the passed image.
 * PARAMS: const cv::Mat* src_image - reference to source image data as an opencv matrix
 *         unsigned int sigma - sigma value to use when calculating the gaussian kernel
 * RETURN: bool - true on success, false on failure
 */
bool GaussianSmoothing(const cv::Mat* src_image, unsigned int sigma);
void TEST_GaussianSmoothing();

/*
 * Takes a given matrix and upper left coordinate for a region slice.
 * From that coordinate, a new matrix is generated with the same
 * data as the region with dimensions equal to the kernel so that
 * operations can be performed, such as MeanSmoothing(). The resulting matrix
 * that gets returned is the region slice with the same data as the source matrix.
 * PARAMS: const cv::Mat* src_image - reference to source image data as an opencv matrix
 *         int row - the top-left row coordinate for desired slice region
 *         int col - the top-left col coordinate for desired slice region
 *         int kernel_size - the dimension of the kernel (will be squared)
 * RETURN: cv::Mat - the matrix containing the requested slice, empty matrix on failure
 */
cv::Mat GetMatrixSlice(const cv::Mat* src_image, int row, int col, int kernel_size);
void TEST_GetMatrixSlice();

/*
 * Applies mean smoothing filter to the passed image.
 * PARAMS: const cv::Mat* src_image - reference to source image data as an opencv matrix
 *         unsigned int iterations - the number of times to apply mean filter before showing image
 * RETURN: bool - true on success, false on failure
 */
bool MeanSmoothing(const cv::Mat* src_image, unsigned int iterations);
void TEST_MeanSmoothing();

/*
 * Determines the median of the values in the matrix and returns it.
 * Dimensions must be square for this to work.
 * PARAMS: const cv::Mat* src - reference to the source matrix
 * RETURN: int - the resulting median value from the passed matrix, -1 on failure
 */
int MedianMatrix(const cv::Mat* src);
void TEST_MedianMatrix();

/*
 * Applies median smoothing filter to the passed image.
 * PARAMS: const cv::Mat* src_image - reference to source image data as an opencv matrix
 *         unsigned int iterations - the number of times to apply median filter before showing image
 * RETURN: bool - true on success, false on failure
 */
bool MedianSmoothing(const cv::Mat* src_image, unsigned int iterations);
void TEST_MedianSmoothing();

/*
 * Takes a given matrix and returns the same matrix padded with zeros.
 * PARAMS: const cv::Mat* src_image - reference to souce image data as an opencv matrix
 * RETURN: a new Mat reference with padded zeros, empty matrix on failure
 */
cv::Mat PadMatrix(const cv::Mat* src_image);
void TEST_PadMatrix();

} // namespace
