/*
 * ConfigFile.h
 *
 *  Created on: 06.02.2015
 *      Author: Oliver Krebs
 */

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <stdint.h>
#include "../Debugging/Debugger.h"

/**
 * a config file with entries of key-value pairs in sections
 */
class ConfigFile {
public:
	/**
	 * Default Constructor
	 */
	ConfigFile();

	/**
	 * Create a new ConfigFile
	 * @param filename 	path to loading file
	 */
	ConfigFile(std::string const &filename);

	/**
	 * Destructor
	 */
	virtual ~ConfigFile();

	/**
	 * load a config file from file
	 * @param filename 	path to the loading file
	 * @return true, if config loaded successfully
	 */
	bool load(std::string const &filename);

	/**
	 * save the config to file
	 */
	virtual void save() const;

	/**
	 * create the config string
	 * @return string of config
	 */
	std::string createString(bool withComments = false) const;

	/**
	 * set a value in the config
	 * @param section 	name of section
	 * @param key 		name of entry key
	 * @param value 	entry value
	 */
	virtual void setString(std::string const &section, std::string const &key, std::string const &value);

	/**
	 * set a value in the config
	 * @param section 	name of section
	 * @param key 		name of entry key
	 * @param value 	entry value
	 */
	template <typename T>
	void set(std::string const &section, std::string const &key, T const &value) {
		std::ostringstream strout;
		strout << value;
		setString( section, key, strout.str());
	}

	/**
	 * get a value of the config
	 * @param section 	name of section
	 * @param key 		name of entry key
	 * @param defValue 	default entry value (set if entry not exists)
	 */
	std::string getString(std::string const &section, std::string const &key, std::string const &defValue);

	/**
	 * get a value of the config and set comment
	 * @param section 	name of section
	 * @param key 		name of entry key
	 * @param defValue 	default entry value (set if entry not exists)
	 * @param comment	comment to entry
	 */
	std::string getString(std::string const &section, std::string const &key, std::string const &defValue, std::string const &comment);

	/**
	 * get a value of the config
	 * @param section 	name of section
	 * @param key 		name of entry key
	 * @param defValue 	default entry value (set if entry not exists)
	 */
	template <typename T>
	T get(std::string const &section, std::string const &key, T const &defValue) {
		std::stringstream ss;
		ss << defValue;
		std::string val = getString( section, key, ss.str());
		T value;
		ss.str( val);
		ss >> value;
		return value;
	}

	/**
	 * get a value of the config and set comment
	 * @param section 	name of section
	 * @param key 		name of entry key
	 * @param defValue 	default entry value (set if entry not exists)
	 * @param comment	comment to entry
	 */
	template <typename T>
	T get(std::string const &section, std::string const &key, T const &defValue, std::string const &comment) {
		setComment(section, key, comment);
		return get<T>(section, key, defValue);
	}

	/**
	 * set comment to an entry in a section
	 * @param section	name of section
	 * @param key 		name of entry key
	 * @param comment 	comment to entry
	 */
	virtual void setComment(std::string const &section, std::string const &key, std::string const &comment);

protected:
	static const std::string SECTION_COMMENT; //!< comment for a section

	/**
	 * A section of a config file with all the entries in it
	 */
	class Section {
	public:
		/**
		 * Create a new section
		 */
		Section() : mSectionName("Global") {
		}

		/**
		 * initialize the section
		 * @param name		name of the section
		 */
		void init(std::string const &name) {
			mSectionName = name;
		}

		/**
		 * serialize content of section to stream
		 * @param stream 	output stream
		 * @param withComments	true means serialize with comments
		 */
		void serialize(std::ostream &stream, bool withComments = true) const {
			EntryIteratorC commentIt;
			if( withComments) {
				commentIt = mComments.find(SECTION_COMMENT);
				if( commentIt != mComments.end()) {
					stream << commentIt->second;
				}
			}
			stream << "[";
			stream << mSectionName;
			stream << "]\n";
			EntryIteratorC it;
			for( it = mEntries.begin(); it != mEntries.end(); ++it) {
				if( withComments) {
					commentIt = mComments.find(it->first);
					if( commentIt != mComments.end()) {
						stream << commentIt->second;
					}
				}
				stream << it->first;
				stream << "=";
				stream << it->second;
				stream << "\n";
			}
		}

		/**
		 * set an entry
		 * @param key 		name of entry key
		 * @param value 	entry value
		 */
		void set(std::string const &key, std::string const &value) {
			if( key.length() > 0 && value.length() > 0) {
				mEntries[key] = value;
			}
		}

		/**
		 * get an entry
		 * @param key 		name of entry key
		 * @param defValue 	default entry value (set if entry not exists)
		 */
		std::string get(std::string const &key, std::string const &defValue) {
			EntryIteratorC it = mEntries.find(key);
			if( it == mEntries.end()) {
				set(key, defValue);
				return defValue;
			}
			return it->second;
		}

		/**
		 * set comment to an entry
		 * @param key 		name of entry key
		 * @param comment 	comment to entry
		 */
		void setComment(std::string const &key, std::string const &comment) {
			if( comment.length() > 0) {
				if( comment[0] == '#' || comment[0] == ';') {
					mComments[key] = comment;
				} else {
					mComments[key] = "# " + comment;
				}
				if( comment[comment.length() - 1] != '\n') {
					mComments[key] += '\n';
				}
			}
		}

	private:
		std::string mSectionName;
		std::map<std::string, std::string> mEntries;
		std::map<std::string, std::string> mComments;

		typedef std::map<std::string,std::string>::iterator EntryIterator;
		typedef std::map<std::string,std::string>::const_iterator EntryIteratorC;
	};

private:
	std::string mFilename;
	std::map<std::string, Section> mSections;

	typedef std::map<std::string,Section>::iterator SectionIterator;
	typedef std::map<std::string,Section>::const_iterator SectionIteratorC;
};

/**
 * spezialization of set for bool
 * @param section 	name of section
 * @param key 		name of entry key
 * @param value 	entry value
 */
template <>
inline void ConfigFile::set<bool>(std::string const &section, std::string const &key, bool const &value) {
	if( value) {
		setString( section, key, "true");
	} else {
		setString( section, key, "false");
	}
}

/**
 * spezialization of set for int8_t
 * @param section 	name of section
 * @param key 		name of entry key
 * @param value 	entry value
 */
template <>
inline void ConfigFile::set<int8_t>(std::string const &section, std::string const &key, int8_t const &value) {
	std::ostringstream strout;
	strout << (int)value;
	setString( section, key, strout.str());
}

/**
 * spezialization of set for uint8_t
 * @param section 	name of section
 * @param key 		name of entry key
 * @param value 	entry value
 */
template <>
inline void ConfigFile::set<uint8_t>(std::string const &section, std::string const &key, uint8_t const &value) {
	std::ostringstream strout;
	strout << (int)value;
	setString( section, key, strout.str());
}

/**
 * spezialization of get for bool
 * @param section 	name of section
 * @param key 		name of entry key
 * @param value 	entry value
 */
template <>
inline bool ConfigFile::get<bool>(std::string const &section, std::string const &key, bool const &value) {
	std::string val = getString(section, key, value ? "true" : "false");
	if( val == "true") {
		return true;
	}
	return false;
}

/**
 * spezialization of set for int8_t
 * @param section 	name of section
 * @param key 		name of entry key
 * @param value 	entry value
 */
template <>
inline int8_t ConfigFile::get<int8_t>(std::string const&section, std::string const &key, int8_t const &value) {
	return (int8_t)get<int>(section, key, value);
}

/**
 * spezialization of get for uint8_t
 * @param section 	name of section
 * @param key 		name of entry key
 * @param value 	entry value
 */
template <>
inline uint8_t ConfigFile::get<uint8_t>(std::string const&section, std::string const &key, uint8_t const &value) {
	return (uint8_t)get<int>(section, key, value);
}

#endif /* CONFIGFILE_H_ */
