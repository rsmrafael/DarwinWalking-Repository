/*
 * ConfigurationProvider.h
 *
 *  Created on: 29.08.2012
 *      Author: Stefan Krupop
 */

#ifndef CONFIGURATIONPROVIDER_H_
#define CONFIGURATIONPROVIDER_H_

#include "../ModuleManager/Module.h"
#include "../Representations/CameraSettings.h"
#include "../Representations/RobotConfiguration.h"
#include "../Representations/BodyStatus.h"
#include "../Representations/Odometry.h"
#include "../Representations/GameStatus.h"
#include "../Representations/KickStatus.h"
#include "../Body/Body.h"
#include "../Game.h"

using namespace std;

BEGIN_DECLARE_MODULE(ConfigurationProvider)
	PROVIDE(CameraSettings,CameraSettings)
	PROVIDE(RobotConfiguration,RobotConfiguration)
	PROVIDE(BodyStatus,BodyStatus)
	PROVIDE(KickStatus, KickStatus)
	PROVIDE(Odometry, Odometry)
	PROVIDE(GameStatus, GameStatus)
END_DECLARE_MODULE(ConfigurationProvider)

class ConfigurationProvider : public ConfigurationProviderBase {
public:
	ConfigurationProvider(Body* body, Game* game);
	virtual ~ConfigurationProvider();

	virtual bool execute();

private:
	void loadCameraSettings() const;
	void saveCameraSettings() const;

	uint8_t mCameraSettingsChangedState;
	Body* mBody;
	Game* mGame;

	//lint -e(1704)
	ConfigurationProvider(const ConfigurationProvider& cSource);
	ConfigurationProvider& operator=(const ConfigurationProvider& cSource);
};

#endif /* CONFIGURATIONPROVIDER_H_ */
