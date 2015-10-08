/*
 * SettingConfig.cpp
 *
 *  Created on: 06.02.2015
 *      Author: Oliver Krebs
 */

#include "SettingConfig.h"
#include "../Config.h"
#include <cstddef>

SettingConfig* SettingConfig::mInstance = NULL;

SettingConfig::SettingConfig()
:	ConfigFile(Config::getInstance()->getString("Setting", "File", "SettingKid.ini"))
{

}

SettingConfig::~SettingConfig()
{
	save();
}

SettingConfig *SettingConfig::getInstance()
{
	if( mInstance == NULL) {
		mInstance = new SettingConfig();
	}
	return mInstance;
}

void SettingConfig::shutdown()
{
	delete mInstance;
	mInstance = NULL;
}
