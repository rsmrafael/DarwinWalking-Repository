#include "../../ModuleManager/Module.h"
#include "../../Representations/YUVImage.h"
#include "../../Representations/BodyStatus.h"
#include "../../Representations/OpenCVMat.h"
#include "../../ConfigChangeListener.h"
#include "../../Representations/Object.h"

BEGIN_DECLARE_MODULE(TechnicalChallengeVision)
	REQUIRE(YUVImage,Image)
	REQUIRE(BodyStatus, BodyStatus)
	//---------------------------------
	PROVIDE(Object,Ball)
END_DECLARE_MODULE(TechnicalChallengeVision)

class TechnicalChallengeVision: public TechnicalChallengeVisionBase, public ConfigChangeListener {
public:
	TechnicalChallengeVision();
	virtual ~TechnicalChallengeVision();

	/**
	 * execute
	 * @see Module::execute
	 */
	virtual bool execute();

	/**
	 * config has changed
	 * @see ConfigChangeListener::configChanged
	 */
	virtual void configChanged();

private:
	/**
	 * Config Parameters
	 */
	size_t mScanlineDistanceColumn;
	size_t mScanlineDistanceRow;
	size_t mMinY_Ball;
	size_t mMinU_Ball;
	size_t mWhiteCounter;
};
