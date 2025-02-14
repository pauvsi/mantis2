/*
 * HypothesisError.h
 *
 *  Created on: Jul 18, 2017
 *      Author: kevin
 */

#ifndef MANTIS2_INCLUDE_MANTIS2_HYPOTHESISERROR_H_
#define MANTIS2_INCLUDE_MANTIS2_HYPOTHESISERROR_H_


#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/video.hpp"

#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/message_filter.h>
#include <tf2_ros/transform_listener.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <tf/tf.h>
#include <tf/tfMessage.h>

#include "mantis2/Mantis2Parameters.h"

#include "mantis2/Mantis2Types.h"

#include <algorithm>
#include <random>

auto engine = std::default_random_engine{}; // for random shuffling of vector


void evaluateSidePoints(MantisImage& img, int& projections, double& error)
{
	for(std::vector<tf::Vector3>::iterator it = side_line_test_points.begin(); it != side_line_test_points.begin() + NUMBER_RANDOM_SIDE_TEST_POINTS; it++)
	{
		error += img.computePointError(*it, WHITE, projections);
	}
}

void evaluateWhitePoints(MantisImage& img, int& projections, double& error)
{
	for(std::vector<tf::Vector3>::iterator it = white_test_points.begin(); it != white_test_points.begin() + NUMBER_RANDOM_WHITE_TEST_POINTS; it++)
	{
		error += img.computePointError(*it, WHITE, projections);
	}
}

void evaluateGreenPoints(MantisImage& img, int& projections, double& error)
{
	for(std::vector<tf::Vector3>::iterator it = green_test_points.begin(); it != green_test_points.begin() + NUMBER_RANDOM_GREEN_TEST_POINTS; it++)
	{
		error += img.computePointError(*it, GREEN, projections);
	}
}

void evaluateRedPoints(MantisImage& img, int& projections, double& error)
{
	for(std::vector<tf::Vector3>::iterator it = red_test_points.begin(); it != red_test_points.begin() + NUMBER_RANDOM_RED_TEST_POINTS; it++)
	{
		error += img.computePointError(*it, RED, projections);
	}
}

void evaluateMantisImage(MantisImage& img, int& projections, double& error, bool colorOnly = false){
	evaluateRedPoints(img, projections, error);
	evaluateGreenPoints(img, projections, error);

	if(!colorOnly){
		evaluateWhitePoints(img, projections, error);
		evaluateSidePoints(img, projections, error);
	}
}

void evaluateBaseFrameHypothesis(BaseFrameHypothesis& hyp, bool colorOnly = false){

	int projections = 0;
	double error = 0; // this will be per pixel

	// set up the local hypothesis for each mantis camera
#if USE_BOTTOM_IN_ERROR_CALC
	hyp.measurement->bottom_img.thisHyp.setW2C(hyp.getW2B() * hyp.measurement->bottom_img.b2c);
#endif
#if USE_IMG2_IN_ERROR_CALC
	hyp.measurement->img2.thisHyp.setW2C(hyp.getW2B() * hyp.measurement->img2.b2c);
#endif
#if USE_IMG3_IN_ERROR_CALC
	hyp.measurement->img3.thisHyp.setW2C(hyp.getW2B() * hyp.measurement->img3.b2c);
#endif
#if USE_IMG4_IN_ERROR_CALC
	hyp.measurement->img4.thisHyp.setW2C(hyp.getW2B() * hyp.measurement->img4.b2c);
#endif
#if USE_IMG5_IN_ERROR_CALC
	hyp.measurement->img5.thisHyp.setW2C(hyp.getW2B() * hyp.measurement->img5.b2c);
#endif

#if USE_BOTTOM_IN_ERROR_CALC
	evaluateMantisImage(hyp.measurement->bottom_img, projections, error, colorOnly);
#endif
#if USE_IMG2_IN_ERROR_CALC
	evaluateMantisImage(hyp.measurement->img2, projections, error, colorOnly);
#endif
#if USE_IMG3_IN_ERROR_CALC
	evaluateMantisImage(hyp.measurement->img3, projections, error, colorOnly);
#endif
#if USE_IMG4_IN_ERROR_CALC
	evaluateMantisImage(hyp.measurement->img4, projections, error, colorOnly);
#endif
#if USE_IMG5_IN_ERROR_CALC
	evaluateMantisImage(hyp.measurement->img5, projections, error, colorOnly);
#endif

	//ROS_DEBUG_STREAM("evaluated hypothesis. projection count: " << projections);


	//TODO add projection bias
	if(projections == 0)
	{
		hyp.error = DBL_MAX;
	}
	else
	{
		hyp.error = error / (double)projections;

		//ROS_DEBUG_STREAM("ppe: " << hyp.error);
	}

}

void evaluateBaseFrameHypotheses(std::vector<BaseFrameHypothesis>& hyps, bool colorOnly = false, bool randomlySample = false)
{
	//shuffle the test points for a random sample of them
	std::shuffle(std::begin(white_test_points), std::end(white_test_points), engine);
	std::shuffle(std::begin(red_test_points), std::end(red_test_points), engine);
	std::shuffle(std::begin(green_test_points), std::end(green_test_points), engine);
	std::shuffle(std::begin(side_line_test_points), std::end(side_line_test_points), engine);

	int numberSamples = 0;
	if(randomlySample)
	{
		ROS_DEBUG("using random samples");
		if(RANDOM_HYPOTHESIS_SAMPLE_COUNT > hyps.size())
		{
			numberSamples = hyps.size();
		}
		else
		{
			numberSamples = RANDOM_HYPOTHESIS_SAMPLE_COUNT;
		}

		//shuffle the hyps
		std::shuffle(std::begin(hyps), std::end(hyps), engine);
	}
	else
	{
		numberSamples = hyps.size();
	}

	for(std::vector<BaseFrameHypothesis>::iterator it = hyps.begin(); it != hyps.begin() + numberSamples; it++)
	{
		evaluateBaseFrameHypothesis(*it, colorOnly);
	}
}

void visualizeHypothesis(BaseFrameHypothesis hyp, MantisImage img)
{
	cv::Mat final = img.img;

	img.thisHyp.setW2C(hyp.getW2B() * img.b2c);

	for(std::vector<tf::Vector3>::iterator it = white_test_points.begin(); it != white_test_points.begin() + NUMBER_RANDOM_WHITE_TEST_POINTS; it++)
	{
		cv::drawMarker(final, img.thisHyp.projectPoint(*it, img.K), cv::Scalar(0, 255, 255), cv::MarkerTypes::MARKER_CROSS);
	}

	for(std::vector<tf::Vector3>::iterator it = green_test_points.begin(); it != green_test_points.begin() + NUMBER_RANDOM_GREEN_TEST_POINTS; it++)
	{
		cv::drawMarker(final, img.thisHyp.projectPoint(*it, img.K), cv::Scalar(0, 255, 0), cv::MarkerTypes::MARKER_CROSS);
	}

	for(std::vector<tf::Vector3>::iterator it = red_test_points.begin(); it != red_test_points.begin() + NUMBER_RANDOM_RED_TEST_POINTS; it++)
	{
		cv::drawMarker(final, img.thisHyp.projectPoint(*it, img.K), cv::Scalar(0, 0, 255), cv::MarkerTypes::MARKER_CROSS);
	}

	for(std::vector<tf::Vector3>::iterator it = side_line_test_points.begin(); it != side_line_test_points.begin() + NUMBER_RANDOM_SIDE_TEST_POINTS; it++)
	{
		cv::drawMarker(final, img.thisHyp.projectPoint(*it, img.K), cv::Scalar(0, 255, 255), cv::MarkerTypes::MARKER_SQUARE);
	}

	cv::imshow("hypo estimate", final);
	cv::waitKey(30);
}

#endif /* MANTIS2_INCLUDE_MANTIS2_HYPOTHESISERROR_H_ */
