/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __OPENDNP3_EVENT_H_
#define __OPENDNP3_EVENT_H_

#include <opendnp3/IndexedValue.h>
#include "EventType.h"

namespace opendnp3
{

/**
 * Record of an event that includes value, index, and class
 */
template <typename T>
struct Event : public IndexedValue<T>
{	
	Event(const T& arValue, uint32_t aIndex, EventClass aClass) :
		IndexedValue<T>(arValue, aIndex),		
		clazz(aClass)
	{}

	Event() : clazz(EventClass::EC1)
	{}	
	
	EventClass clazz;	// class of the event (PC_CLASS<1-3>)	
};

} //end namespace

/* vim: set ts=4 sw=4: */

#endif

