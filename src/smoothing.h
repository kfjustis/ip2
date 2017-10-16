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

} // namespace
