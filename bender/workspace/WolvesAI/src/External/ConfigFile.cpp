/*
 * ConfigFile.cpp
 *
 *  Created on: 06.02.2015
 *      Author: Oliver Krebs
 */

#include "ConfigFile.h"
#include "../Debugging/Debugger.h"

using namespace std;
const std::string ConfigFile::SECTION_COMMENT = "###";

std::string trim(std::string const& source, char const* delims = " \t\r\n") {
	std::string result(source);
	std::string::size_type index = result.find_last_not_of(delims);
	if(index != std::string::npos) {
		result.erase(++index);
	}

	index = result.find_first_not_of(delims);
	if(index != std::string::npos) {
		result.erase(0, index);
	} else {
		result.erase();
	}
	return result;
}

ConfigFile::ConfigFile()
{

}

ConfigFile::ConfigFile(string const &filename)
{
	load( filename);
}

ConfigFile::~ConfigFile()
{

}

bool ConfigFile::load(string const &filename)
{
	mFilename = filename;
	std::ifstream file;
	file.open(filename.c_str());
	if( !file) {
		fprintf(stderr, "[ConfigFile] Could not load file: %s\n", filename.c_str());
		return false;
	}
	std::string line;
	std::string section = "Global";
	std::string comment = "";
	while (std::getline(file, line)) {
		if (line.length() == 0) {
			continue;
		} else if (line[0] == '#' || line[0] == ';') {
			comment += line + "\n";
		} else if (line[0] == '[') {
			int pos = line.find(']');
			if( pos != string::npos) {
				section = trim(line.substr(1, pos - 1));
				if( mSections.find(section) == mSections.end()) {
					mSections[section].init(section);
					mSections[section].setComment( SECTION_COMMENT, comment);
					comment = "";
				}
			}
		} else {
			int pos = line.find('=');
			if( pos != string::npos) {
				std::string name = trim(line.substr(0, pos));
				std::string value = trim(line.substr(pos + 1));
				mSections[section].set( name, value);
				mSections[section].setComment( name, comment);
				comment = "";
			}
		}
	}
	return true;
}

void ConfigFile::save() const
{
	ofstream file(mFilename.c_str());
	if( !file) {
		Debugger::WARN("ConfigFile", "Could not save file: %s", mFilename.c_str());
		return;
	}
	Debugger::DEBUG("ConfigFile", "Save: %s", mFilename.c_str());
	SectionIteratorC it;
	for( it = mSections.begin(); it != mSections.end(); ++it) {
		it->second.serialize(file);
	}
	file.close();
}

string ConfigFile::createString(bool withComments) const {
	ostringstream stream;
	SectionIteratorC it;
	for( it = mSections.begin(); it != mSections.end(); ++it) {
		it->second.serialize(stream, withComments);
	}
	return stream.str();
}

void ConfigFile::setString(std::string const &section, std::string const &key, std::string const &value) {
	SectionIterator it = mSections.find(section);
	if( it == mSections.end()) {
		mSections[section].init(section);
		mSections[section].set(key, value);
	} else {
		it->second.set(key, value);
	}
}

std::string ConfigFile::getString(std::string const &section, std::string const &key, std::string const &defValue) {
	SectionIterator it = mSections.find(section);
	if( it == mSections.end()) {
		set(section, key, defValue);
		return defValue;
	}
	return it->second.get(key, defValue);
}

std::string ConfigFile::getString(std::string const &section, std::string const &key, std::string const &defValue, std::string const &comment) {
	setComment(section, key, comment);
	return getString(section, key, defValue);
}

void ConfigFile::setComment(std::string const &section, std::string const &key, std::string const &comment) {
	SectionIterator it = mSections.find(section);
	if( it != mSections.end()) {
		it->second.setComment(key, comment);
	}
}
