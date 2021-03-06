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

    test_mat.release();
}

bool CheckIsSquare(int value) {
    if (value < 0) {
        std::cout << "\nError (smoothing.cpp/CheckIsSquare): " << std::endl;
        std::cout << "\tPassed value was negative" << std::endl;
        return false;
    }

    int root = (int)sqrt(value);
    if (root * root != value) {
        return false;
    }

    return true;
}

void TEST_CheckIsSquare() {
    return;
}

bool CheckPowTwo(int value) {
    if (value < 0) {
        std::cout << "\nError (smoothing.cpp/CheckPowTwo): " << std::endl;
        std::cout << "\tPassed value was negative" << std::endl;
        return false;
    } else if (value == 0) {
        std::cout << "\nError (smoothing.cpp/CheckPowTwo): " << std::endl;
        std::cout << "\tPassed value was 0" << std::endl;
        return false;
    }

    return (value & (value - 1)) == 0;
}

void TEST_CheckPowTwo() {
    return;
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

    test_mat.release();
    test_bad_mat.release();
}

bool GaussianPyramid(cv::Mat* src_image) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/GaussianPyramid): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return false;
    }

    if (CheckSliceDimensions(src_image) == false) {
        std::cout << "\nError (smoothing.cpp/GaussianPyramid): " << std::endl;
        std::cout << "\tPassed image was not square" << std::endl;
        return false;
    }

    // holds image copy to process with
    cv::Mat original = src_image->clone();
    cv::Mat temp = src_image->clone();

    // concat the extra space for the Pyramid
    int col_ext = src_image->rows / 2;
    cv::Mat extra = cv::Mat::Mat(src_image->rows, col_ext, CV_8UC1);
    cv::hconcat(*src_image,extra,*src_image);
    cv::Mat scale_arr = cv::Mat::Mat(128, src_image->cols, CV_8UC1);
    cv::Mat upsample;

    // average the image by 1 pass
    temp = MeanSmoothingReturn(&temp, 1);
    // holds the downscaled version of temp
    cv::Mat down = cv::Mat::Mat((int)round(temp.rows/2), (int)round(temp.cols/2), CV_8UC1);
    // coords to place the downsamples
    int place_row = 0, place_col = original.cols;
    // for the images at the bottom
    int pr_bot_start = 0, pc_bot_le = 0;
    int pr_bot = pr_bot_start, pc_bot = pc_bot_le;

    // loop here
    while (down.rows != 0) {
        // generate downsample from temp by removing every even row and column
        for (int i = 0; i < down.rows; ++i) {
            for (int j = 0; j < down.cols; ++j) {
                down.at<uchar>(i,j) = (uchar) temp.at<uchar>(i*2+1, j*2+1);
                if (i == down.rows-1) {
                    down.at<uchar>(i,j) = (uchar) temp.at<uchar>(i*2, j*2);
                }
                if (j == down.cols-1) {
                    down.at<uchar>(i,j) = (uchar) temp.at<uchar>(i*2, j*2);
                }
            }
        }

        // place down into source image
        for (int y = 0; y < down.rows; ++y) {
            for (int z = 0; z < down.cols; ++z) {
                if (place_col > (original.cols + down.cols-1)) {
                    place_col = original.cols;
                }
                src_image->at<uchar>(place_row, place_col) = (uchar) down.at<uchar>(y,z);
                place_col++;
                //std::cout << "py: wrote to r,c :: " << place_row << " , " << place_col << std::endl;
            }
            place_row++;
        }

        // place the images at the bottom
        if (down.cols <= 128 && pc_bot_le != src_image->cols) {
            upsample = Upsample(&down, 128);
            //std::cout << "down: wrote to r,c :: " << down.rows << " , " << down.cols << std::endl;
            //std::cout << "upsample: wrote to r,c :: " << upsample.rows << " , " << upsample.cols << std::endl;
            for (int a = 0; a < upsample.rows; ++a) {
                for (int b = 0; b < upsample.cols; ++b) {
                    if (pc_bot > (pc_bot_le + 128-1)) {
                        pc_bot = pc_bot_le;
                    }
                    scale_arr.at<uchar>(pr_bot, pc_bot) = (uchar) upsample.at<uchar>(a,b);
                    pc_bot++;
                }
                pr_bot++;
                if (pr_bot == scale_arr.rows) {
                    pr_bot = 0;
                }
            }
            pc_bot_le += 128;
        }
        // generate new temp and resize down
        temp = down.clone();
        temp = MeanSmoothingReturn(&temp, 1);
        down = cv::Mat::Mat(temp.rows/2, temp.cols/2, CV_8UC1);
    }

    // concat the array
    cv::vconcat(*src_image,scale_arr,*src_image);

    cv::namedWindow("Gaussian pyramid", CV_WINDOW_AUTOSIZE);
	cv::imshow("Gaussian pyramid", *src_image);
    cv::waitKey(0);

    // TODO something here was causing free errors
    /*original.release();
    temp.release();
    extra.release();
    down.release();
    scale_arr.release();
    upsample.release();*/

    return true;
}

bool GaussianSmoothing(const cv::Mat* src_image, unsigned int sigma) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/GaussianSmoothing): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return false;
    }

    if (sigma == 0) {
        std::cout << "\nError (smoothing.cpp/GaussianSmoothing): " << std::endl;
        std::cout << "\tSigma cannot be 0" << std::endl;
        return false;
    }

    cv::Mat padded_image = PadMatrix(src_image);
    cv::Mat slice, output;
    int kernel_size = 3;
    output = src_image->clone();

    double x = 1.0;
    double y = 1.0;
    double x_2 = 1.0;
    double y_2 = 1.0;
    double sig_2 = 1.0;
    double value = 1.0;
    int temp_pixel = 0;
    int out_pixel = 0;
    for (int i = 0; i < padded_image.rows-kernel_size+1; ++i) {
        for (int j = 0; j < padded_image.cols-kernel_size+1; ++j) {
            slice = GetMatrixSlice(&padded_image, i, j, kernel_size);

            /*std::cout << "\nMessage (smoothing.cpp/GaussianSmoothing): " << std::endl;
            std::cout << "\toriginal slice: " << std::endl << slice << std::endl;*/

            // calculate kernel values and apply to slice (they have same indices)
            for (int k = 0; k < slice.rows; ++k) {
                for (int l = 0; l < slice.cols; ++l) {
                    x = (double)(k+1-2);
                    y = (double)(l+1-2);
                    x_2 = pow(x, 2.0);
                    y_2 = pow(y, 2.0);
                    sig_2 = pow((double)sigma, 2.0);
                    //std::cout << "\tx_2 was: " << x_2 << std::endl;
                    //std::cout << "\ty_2 was: " << y_2 << std::endl;
                    //std::cout << "\tsig_2 was: " << sig_2 << std::endl;
                    // this is from Assignment 2 Q&A slides
                    //value = (1.0/(2*3.14159*sig_2)) * exp(-((x_2+y_2)/(2*sig_2)));
                    value = (1.0/(2*3.14159)) * exp(-((x_2+y_2)/(2*sig_2)));
                    if (value > 255.0) {
                        value = 255.0;
                    } else if (value < 0.0) {
                        value = 0.0;
                    }
                    //std::cout << "\tvalue was: " << sig_2 << std::endl;
                    temp_pixel = (slice.at<uchar>(k,l) * value);
                    //std::cout << "\tpixel was: " << pixel << std::endl;
                    if (temp_pixel > 255) {
                        temp_pixel = 255;
                    } else if (temp_pixel < 0) {
                        temp_pixel = 0;
                    }
                    slice.at<uchar>(k,l) = (uchar) temp_pixel;
                    out_pixel = slice.at<uchar>(k,l) * 20;
                    //out_pixel = slice.at<uchar>(k,l);
                    if (out_pixel > 255) {
                        out_pixel = 255;
                    } else if (out_pixel < 0) {
                        out_pixel = 0;
                    }
                    output.at<uchar>(i+k,j+l) = (uchar) out_pixel;
                }
            }
        }
    }

    cv::namedWindow("Output image", CV_WINDOW_AUTOSIZE);
	cv::imshow("Output image", output);
    cv::waitKey(0);

    padded_image.release();
    slice.release();
    output.release();

    return true;
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

    cv::Mat slice1 = GetMatrixSlice(&test_mat,0,0,3);
    std::cout << "3x3 with ks 3: " << std::endl << test_mat << std::endl;

    cv::Mat pad_test_mat = PadMatrix(&test_mat);
    std::cout << "pad_test_mat: " << std::endl;
    std::cout << pad_test_mat << std::endl;

    for (int i = 0; i < test_mat.rows; ++i) {
        for (int j = 0; j < test_mat.cols; ++j) {
            cv::Mat slice = GetMatrixSlice(&pad_test_mat,i,j,3);
            std::cout << "\nslice at " << i << "," << j << std::endl;
            std::cout << slice << std::endl;
            slice.release();
        }
    }

    test_mat.release();
    slice1.release();
    pad_test_mat.release();
}

bool ImageSharpen(const cv::Mat* src_image, unsigned int data) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/ImageSharpen): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return false;
    }

    // setup the convolution kernel
    cv::Mat kernel = cv::Mat::Mat(3, 3, CV_8UC1);

    /* 0 1 0
       1 -4 1
       0 1 0 */
    kernel.at<uchar>(0,0) = (uchar) 0;
    kernel.at<uchar>(0,1) = (uchar) 1;
    kernel.at<uchar>(0,2) = (uchar) 0;
    kernel.at<uchar>(1,0) = (uchar) 1;
    kernel.at<uchar>(1,1) = (uchar) -4;
    kernel.at<uchar>(1,2) = (uchar) 1;
    kernel.at<uchar>(2,0) = (uchar) 0;
    kernel.at<uchar>(2,1) = (uchar) 1;
    kernel.at<uchar>(2,2) = (uchar) 0;

    // the original matrix
    cv::Mat original = src_image->clone();
    // matrix after the filter is applied
    cv::Mat after_filter = src_image->clone();
    // matrix mask after subtracting the average
    cv::Mat original_mask = src_image->clone();
    // matrix after the filtered matrix is subtracted from the original matrix (the result)
    cv::Mat output = src_image->clone();

    // get the averaged image
    after_filter = MeanSmoothingReturn(&original, data);
    if (after_filter.empty()) {
        std::cout << "\nError (smoothing.cpp/ImageSharpen): " << std::endl;
        std::cout << "\tFailed to average original image" << std::endl;
        return false;
    }

    int first_val = 0, second_val = 0;
    for (int i = 0; i < original_mask.rows; ++i) {
        for (int j = 0; j < original_mask.cols; ++j) {
            first_val = (int) 4 * (original.at<uchar>(i,j) - after_filter.at<uchar>(i,j));
            if (first_val < 0) {
                first_val = 0;
            } else if (first_val > 255) {
                first_val = 255;
            }
            original_mask.at<uchar>(i,j) = first_val;
            second_val = original.at<uchar>(i,j) + original_mask.at<uchar>(i,j);
            if (second_val < 0) {
                second_val = 0;
            } else if (second_val > 255) {
                second_val = 255;
            }
            output.at<uchar>(i,j) = second_val;
        }
    }

    cv::namedWindow("Output image", CV_WINDOW_AUTOSIZE);
	cv::imshow("Output image", output);
    cv::waitKey(0);

    kernel.release();
    original.release();
    after_filter.release();
    output.release();

    return true;
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

    padded_image.release();
    slice.release();
    output.release();

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

cv::Mat MeanSmoothingReturn(const cv::Mat* src_image, unsigned int iterations) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/MeanSmoothingReturn): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return cv::Mat();
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
                    std::cout << "\nError (smoothing.cpp/MeanSmoothingReturn): " << std::endl;
                    std::cout << "\tFailed to average during processing" << std::endl;
                    return cv::Mat();
                }
                //std::cout << "average: " << average << std::endl;
                output.at<uchar>(i,j) = (uchar) average;
            }
        }
        // setup padded_image for next iteration
        padded_image = PadMatrix(&output);
    }

    padded_image.release();
    slice.release();

    return output;
}

int MedianMatrix(const cv::Mat* src) {
    if (src == NULL) {
        std::cout << "\nError (smoothing.cpp/MedianMatrix): " << std::endl;
        std::cout << "\tPassed matrix was null" << std::endl;
        return -1;
    }

    /*if (CheckSliceDimensions(src) == false) {
        std::cout << "\nError (smoothing.cpp/MedianMatrix): " << std::endl;
        std::cout << "\tPassed matrix was not square" << std::endl;
        return -1;
    }*/

    // reshape the slice to be 1D matrix
    cv::Mat sort_holder = src->clone();
    sort_holder = sort_holder.reshape(0,1);

    /*std::cout << "\nMessage (smoothing.cpp/MedianMatrix): " << std::endl;
    std::cout << "Re-shaped slice:" << std::endl;
    std::cout << sort_holder << std::endl;*/

    cv::sort(sort_holder, sort_holder, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);

    /*std::cout << "\nMessage (smoothing.cpp/MedianMatrix): " << std::endl;
    std::cout << "After sorting:" << std::endl;
    std::cout << sort_holder << std::endl;*/

    int mid_idx = 0;
    int median = -1;
    if (sort_holder.cols % 2 == 0) {
        // do even case
        mid_idx = sort_holder.cols/2 - 1;
        /*std::cout << "\nMessage (smoothing.cpp/MedianMatrix): " << std::endl;
        std::cout << "Middle even index: " << mid_idx << std::endl;*/
        median = (sort_holder.at<uchar>(mid_idx) + sort_holder.at<uchar>(mid_idx+1))/2;
        /*std::cout << "\nMessage (smoothing.cpp/MedianMatrix): " << std::endl;
        std::cout << "Median value: " << median << std::endl;*/
    } else {
        mid_idx = (int) (round(sort_holder.cols/2.0) - 1);
        /*std::cout << "\nMessage (smoothing.cpp/MedianMatrix): " << std::endl;
        std::cout << "Middle index: " << mid_idx << std::endl;*/
        median = sort_holder.at<uchar>(mid_idx);
        /*std::cout << "\nMessage (smoothing.cpp/MedianMatrix): " << std::endl;
        std::cout << "Median value: " << median << std::endl;*/
    }

    sort_holder.release();

    return median;
}

void TEST_MedianMatrix() {
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

    int median = MedianMatrix(&test_mat);
    if (median == 5) {
        std::cout << "\nTest passed (smoothing.cpp/TEST_MedianMatrix)" << std::endl;
        std::cout << "\tTest passed odd case." << std::endl;
    } else {
        std::cout << "\nTest failed (smoothing.cpp/TEST_MedianMatrix)" << std::endl;
        std::cout << "\tTest failed odd case." << std::endl;
    }

    test_mat.release();

    cv::Mat test_mat2 = cv::Mat::Mat(1, 4, CV_8UC1);

    /* 4 2 4 5 */
    test_mat2.at<uchar>(0, 0) = (uchar) 4;
    test_mat2.at<uchar>(0, 1) = (uchar) 2;
    test_mat2.at<uchar>(0, 2) = (uchar) 4;
    test_mat2.at<uchar>(0, 3) = (uchar) 5;

    median = MedianMatrix(&test_mat2);
    if (median == 4) {
        std::cout << "\nTest passed (smoothing.cpp/TEST_MedianMatrix)" << std::endl;
        std::cout << "\tTest passed even case." << std::endl;
    } else {
        std::cout << "\nTest failed (smoothing.cpp/TEST_MedianMatrix)" << std::endl;
        std::cout << "\tTest failed even case." << std::endl;
    }

    test_mat2.release();
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

    /*std::cout << "\nMedianSmoothing before and after:" << std::endl;
    std::cout << *src_image << std::endl << std::endl;
    std::cout << output << std::endl;*/

    cv::namedWindow("Output image", CV_WINDOW_AUTOSIZE);
	cv::imshow("Output image", output);
    cv::waitKey(0);

    padded_image.release();
    slice.release();
    output.release();

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

    new_image.release();

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

cv::Mat Upsample(const cv::Mat* src_image, int target_size) {
    if (src_image == NULL) {
        std::cout << "\nError (smoothing.cpp/Upsample): " << std::endl;
        std::cout << "\tPassed image was null" << std::endl;
        return cv::Mat();
    }

    if (CheckSliceDimensions(src_image) == false) {
        std::cout << "\nError (smoothing.cpp/Upsample): " << std::endl;
        std::cout << "\tPassed image was not square" << std::endl;
        return cv::Mat();
    }

    if (CheckPowTwo(target_size) == false) {
        std::cout << "\nError (smoothing.cpp/Upsample): " << std::endl;
        std::cout << "\tPassed target size was not a valid power of two" << std::endl;
        return cv::Mat();
    }

    if (src_image->cols == target_size) {
        //std::cout << "\nError (smoothing.cpp/Upsample): " << std::endl;
        //std::cout << "\tNo upsampling required due to same target dimensions" << std::endl;
        /*cv::namedWindow("No sample", CV_WINDOW_AUTOSIZE);
        cv::imshow("No sample", *src_image);
        cv::waitKey(0);*/
        return src_image->clone();
    }

    cv::Mat original = src_image->clone();
    cv::Mat upsized = cv::Mat::Mat(original.rows*2, original.rows*2, CV_8UC1);
    cv::Mat output;
    cv::Mat slice;

    // transfer pixels from original to upsize until targer size
    while (output.cols != target_size) {
        for (int i = 0; i < original.rows; ++i) {
            for (int j = 0; j < original.cols; ++j) {
                // pass TL coord of current quad in upsampled mat to be sliced and filled
                int ur = i*2, uc = j*2;
                slice = cv::Mat::Mat(2,2, CV_8UC1, (double) original.at<uchar>(i,j));

                // place new values in upsides (TL, TR, BL, BR)
                upsized.at<uchar>(i*2,j*2) = slice.at<uchar>(0,0);
                upsized.at<uchar>(i*2,j*2+1) = slice.at<uchar>(0,1);
                upsized.at<uchar>(i*2+1,j*2) = slice.at<uchar>(1,0);
                upsized.at<uchar>(i*2+1,j*2+1) = slice.at<uchar>(1,1);
            }
        }
        output = upsized.clone();
        original = upsized.clone();
        upsized = cv::Mat::Mat(original.rows*2, original.rows*2, CV_8UC1);
    }

    /*cv::namedWindow("Upsample", CV_WINDOW_AUTOSIZE);
    cv::imshow("Upsample", output);
    cv::waitKey(0);*/

    original.release();
    upsized.release();
    slice.release();
    return output;
}

void TEST_Upsample(const cv::Mat* src_image, int target_size) {
    cv::Mat test = Upsample(src_image, target_size);
    if (test.empty()) {
        std::cout << "\nError (smoothing.cpp/TEST_Upsample): " << std::endl;
        std::cout << "\tReturned upsample was empty" << std::endl;
        test.release();
    } else {
        test.release();
        std::cout << "\nTest passed (smoothing.cpp/TEST_Upsample): " << std::endl;
    }
}
} // namespace
