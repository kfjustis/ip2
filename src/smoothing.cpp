#include "smoothing.h"

namespace ip2 {
bool MeanSmoothing(cv::Mat* src_image) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/MeanSmoothing): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return false;
    }

    cv::Mat padded_image = PadMatrix(src_image);

    //cv::namedWindow("Output image", CV_WINDOW_AUTOSIZE);
	//cv::imshow("Output images", padded_image);
    //cv::waitKey(0);
    //cv::imwrite( "../../data/padded.tif", padded_image);

    return true;
}

cv::Mat PadMatrix(const cv::Mat* src_image) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/PadMatrix): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return cv::Mat();
    }

    // generate padded matrix
    int padded_rows = src_image->rows + 2;
    int padded_cols = src_image->cols + 2;

    cv::Mat new_image = cv::Mat::Mat(padded_rows, padded_cols, CV_8UC1);
    cv::Mat padded_image = new_image.clone();

    for (int row = 0; row < padded_image.rows; ++row) {
        for (int col = 0; col < padded_image.cols; ++col) {
            if (row == 0 || col == 0) {
                padded_image.at<uchar>(row, col) = (uchar) 0;
            } else if (row == padded_image.rows-1 || col == padded_image.cols-1) {
                padded_image.at<uchar>(row, col) = (uchar) 0;
            } else {
                padded_image.at<uchar>(row, col) = (uchar) src_image->at<uchar>(row, col);
            }
        }
    }

    std::cout << "\nOld rows: " << src_image->rows << std::endl;
    std::cout << "Old cols: " << src_image->cols << std::endl;
    std::cout << "New rows: " << padded_image.rows << std::endl;
    std::cout << "New cols: " << padded_image.cols << std::endl;
    //std::cout << padded_image << std::endl;

    return padded_image;
}
} // namespace
