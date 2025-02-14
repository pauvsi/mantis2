/*
 * Mantis2Parameters.h
 *
 *  Created on: Jul 17, 2017
 *      Author: kevin
 */

#ifndef MANTIS2_INCLUDE_MANTIS2_MANTIS2PARAMETERS_H_
#define MANTIS2_INCLUDE_MANTIS2_MANTIS2PARAMETERS_H_

#define ULTRA_DEBUG false
#define SUPER_DEBUG true

#define BOTTOM_CAMERA_NS "bottom_camera"
#define FRONT_CAMERA_NS "front_camera"
#define BACK_CAMERA_NS "back_camera"
#define LEFT_CAMERA_NS "left_camera"
#define RIGHT_CAMERA_NS "right_camera"

#define BASE_FRAME "base_link"
#define WORLD_FRAME "world"

#define GRID_WIDTH 20
#define GRID_HEIGHT 20
#define GRID_SPACING 1.0
#define LINE_THICKNESS 0.05

//how much to split the nodes by integer
#define TEST_POINT_SPLIT_COUNT 2

#define NUMBER_RANDOM_WHITE_TEST_POINTS 60
#define NUMBER_RANDOM_SIDE_TEST_POINTS 5
#define NUMBER_RANDOM_RED_TEST_POINTS 20
#define NUMBER_RANDOM_GREEN_TEST_POINTS 20

#define RANDOMLY_SAMPLE_BRUTEFORCE_UPDATE false
// if we are randomly sampling the hypotheses for evaluation
#define RANDOM_HYPOTHESIS_SAMPLE_COUNT 3000


// the amoutn to blur the sense model by if we randomly sample or use quadrilaterals
#define SENSE_MODEL_BLUR_SIGMA 0.5


// -=-==-=-=-=-=-=-=-=-=-=-=-=
#define DO_RED_GREEN_TRICK false

#define DETECT_QUADS_WITH_ALL_IMAGES false

#define USE_BOTTOM_IN_ERROR_CALC false
#define USE_IMG2_IN_ERROR_CALC true
#define USE_IMG3_IN_ERROR_CALC true
#define USE_IMG4_IN_ERROR_CALC true
#define USE_IMG5_IN_ERROR_CALC true

// the inverse scaling factor for quad detection
#define QUAD_DETECTION_INV_SCALE 2

//use erode and dilate t fill gaps
#define FILL_CANNY_GAPS true

//minimum quad area in pixels
#define MINIMUM_CONTOUR_AREA 400

// canny thresh for quad detect
#define CANNY_HYSTERESIS 25

//gaussian blur for canny quad
#define CANNY_BLUR_SIGMA 2.5
#define CANNY_BLUR_KERNEL cv::Size(0, 0)

// polygon eps for quad approx
#define POLYGON_EPSILON 10

//minimum quadrilaterals detected to use in markov model
#define MINIMUM_QUADRILATERALS 5


//ERROR CALCULATION
// search the surrounding 5 pixels to compute a points error
#define POINT_ERROR_KERNEL_SIZE 2

#define WHITE cv::Vec3i(255, 255, 255)
#define RED cv::Vec3i(0, 0, 255)
#define GREEN cv::Vec3i(0, 255, 0)


//MARKOV
#define XY_MARKOV_RESOLUTION 0.5
// howmany nodes to pad the edges by
#define XY_MARKOV_EDGE_PADDING 1
#define XY_MARKOV_MINIMUM_PROBABLILTY 1e-16

//if during a convolution the node the convolve is from doesnt exist the node equals the current node probablility times this
#define NONEXISTANT_NODE_MULTIPLIER 0.5

//blur sigma multiplier for convolve ( this * move mag)
#define CONVOLVE_BLUR_SIGMA_MULTIPLIER 4

//scale factor for sub pixel convolution
#define CONVOLUTION_RESOLUTION_SCALE 50





#endif /* MANTIS2_INCLUDE_MANTIS2_MANTIS2PARAMETERS_H_ */
