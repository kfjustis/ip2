#include "smoothing.h"

int main(int argc, char** argv) {
	// check args
	if (argc != 4) {
		std::cout << "\nError (main.cpp): " << std::endl;
		std::cout << "Invalid command line args -->" << std::endl;
		std::cout << "\tUSAGE: " << argv[0] << " <image file> <operation> <data>" << std::endl;
		std::cout << "\n<operations 1-3> <data == num iterations>" << std::endl;
		std::cout << "\t1 -- Mean smoothing | 2 -- Gaussian smoothing | 3 -- Median smoothing" << std::endl;
		std::cout << "\n<operation 4> <data == average>" << std::endl;
		std::cout << "\tImage sharpening; the data param specifies the degree of average" << std::endl;
		std::cout << "\n<operation 5> <data == does nothing>" << std::endl;
		std::cout << "\tCreates a gaussian pryamid" << std::endl;
		std::cout << "\n<operation 6> <data == does nothing>" << std::endl;
		std::cout << "\tPerforms test functions when enabled" << std::endl;
		std::cout << "\n<operation> and <data> parameters should be integer values" << std::endl;
		return -1;
	}

	// load source image as grayscale
	cv::Mat src_image;
	src_image = cv::imread(argv[1], 0);
	if (src_image.data == NULL) {
		std::cout << "\nError (main.cpp): " << std::endl;
		std::cout << "Failed to load source image -->" << std::endl;
		std::cout << "\tMake sure passed file was an image with correct path" << std::endl;
		return -1;
	}

	// load the filter type
	int filter_type = atoi(argv[2]);
	unsigned int data = (unsigned int) atoi(argv[3]);

	// apply filter
	switch(filter_type) {
		case 1:
			std::cout << "\nProcessing image with mean filter at " << data << " iterations..." << std::endl;
			if (ip2::MeanSmoothing(&src_image, data) == false) {
				std::cout << "\nError (main.cpp): " << std::endl;
				std::cout << "\tMean smoothing failed" << std::endl;
				src_image.release();
				return -1;
			} else {
				std::cout << "Mean smoothing success.\n" << std::endl;
				break;
			}
		case 2:
			std::cout << "\nProcessing image with gaussian filter with sigma " << data << " ..." << std::endl;
			if (ip2::GaussianSmoothing(&src_image, data) == false) {
				std::cout << "\nError (main.cpp): " << std::endl;
				std::cout << "\tGaussian smoothing failed" << std::endl;
				src_image.release();
				return -1;
			} else {
				std::cout << "Gaussian smoothing success. \n" << std::endl;
				break;
			}
		case 3:
			std::cout << "\nProcessing image with median filter at " << data << " iterations..." << std::endl;
			if (ip2::MedianSmoothing(&src_image, data) == false) {
				std::cout << "\nError (main.cpp): " << std::endl;
				std::cout << "\tMedian smoothing failed" << std::endl;
				src_image.release();
				return -1;
			} else {
				std::cout << "\nMean smoothing success.\n" << std::endl;
				break;
			}
		case 4:
			if (ip2::ImageSharpen(&src_image, data) == false) {
				std::cout << "\nError (main.cpp): " << std::endl;
				std::cout << "\tSharpening failed" << std::endl;
				src_image.release();
				return -1;
			} else {
				std::cout << "\nSharpening success.\n" << std::endl;
				break;
			}
		case 5:
			if (ip2::GaussianPyramid(&src_image) == false) {
				std::cout << "\nError (main.cpp): " << std::endl;
				std::cout << "\tGaussian pyramid failed" << std::endl;
				src_image.release();
				return -1;
			} else {
				std::cout << "\nPyramid success.\n" << std::endl;
				break;
			}
		case 6:
			std::cout << "\nRunning tests..." << std::endl;
			//ip2::TEST_GetMatrixSlice();
			//ip2::TEST_PadMatrix();
			//ip2::TEST_CheckSliceDimensions();
			//ip2::TEST_AverageMatrix();
			//ip2::TEST_MeanSmoothing();
			//ip2::TEST_MedianMatrix();
			//ip2::TEST_GaussianSmoothing();
			ip2::TEST_Upsample(&src_image, 512);
			break;
		default:
			std::cout << "\nError (main.cpp): " << std::endl;
			std::cout << "\tInvalid filter case" << std::endl;
			src_image.release();
			return -1;
	}

	// release
	src_image.release();

	return 0;
}
