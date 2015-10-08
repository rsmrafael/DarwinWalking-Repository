/*
 * VisionAnalyzer.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: Jan Carstensen
 *      Edited: Oliver Krebs
 */

#include "VisionAnalyzer.h"
#include <sstream>
#include <iomanip>

using namespace std;

VisionAnalyzer::VisionAnalyzer(bool createCsv)
{
	mCreateCsv = createCsv;
	mTotalImages = 0;

	visionAnalyzeData_t *data;
	data = new visionAnalyzeData_t;
	data->name = "ball";
	mVisionObjects.insert(make_pair<Object::ObjectType, visionAnalyzeData_t*>(Object::BALL, data));

	data = new visionAnalyzeData_t;
	data->name = "goal pole";
	mVisionObjects.insert(make_pair<Object::ObjectType, visionAnalyzeData_t*>(Object::GOAL_POLE_YELLOW, data));

	data = new visionAnalyzeData_t;
	data->name = "cyan robot";
	mVisionObjects.insert(make_pair<Object::ObjectType, visionAnalyzeData_t*>(Object::ROBOT_CYAN, data));

	data = new visionAnalyzeData_t;
	data->name = "magenta robot";
	mVisionObjects.insert(make_pair<Object::ObjectType, visionAnalyzeData_t*>(Object::ROBOT_MAGENTA, data));

	map<Object::ObjectType, visionAnalyzeData_t*>::const_iterator it;
	for( it = mVisionObjects.begin(); it != mVisionObjects.end(); ++it) {
		it->second->count = 0;
		it->second->found = 0;
		it->second->countSum = 0;
		it->second->foundSum = 0;
		it->second->falsePositive = 0;
		it->second->falseNegative = 0;
		it->second->correctPercent = 0.0;
		it->second->objects.clear();
	}
}

VisionAnalyzer::~VisionAnalyzer()
{
	Debugger::INFO("VisionAnalyzer", "Images processed: %u", mTotalImages);
	if( mCreateCsv) {
		Debugger::addMetric("img", "Images processed", (double)mTotalImages);
	}

	map<Object::ObjectType, visionAnalyzeData_t*>::const_iterator it;
	for( it = mVisionObjects.begin(); it != mVisionObjects.end(); ++it)
	{
		double fpRate = 0.0;
		double fnRate = 0.0;
		double correct = 0.0;
		double correctRate = 0.0;

		if (mTotalImages > 0) {
			fpRate = 100.0 * (double)it->second->falsePositive / (double)mTotalImages;
			fnRate = 100.0 * (double)it->second->falseNegative / (double)mTotalImages;
			correct = 100.0 * it->second->correctPercent / (double)mTotalImages;
		}
		if (it->second->count > 0) {
			correctRate = 100.0 * (double)it->second->foundSum / (double)it->second->countSum;
		}

		Debugger::INFO("VisionAnalyzer",
				"%s: annotated: %u, total found: %u, correct: %.2f %%",
				it->second->name.c_str(), it->second->countSum, it->second->foundSum, correctRate);
		Debugger::INFO("VisionAnalyzer",
				" falsePositive: %u (%.2f %%), falseNegative: %u (%.2f %%)",
				it->second->falsePositive, fpRate, it->second->falseNegative, fnRate);

		if( mCreateCsv) {
			string name = it->second->name + "s";
			Debugger::addMetric("img", name, (double)it->second->countSum);
			Debugger::addMetric("va-abs", "False positive " + name, (double)it->second->falsePositive);
			Debugger::addMetric("va-abs", "False negative " + name, (double)it->second->falseNegative);
			Debugger::addMetric("va", "False positive " + name + " [%]", fpRate);
			Debugger::addMetric("va", "False negative " + name + " [%]", fnRate);
			//Debugger::addMetric("va-correct", "Correct " + name + " [%]", correctRate);
			Debugger::addMetric("va-correct", "Correct " + name + " [%]", correct);
		}
		delete it->second;
	}
}

void VisionAnalyzer::createHistogram(string name, const map<int, int> &mapData, int maxVal) const
{
	stringstream sstr;
	sstr << maxVal;
	int numChars = (int) sstr.str().length();
	Debugger::DEBUG("VisionAnalyzer", "maxVal: %s, numChars: %d", sstr.str().c_str(), numChars);
	map<int, int>::const_iterator mapIt;
	for (mapIt = mapData.begin(); mapIt != mapData.end(); ++mapIt) {
		stringstream stream;
		stream << setfill('0') << setw(numChars) << mapIt->first;
		Debugger::addMetric("histogram-" + name,
				stream.str(), (double) mapIt->second);
	}
}

void VisionAnalyzer::createHistogramHex(string name, const map<int, int> &mapData, int maxVal) const
{
	stringstream sstr;
	sstr << hex << maxVal;
	int numChars = (int) sstr.str().length();
	Debugger::DEBUG("VisionAnalyzer", "maxVal: %s, numChars: %d", sstr.str().c_str(), numChars);
	map<int, int>::const_iterator mapIt;
	for (mapIt = mapData.begin(); mapIt != mapData.end(); ++mapIt) {
		stringstream stream;
		stream << "0x";
		stream << hex << setfill('0') << setw(numChars) << mapIt->first;
		Debugger::addMetric("histogram-" + name,
				stream.str(), (double) mapIt->second);
	}
}

bool VisionAnalyzer::execute()
{
	++mTotalImages;

	map<Object::ObjectType, visionAnalyzeData_t*>::iterator dataIt;
	for( dataIt = mVisionObjects.begin(); dataIt != mVisionObjects.end(); ++dataIt) {
		if( dataIt->first == Object::GOAL_POLE_YELLOW) {
			dataIt->second->objects = vector<Object>(getGoalPoles().getObjects());
		} else if( dataIt->first == Object::ROBOT_CYAN) {
			dataIt->second->objects = vector<Object>(getCyanRobots().getObjects());
		} else if( dataIt->first == Object::ROBOT_MAGENTA) {
			dataIt->second->objects = vector<Object>(getMagentaRobots().getObjects());
		} else {
			dataIt->second->objects.clear();
			if( getBall().lastImageSeen) {
				dataIt->second->objects.push_back(getBall());
			}
		}
		dataIt->second->count = 0;
		dataIt->second->found = 0;
	}

	const vector<Object>& annotations = getAnnotations().getObjects();
	for (vector<Object>::const_iterator annoIt = annotations.begin();
			annoIt != annotations.end(); ++annoIt) {
		bool isFound = false;
		dataIt = mVisionObjects.find( annoIt->type);
		if (dataIt != mVisionObjects.end()) {
			dataIt->second->count++;

			vector<Object>::iterator it;
			for ( it = dataIt->second->objects.begin(); it != dataIt->second->objects.end();
					++it) {
				if ((*it).doBoundingBoxesOverlap((*annoIt))) {
					isFound = true;
					dataIt->second->found++;
					dataIt->second->objects.erase(it);
					break;
				}
			}

			if (!isFound) {
				dataIt->second->falseNegative++;
				Debugger::DEBUG("VisionAnalyzer", "%s falseNegative", dataIt->second->name.c_str());
			}
		}
	}

	for( dataIt = mVisionObjects.begin(); dataIt != mVisionObjects.end(); ++dataIt) {
		size_t falsePositive = dataIt->second->objects.size();
		if( falsePositive > 0) {
			dataIt->second->falsePositive += falsePositive;
			Debugger::DEBUG("VisionAnalyzer", "%s falsePositive: %u", dataIt->second->name.c_str(), falsePositive);
		}
		double found = (double)(dataIt->second->found + 1) / (double)(dataIt->second->count + falsePositive + 1);
		dataIt->second->correctPercent += found;
		dataIt->second->countSum += dataIt->second->count;
		dataIt->second->foundSum += dataIt->second->found;
	}
	return true;
}
