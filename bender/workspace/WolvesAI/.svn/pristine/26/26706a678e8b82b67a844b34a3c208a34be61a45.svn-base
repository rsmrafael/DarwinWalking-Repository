/*
 * VisionAnalyzer.h
 *
 *  Created on: Sep 3, 2012
 *      Author: jan
 */

#ifndef VisionAnalyzer_H_
#define VisionAnalyzer_H_

#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/ObjectList.h"
#include "../../Representations/Object.h"
#include "../../Representations/Histogram.h"
#include <string>
#include <vector>

BEGIN_DECLARE_MODULE(VisionAnalyzer)
	REQUIRE(YUVImage, Image)
	REQUIRE(ObjectList, Annotations)
	REQUIRE(Object, Ball)
	REQUIRE(ObjectList, GoalPoles)
	REQUIRE(ObjectList, CyanRobots)
	REQUIRE(ObjectList, MagentaRobots)
END_DECLARE_MODULE(VisionAnalyzer)

/**
 * VisionAnalyzer analyzes the vision data compared to the annotations
 */
class VisionAnalyzer: public VisionAnalyzerBase {
public:

	/**
	 * a structure for vision analyze data
	 */
	typedef struct {
		std::string name;			//!< name of the vision object
		size_t count;				//!< how many times the object is in the image (annotations)
		size_t found;				//!< how many times the object was found correctly
		size_t countSum;			//!< like count, but for all images
		size_t foundSum;			//!< like found, but for all images
		size_t falsePositive;		//!< false positives for all images
		size_t falseNegative;		//!< false negatives for all images
		size_t correctImages;		//!< number of images objects were found correctly (no false negatives or no false positives)
		vector<Object> objects;		//!< found vision objects
	} visionAnalyzeData_t;

	/**
	 * Constructor
	 * @param createCsv		create csv files
	 */
	VisionAnalyzer(bool createCsv = true);
	virtual ~VisionAnalyzer();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

private:
	void createHistogram(string name, const map<int, int> &mapData, int maxVal) const;
	void createHistogramHex(string name, const map<int, int> &mapData, int maxVal) const;

	size_t mTotalImages;

	std::map<Object::ObjectType, visionAnalyzeData_t*> mVisionObjects;

	bool mCreateCsv;
};

#endif /* VisionAnalyzer_H_ */
