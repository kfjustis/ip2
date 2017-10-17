# pragma once

#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace ip2 {
/*
 * Applies mean smoothing filter to the passed image.
 * PARAMS: cv::Mat* src_image - reference to source image data as an opencv matrix
 * RETURN: bool - true on success, false on failure
 */
bool MeanSmoothing(cv::Mat* src_image);

/*
 * Takes a given matrix and returns the same matrix padded with zeros.
 * PARAMS: const cv::Mat* src_image - reference to souce image data as an opencv matrix
 * RETURN: a new Mat reference with padded zeros, NULL on failure
 */
cv::Mat PadMatrix(const cv::Mat* src_image);

/*
 * Takes a given matrix and upper left coordinate for a region slice.
 * From that coordinate, a new matrix is generated with the same
 * data as the region with dimensions equal to the kernel so that
 * operations can be performed, such as MeanSmoothing(). The resulting matrix
 * that gets returned is the region slice with the same data as the source matrix.
 */
cv::Mat GetMatrixSlice(const cv::Mat* src_image, int row, int col, int kernel_size);
void TEST_GetMatrixSlice();

} // namespace
