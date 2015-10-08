#include "TechnicalChallengeVision.h"
#include "../../Config.h"

TechnicalChallengeVision::TechnicalChallengeVision()
:	mScanlineDistanceColumn( 2),
	mScanlineDistanceRow( 2),
	mMinY_Ball( 150),
	mMinU_Ball(100),
	mWhiteCounter(0)
{
	Config::getInstance()->registerConfigChangeHandler(this);
	this->configChanged();
}

TechnicalChallengeVision::~TechnicalChallengeVision() {
	Config::getInstance()->removeConfigChangeHandler(this);
}

bool TechnicalChallengeVision::execute() {
	size_t mWhiteCurrent = 0;

	const YUVImage* yuvImage = &getImage();
	int width = yuvImage->getWidth();
	int height = yuvImage->getHeight();

	int dx = mScanlineDistanceColumn;
	int dy = mScanlineDistanceRow;
	for (int x = 0; x < width; x += dx) {
		for (int y = height - 1; y >= 0; y -= dy) { //
			struct YUVImage::YUVData currentColor = yuvImage->getValue(x, y);
			if (currentColor.Y >= mMinY_Ball && currentColor.U >= mMinU_Ball) {
				mWhiteCurrent++;
			}
		}
	}
	if (mWhiteCounter == 0 || mWhiteCurrent*1.15 >= mWhiteCounter) {
		mWhiteCounter = mWhiteCurrent;
		getBall().notSeen();
	} else {
		getBall().updateObject(0, 0, 0, 0);
	}
	return true;
}

void TechnicalChallengeVision::configChanged() {
	Debugger::INFO("Scanlines", "config changed");

	ConfigFile *config = Config::getInstance();
	mScanlineDistanceColumn		= config->get<int>("Vision", "scanlineDistanceColumn", 3);
	mScanlineDistanceRow		= config->get<int>("Vision", "scanlineDistanceRow", 3);

	if( mScanlineDistanceColumn < 1) {
		Debugger::WARN("Scanlines", "ScanlineDistanceColumn was too low (%d), set to 1!", mScanlineDistanceColumn);
		mScanlineDistanceColumn = 1;
	}
	if( mScanlineDistanceRow < 1) {
		Debugger::WARN("Scanlines", "ScanlineDistanceRow was too low (%d), set to 1!", mScanlineDistanceRow);
		mScanlineDistanceRow = 1;
	}

	mMinY_Ball = config->get<int>("Vision", "Ball_minY", 150);
	mMinU_Ball 			= config->get<int>("Vision", "Ball_minU", 100);
}
