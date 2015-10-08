/*
 * Config.cpp
 *
 *  Created on: 01.07.2009
 *      Author: Stefan
 */

#include <iostream>

#include "Config.h"
#include "Blackboard/DataHolder.h"
#include "ModuleManager/ModuleManager.h"
#include "Debugging/Debugger.h"

Config* Config::mInstance = NULL;

Config* Config::GetInstance() {
	if (mInstance == NULL) {
		mInstance = new Config();
	}
	return mInstance;
}

Config::Config() {
#ifdef CONFIG_FILE_ACCESS
	if (!ini_start(CONFIG_FILE)) {
		mNoSaveMode = true;
		fprintf(stderr, "[Config] WARNING! Could not load config file, will not save configuration changes!\n");
	} else {
		fprintf(stdout, "[Config] Config.ini loaded\n");
		mNoSaveMode = false;
	}
#else
	mNoSaveMode = true;
	fprintf(stdout, "[Config] Do not load config file, use default values!\n");
#endif
	mListeners = new list<ConfigChangeListener*>();
	mDisableChangeListeners = false;
	mDescriptions = NULL;
}

Config::~Config() {
	Debugger::INFO("Config", "Shutting down...");
	mListeners->clear();
	delete mListeners;
	if (!mNoSaveMode) {
		ini_end();
	}
	mDescriptions = NULL;
}

void Config::shutdown() const {
	delete mInstance;
	mInstance = 0;
}

void Config::saveConfig() const {
	if (!mNoSaveMode) {
		save();
	} else {
		Debugger::ERR("Config", "WARNING: NoSaveMode enabled, will not store configuration!");
	}
}

string Config::getConfigAsString() const {
#ifdef CONFIG_FILE_ACCESS
	return content_string();
#else
	return "[config]\nno_Access=Compile_with_CONFIG_FILE_ACCESS!\n";
#endif
}

void Config::registerConfigChangeHandler(ConfigChangeListener* handler) {
	mListeners->push_back(handler);
}

void Config::removeConfigChangeHandler(ConfigChangeListener* handler) {
	mListeners->remove(handler);
}

#ifdef CONFIG_FILE_ACCESS
void Config::notifyChangeHandlers() {
	if (!mDisableChangeListeners) {
		for (list<ConfigChangeListener*>::iterator it = mListeners->begin(); it != mListeners->end(); ++it) {
			(*it)->configChanged();
		}
	}
}
#endif

void Config::setDisableChangeListeners(bool state) {
	mDisableChangeListeners = state;
}

bool Config::GetBoolean(string Section, string Setting, bool Default) {
#ifdef CONFIG_FILE_ACCESS
	string buffer = get_value(Section.c_str(), Setting.c_str());
	if (buffer != "") {
		if (buffer == "wahr" || buffer == "Wahr" || buffer == "WAHR" || buffer == "true" || buffer == "True"  || buffer == "TRUE") {
			return true;
		} else {
			return false;
		}
	} else {
#ifdef WRITE_DEFAULTS_ON_ACCESS
		SetBoolean(Section, Setting, Default);
#endif
		return Default;
	}
#else
	return Default;
#endif
}

bool Config::GetBoolean(string Section, string Setting, bool Default, string Description) {
	addDescription(Section, Setting, Description);
	return GetBoolean(Section, Setting, Default);
}

bool Config::SetBoolean(string Section, string Setting, bool Value) {
#ifdef CONFIG_FILE_ACCESS
	bool ret = set_value(Section.c_str(), Setting.c_str(), Value ? "true" : "false");
	GetInstance()->notifyChangeHandlers();
	return ret;
#else
	return false;
#endif
}

string Config::GetString(string Section, string Setting, string Default) {
#ifdef CONFIG_FILE_ACCESS
	string buffer = get_value(Section.c_str(), Setting.c_str());
	if (buffer != "") {
		return buffer;
	} else {
#ifdef WRITE_DEFAULTS_ON_ACCESS
		SetString(Section, Setting, Default);
#endif
		return Default;
	}
#else
	return Default;
#endif
}

string Config::GetString(string Section, string Setting, string Default, string Description) {
	addDescription(Section, Setting, Description);
	return GetString(Section, Setting, Default);
}

bool Config::SetString(string Section, string Setting, string Value) {
#ifdef CONFIG_FILE_ACCESS
	bool ret = set_value(Section.c_str(), Setting.c_str(), Value.c_str());
	GetInstance()->notifyChangeHandlers();
	return ret;
#else
	return false;
#endif
}

int Config::GetInt(string Section, string Setting, int Default) {
#ifdef CONFIG_FILE_ACCESS
	string buffer = get_value(Section.c_str(), Setting.c_str());

	if (buffer != "") {
		int value;
	    istringstream strin;
	    strin.str(buffer);
	    strin >> value;
	    return value;
	} else
	{
#ifdef WRITE_DEFAULTS_ON_ACCESS
		SetInt(Section, Setting, Default);
#endif
		return Default;
	}
#else
	return Default;
#endif
}

int Config::GetInt(string Section, string Setting, int Default, string Description) {
	addDescription(Section, Setting, Description);
	return GetInt(Section, Setting, Default);
}

bool Config::SetInt(string Section, string Setting, int Value) {
#ifdef CONFIG_FILE_ACCESS
    ostringstream strout;
    strout << Value;

    string val = strout.str();

	bool ret = set_value(Section.c_str(), Setting.c_str(), val.c_str());
	GetInstance()->notifyChangeHandlers();
	return ret;
#else
	return false;
#endif
}

double Config::GetDouble(string Section, string Setting, double Default) {
#ifdef CONFIG_FILE_ACCESS
	string buffer = get_value(Section.c_str(), Setting.c_str());

	if (buffer != "") {
		double value;
	    istringstream strin;
	    strin.str(buffer);
	    strin >> value;
	    return value;
	} else {
#ifdef WRITE_DEFAULTS_ON_ACCESS
		SetDouble(Section, Setting, Default);
#endif
		return Default;
	}
#else
	return Default;
#endif
}

double Config::GetDouble(string Section, string Setting, double Default, string Description) {
	addDescription(Section, Setting, Description);
	return GetDouble(Section, Setting, Default);
}

bool Config::SetDouble(string Section, string Setting, double Value) {
#ifdef CONFIG_FILE_ACCESS
    ostringstream strout;
    strout << Value;

    string val = strout.str();

	bool ret = set_value(Section.c_str(), Setting.c_str(), val.c_str());
	GetInstance()->notifyChangeHandlers();
	return ret;
#else
	return false;
#endif
}

void Config::addDescription(string Section, string Setting, string Description) {
	ConfigDescriptions* desc = GetInstance()->mDescriptions;
	if (desc != NULL) {
		desc->addDescription(Section, Setting, Description);
	}
	// cppcheck-suppress memleak
}

void Config::moduleManagerReady() {
	DataHolder<ConfigDescriptions>& rep = ModuleManager::GetInstance()->getBlackboardDirect(true)->getRepresentation<DataHolder<ConfigDescriptions> >("ConfigDescriptions");
	mDescriptions = &(*rep);
}
