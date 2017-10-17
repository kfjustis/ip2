#include "smoothing.h"

namespace ip2 {
bool AverageMatrix(cv::Mat* src) {
    if (src == NULL) {
        std::cout << "\nError (smoothing.cpp/AverageMatrix): " << std::endl;
        std::cout << "\tPassed matrix was null" << std::endl;
    }

    //if ()

    return true;
}

bool CheckSliceDimensions(const cv::Mat* src) {
    if (src == NULL) {
        std::cout << "\nError (smoothing.cpp/CheckSliceDimensions): " << std::endl;
        std::cout << "\tPassed matrix was null" << std::endl;
    }

    int dim_sqr = (int)(src->rows * src->cols);
    int root = (int)sqrt(dim_sqr);
    if (root != src->rows || root != src->cols) {
        return false;
    }

    return true;
}

void TEST_CheckSliceDimensions() {
    cv::Mat test_mat = cv::Mat::Mat(3, 3, CV_8UC1);

    /* 1 2 3
       4 2 4
       5 7 9 */
    test_mat.at<uchar>(0,0) = (uchar) 1;
    test_mat.at<uchar>(0,1) = (uchar) 2;
    test_mat.at<uchar>(0,2) = (uchar) 3;
    test_mat.at<uchar>(1,0) = (uchar) 4;
    test_mat.at<uchar>(1,1) = (uchar) 2;
    test_mat.at<uchar>(1,2) = (uchar) 4;
    test_mat.at<uchar>(2,0) = (uchar) 5;
    test_mat.at<uchar>(2,1) = (uchar) 7;
    test_mat.at<uchar>(2,2) = (uchar) 9;

    cv::Mat test_bad_mat = cv::Mat::Mat(2, 1, CV_8UC1);
    test_bad_mat.at<uchar>(0,0) = (uchar) 1;
    test_bad_mat.at<uchar>(1,0) = (uchar) 3;

    std::cout << "test_mat passes: " << CheckSliceDimensions(&test_mat) << std::endl;
    std::cout << "test_bad_mat passes: " << CheckSliceDimensions(&test_bad_mat) << std::endl;
}

cv::Mat GetMatrixSlice(const cv::Mat* src_image, int row, int col, int kernel_size) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/GetMatrixSlice): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return cv::Mat();
    }

    // check coordinates and kernel size
    if (row > src_image->rows || col > src_image->cols) {
        std::cout << "\nError (smoothing.cpp/GetMatrixSlice): " << std::endl;
        std::cout << "\tRequested coordinates out of bounds" << std::endl;
        return cv::Mat();
    }

    if (kernel_size <= 0) {
        std::cout << "\nError (smoothing.cpp/GetMatrixSlice): " << std::endl;
        std::cout << "\tKernel size must be greater than 0" << std::endl;
        return cv::Mat();
    }

    if (kernel_size * kernel_size > src_image->rows * src_image->cols) {
        std::cout << "\nError (smoothing.cpp/GetMatrixSlice): " << std::endl;
        std::cout << "\tKernel size larger than source size" << std::endl;
        return cv::Mat();
    }

    if (row + kernel_size > src_image->rows || col + kernel_size > src_image->cols) {
        std::cout << "\nError (smoothing.cpp/GetMatrixSlice): " << std::endl;
        std::cout << "\tRequested coordinate cannot generate a full slice" << std::endl;
        return cv::Mat();
    }

    // generate slice matrix and load data from source image
    cv::Mat mat_slice = cv::Mat::Mat(kernel_size, kernel_size, CV_8UC1);

    /* for  0,1 with k_size 2
     * want r c want k j
            0,1      0,0
            0,2      0,1
            1,1      1,0
            1,2      1,1 */
    // k,j are the row/col for the slice matrix
    int k = 0, j = 0;
    for (int r = row; r < (row + kernel_size); ++r, ++k) {
        if (k == kernel_size) {
            k = 0;
        }
        for (int c = col; c < (col + kernel_size); ++c, ++j) {
            if (j == kernel_size) {
                j = 0;
            }
            //std::cout << "r,c :: j,k -> " << r << "," << c << " :: " << j << "," << k << std::endl;
            mat_slice.at<uchar>(k,j) = (uchar) src_image->at<uchar>(r,c);
        }
    }

    return mat_slice;
}

void TEST_GetMatrixSlice() {
    std::cout << "\n===================" << std::endl;
    std::cout << "TEST_GetMatrixSlice" << std::endl;
    std::cout << "===================" << std::endl;

    cv::Mat test_mat = cv::Mat::Mat(3, 3, CV_8UC1);

    /* 1 2 3
       4 2 4
       5 7 9 */
    test_mat.at<uchar>(0,0) = (uchar) 1;
    test_mat.at<uchar>(0,1) = (uchar) 2;
    test_mat.at<uchar>(0,2) = (uchar) 3;
    test_mat.at<uchar>(1,0) = (uchar) 4;
    test_mat.at<uchar>(1,1) = (uchar) 2;
    test_mat.at<uchar>(1,2) = (uchar) 4;
    test_mat.at<uchar>(2,0) = (uchar) 5;
    test_mat.at<uchar>(2,1) = (uchar) 7;
    test_mat.at<uchar>(2,2) = (uchar) 9;

    std::cout << "test_mat: " << std::endl;
    std::cout << test_mat << std::endl;

    for (int i = 0; i < test_mat.rows; ++i) {
        for (int j = 0; j < test_mat.cols; ++j) {
            cv::Mat slice = GetMatrixSlice(&test_mat,i,j,2);
            //std::cout << "\n2x2 slice at " << i << "," << j << std::endl;
            std::cout << slice << std::endl;
        }
    }
}

bool MeanSmoothing(const cv::Mat* src_image) {
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

    // TODO check with values in slides to make sure

    /* for each index of the matrix (the inner part of the padded matrix)
        grab a slice the size of the kernel
        perform the average calculation and replace the data
        write the slice into a new matrix?
        display the image
        wait key
    */

    cv::Mat slice;
    int kernel_size = 3;
    for (int i = 0; i < padded_image.rows; ++i) {
        for (int j = 0; j < padded_image.cols; ++j) {
            slice = GetMatrixSlice(&padded_image, i, j, kernel_size);
        }
    }

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
