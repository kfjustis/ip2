#include "smoothing.h"

namespace ip2 {
bool MeanSmoothing(cv::Mat* src_image) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/MeanSmoothing): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return false;
    }
    return true;
}
} // namespace
