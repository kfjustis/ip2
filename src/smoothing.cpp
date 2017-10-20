#include "smoothing.h"

namespace ip2 {
int AverageMatrix(const cv::Mat* src) {
    if (src == NULL) {
        std::cout << "\nError (smoothing.cpp/AverageMatrix): " << std::endl;
        std::cout << "\tPassed matrix was null" << std::endl;
        return -1;
    }

    if (CheckSliceDimensions(src) == false) {
        std::cout << "\nError (smoothing.cpp/AverageMatrix): " << std::endl;
        std::cout << "\tPassed matrix was not square" << std::endl;
        return -1;
    }

    // sum the elements
    int sum = 0;
    for (int i = 0; i < src->rows; ++i) {
        for (int j = 0; j < src->cols; ++j) {
            sum += (uchar) src->at<uchar>(i,j);
        }
    }

    // calculate average
    double total = src->rows * src->cols;
    double average = round((double)(sum / total));
    //std::cout << "average was: " << average << std::endl;

    // replace center value with average
    //src->at<uchar>((int) src->rows/2, (int) src->cols/2) = (uchar) average;

    return average;
}

void TEST_AverageMatrix() {
    cv::Mat test_mat = cv::Mat::Mat(3, 3, CV_8UC1);

    /* 5 3 6
       2 1 9
       8 4 7 */
    test_mat.at<uchar>(0,0) = (uchar) 5;
    test_mat.at<uchar>(0,1) = (uchar) 3;
    test_mat.at<uchar>(0,2) = (uchar) 6;
    test_mat.at<uchar>(1,0) = (uchar) 2;
    test_mat.at<uchar>(1,1) = (uchar) 1;
    test_mat.at<uchar>(1,2) = (uchar) 9;
    test_mat.at<uchar>(2,0) = (uchar) 8;
    test_mat.at<uchar>(2,1) = (uchar) 4;
    test_mat.at<uchar>(2,2) = (uchar) 7;

    if (AverageMatrix(&test_mat) == false) {
        std::cout << "\nError (smoothing.cpp/TEST_AverageMatrix)" << std::endl;
        std::cout << "\tTest failed in AverageMatrix fn" << std::endl;
        return;
    }

    if (test_mat.at<uchar>(1,1) == (uchar) 5) {
        std::cout << "Test passed (smoothing.cpp/TEST_AverageMatrix)" << std::endl;
        std::cout << test_mat << std::endl;
        return;
    } else {
        std::cout << "\nError (smoothing.cpp/TEST_AverageMatrix)" << std::endl;
        std::cout << "\tTest failed. Value was: " << test_mat.at<uchar>(1,1) << std::endl;
        return;
    }
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

    if (row + kernel_size-1 > src_image->rows || col + kernel_size-1 > src_image->cols) {
        std::cout << "\nError (smoothing.cpp/GetMatrixSlice): " << std::endl;
        std::cout << "\tRequested coordinate cannot generate a full slice" << std::endl;
        return cv::Mat();
    }

    // generate slice matrix and load data from source image
    cv::Mat mat_slice = cv::Mat::Mat(kernel_size, kernel_size, CV_8UC1);

    if (kernel_size * kernel_size == src_image->rows * src_image-> cols) {
        mat_slice = src_image->clone();
        return mat_slice;
    } else {
        /*std::cout << "\nMessage (smoothing.cpp/GetMatrixSlice): " << std::endl;
        std::cout << "\tkernel_size: " << kernel_size << std::endl;
        std::cout << "\tsrc_rows: " << src_image->rows << std::endl;
        std::cout << "\tsrc_cols: " << src_image->cols << std::endl;
        return cv::Mat();*/
    }

    /* for  0,1 with k_size 2
     * want r c want k j
            0,1      0,0
            0,2      0,1
            1,1      1,0
            1,2      1,1

       for 0,0 with k_size 3
     * want r c want k j
            0,0
            0,1
            0,2
            1,0
            1,1
            1,2
            2,0
            2,1
            2,2*/
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

    cv::Mat slice = GetMatrixSlice(&test_mat,0,0,3);
    std::cout << "3x3 with ks 3: " << std::endl << test_mat << std::endl;

    cv::Mat pad_test_mat = PadMatrix(&test_mat);
    std::cout << "pad_test_mat: " << std::endl;
    std::cout << pad_test_mat << std::endl;

    for (int i = 0; i < test_mat.rows; ++i) {
        for (int j = 0; j < test_mat.cols; ++j) {
            cv::Mat slice = GetMatrixSlice(&pad_test_mat,i,j,3);
            std::cout << "\nslice at " << i << "," << j << std::endl;
            std::cout << slice << std::endl;
        }
    }
}

bool MeanSmoothing(const cv::Mat* src_image, unsigned int iterations) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/MeanSmoothing): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return false;
    }

    /* for each index of the matrix (the inner part of the padded matrix)
        grab a slice the size of the kernel
        perform the average calculation and replace the data
        write the slice into a new matrix?
        display the image
        wait key
    */

    cv::Mat padded_image = PadMatrix(src_image);
    cv::Mat slice, output;
    int kernel_size = 3;
    int average = -2;
    output = src_image->clone();

    /*std::cout << "\nMessage (smoothing.cpp/MeanSmoothing): " << std::endl;
    std::cout << "\tRequested loop iterations: " << iterations << std::endl;
    std::cout << "\tStart matrix:" << std::endl;
    std::cout << *src_image << std::endl;*/

    for (unsigned int loop = 0; loop < iterations; ++loop) {
        // i,j always points to the upper left of the current slice with this indexing
        /*std::cout << "\nMessage (smoothing.cpp/MeanSmoothing): " << std::endl;
        std::cout << "\tPadded image before slicing:" << std::endl;
        std::cout << padded_image << std::endl;*/
        for (int i = 0; i < padded_image.rows-kernel_size+1; ++i) {
            for (int j = 0; j < padded_image.cols-kernel_size+1; ++j) {
                slice = GetMatrixSlice(&padded_image, i, j, kernel_size);
                if ((average = AverageMatrix(&slice)) == -1) {
                    std::cout << "\nError (smoothing.cpp/MeanSmoothing): " << std::endl;
                    std::cout << "\tFailed to average during processing" << std::endl;
                    return false;
                }
                //std::cout << "average: " << average << std::endl;
                output.at<uchar>(i,j) = (uchar) average;
            }
        }
        // setup padded_image for next iteration
        padded_image = PadMatrix(&output);
    }

    /*std::cout << "\nMeanSmoothing before and after:" << std::endl;
    std::cout << *src_image << std::endl << std::endl;
    std::cout << output << std::endl;*/

    cv::namedWindow("Output image", CV_WINDOW_AUTOSIZE);
	cv::imshow("Output image", output);
    cv::waitKey(0);

    return true;
}

void TEST_MeanSmoothing() {
    cv::Mat test_mat = cv::Mat::Mat(6, 6, CV_8UC1);

    /* 1 4 0 1 3 1
       2 2 4 2 2 3
       1 0 1 0 1 0
       1 2 1 0 2 2
       2 5 3 1 2 5
       1 1 4 2 3 0 */
    test_mat.at<uchar>(0,0) = (uchar) 1;
    test_mat.at<uchar>(0,1) = (uchar) 4;
    test_mat.at<uchar>(0,2) = (uchar) 0;
    test_mat.at<uchar>(0,3) = (uchar) 1;
    test_mat.at<uchar>(0,4) = (uchar) 3;
    test_mat.at<uchar>(0,5) = (uchar) 1;

    test_mat.at<uchar>(1,0) = (uchar) 2;
    test_mat.at<uchar>(1,1) = (uchar) 2;
    test_mat.at<uchar>(1,2) = (uchar) 4;
    test_mat.at<uchar>(1,3) = (uchar) 2;
    test_mat.at<uchar>(1,4) = (uchar) 2;
    test_mat.at<uchar>(1,5) = (uchar) 3;

    test_mat.at<uchar>(2,0) = (uchar) 1;
    test_mat.at<uchar>(2,1) = (uchar) 0;
    test_mat.at<uchar>(2,2) = (uchar) 1;
    test_mat.at<uchar>(2,3) = (uchar) 0;
    test_mat.at<uchar>(2,4) = (uchar) 1;
    test_mat.at<uchar>(2,5) = (uchar) 0;

    test_mat.at<uchar>(3,0) = (uchar) 1;
    test_mat.at<uchar>(3,1) = (uchar) 2;
    test_mat.at<uchar>(3,2) = (uchar) 1;
    test_mat.at<uchar>(3,3) = (uchar) 0;
    test_mat.at<uchar>(3,4) = (uchar) 2;
    test_mat.at<uchar>(3,5) = (uchar) 2;

    test_mat.at<uchar>(4,0) = (uchar) 2;
    test_mat.at<uchar>(4,1) = (uchar) 5;
    test_mat.at<uchar>(4,2) = (uchar) 3;
    test_mat.at<uchar>(4,3) = (uchar) 1;
    test_mat.at<uchar>(4,4) = (uchar) 2;
    test_mat.at<uchar>(4,5) = (uchar) 5;

    test_mat.at<uchar>(5,0) = (uchar) 1;
    test_mat.at<uchar>(5,1) = (uchar) 1;
    test_mat.at<uchar>(5,2) = (uchar) 4;
    test_mat.at<uchar>(5,3) = (uchar) 2;
    test_mat.at<uchar>(5,4) = (uchar) 3;
    test_mat.at<uchar>(5,5) = (uchar) 0;

    cv::Mat test_padded = PadMatrix(&test_mat);
    if (test_padded.empty() == true) {
        std::cout << "\nError (smoothing.cpp/TEST_MeanSmoothing)" << std::endl;
        std::cout << "\tTest failed. Could not generate padded matrix" << std::endl;
        test_mat.release();
        test_padded.release();
        return;
    } else {
        std::cout << "\n1 iteration:" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        MeanSmoothing(&test_mat, 1);
        std::cout << "\n\n\n10 iterations:" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        MeanSmoothing(&test_mat, 10);
        std::cout << "\nTest passed (smoothing.cpp/TEST_MeanSmoothing)" << std::endl;
        std::cout << "\tTest passed." << std::endl;
    }

    // release
    test_mat.release();
    test_padded.release();
}

int MedianMatrix(const cv::Mat* src) {
    return -1;
}

bool MedianSmoothing(const cv::Mat* src_image, unsigned int iterations) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/MedianSmoothing): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return false;
    }

    /* for each index of the matrix (the inner part of the padded matrix)
        grab a slice the size of the kernel
        determine occurrences and choose the max to replace the center
        write the slice into a new matrix?
        display the image
        wait key
    */

    cv::Mat padded_image = PadMatrix(src_image);
    cv::Mat slice, output;
    int kernel_size = 3;
    int median = -2;
    output = src_image->clone();

    /*std::cout << "\nMessage (smoothing.cpp/MedianSmoothing): " << std::endl;
    std::cout << "\tRequested loop iterations: " << iterations << std::endl;
    std::cout << "\tStart matrix:" << std::endl;
    std::cout << *src_image << std::endl;*/

    for (unsigned int loop = 0; loop < iterations; ++loop) {
        // i,j always points to the upper left of the current slice with this indexing
        for (int i = 0; i < padded_image.rows-kernel_size+1; ++i) {
            for (int j = 0; j < padded_image.cols-kernel_size+1; ++j) {
                slice = GetMatrixSlice(&padded_image, i, j, kernel_size);
                /*if ((average = AverageMatrix(&slice)) == -1) {
                    std::cout << "\nError (smoothing.cpp/MeanSmoothing): " << std::endl;
                    std::cout << "\tFailed to average during processing" << std::endl;
                    return false;
                }*/
                //std::cout << "average: " << average << std::endl;
                if ((median = MedianMatrix(&slice)) == -1) {
                    std::cout << "\nError (smoothing.cpp/MedianSmoothing): " << std::endl;
                    std::cout << "\tFailed to get median during processing" << std::endl;
                    return false;
                }
                output.at<uchar>(i,j) = (uchar) median;
            }
        }
        // setup padded_image for next iteration
        padded_image = PadMatrix(&output);
    }

    std::cout << "\nMedianSmoothing before and after:" << std::endl;
    std::cout << *src_image << std::endl << std::endl;
    std::cout << output << std::endl;

    /*cv::namedWindow("Output image", CV_WINDOW_AUTOSIZE);
	cv::imshow("Output image", output);
    cv::waitKey(0);*/

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
                padded_image.at<uchar>(row, col) = (uchar) src_image->at<uchar>(row-1, col-1);
            }
        }
    }

    //std::cout << "\nOld rows: " << src_image->rows << std::endl;
    //std::cout << "Old cols: " << src_image->cols << std::endl;
    //std::cout << "New rows: " << padded_image.rows << std::endl;
    //std::cout << "New cols: " << padded_image.cols << std::endl;
    //std::cout << padded_image << std::endl;

    return padded_image;
}

void TEST_PadMatrix() {
    std::cout << "\n===================" << std::endl;
    std::cout << "TEST_PadMatrix" << std::endl;
    std::cout << "===================" << std::endl;

    cv::Mat test_mat2 = cv::Mat::Mat(2, 2, CV_8UC1);
    /* 1 2
       3 4 */
    test_mat2.at<uchar>(0,0) = (uchar) 1;
    test_mat2.at<uchar>(0,1) = (uchar) 2;
    test_mat2.at<uchar>(1,0) = (uchar) 3;
    test_mat2.at<uchar>(1,1) = (uchar) 4;

    std::cout << "test_mat 2x2: " << std::endl;
    std::cout << test_mat2 << std::endl;

    cv::Mat pad2 = PadMatrix(&test_mat2);
    std::cout << "padded test_mat 2x2: " << std::endl;
    std::cout << pad2 << std::endl;

    cv::Mat test_mat3 = cv::Mat::Mat(3, 3, CV_8UC1);
    /* 1 2 3
       4 2 4
       5 7 9 */
    test_mat3.at<uchar>(0,0) = (uchar) 1;
    test_mat3.at<uchar>(0,1) = (uchar) 2;
    test_mat3.at<uchar>(0,2) = (uchar) 3;
    test_mat3.at<uchar>(1,0) = (uchar) 4;
    test_mat3.at<uchar>(1,1) = (uchar) 2;
    test_mat3.at<uchar>(1,2) = (uchar) 4;
    test_mat3.at<uchar>(2,0) = (uchar) 5;
    test_mat3.at<uchar>(2,1) = (uchar) 7;
    test_mat3.at<uchar>(2,2) = (uchar) 9;

    std::cout << "\ntest_mat 3x3: " << std::endl;
    std::cout << test_mat3 << std::endl;

    cv::Mat pad3 = PadMatrix(&test_mat3);
    std::cout << "padded test_mat 3x3: " << std::endl;
    std::cout << pad3 << std::endl;

    // release
    test_mat2.release();
    pad2.release();
    test_mat3.release();
    pad3.release();

}
} // namespace
