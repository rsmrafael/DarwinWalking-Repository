/*
 * ConfigChangeListener.h
 *
 *  Created on: 24.04.2013
 *      Author: Stefan
 */

#ifndef CONFIGCHANGELISTENER_H_
#define CONFIGCHANGELISTENER_H_

class ConfigChangeListener {
	public:
		virtual ~ConfigChangeListener() {}
		virtual void configChanged() = 0;
};

#endif /* CONFIGCHANGELISTENER_H_ */
