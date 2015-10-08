/*
 * SettingConfig.h
 *
 *  Created on: 06.02.2015
 *      Author: Oliver Krebs
 */

#ifndef SETTINGCONFIG_H_
#define SETTINGCONFIG_H_

#include "ConfigFile.h"
#include <string>

/**
 * Setting Config is a config file for all setting values, that do not change frequently.
 * e.g. the length of the field, size of the ball, ...
 */
class SettingConfig : public ConfigFile {
public:
	/**
	 * get singleton instance of SettingConfig
	 */
	static SettingConfig *getInstance();

	/**
	 * clear up and destroy instance
	 */
	static void shutdown();

private:
	SettingConfig();
	~SettingConfig();

	static SettingConfig *mInstance;	//!< singleton instance
};

#endif /* SETTINGCONFIG_H_ */
