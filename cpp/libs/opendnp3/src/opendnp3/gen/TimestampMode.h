//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef __OPENDNP3_GENERATED_TIMESTAMPMODE_H_
#define __OPENDNP3_GENERATED_TIMESTAMPMODE_H_

#include <cstdint>

namespace opendnp3 {

/**
  Indicates the validity of a timestamp values for an entire object header
*/
enum class TimestampMode : int
{
  /// Timestamp is not valid, ignore the value and use a local timestamp
  INVALID = 0,
  /// The timestamp is UTC synchronzied at the remote device
  SYNCHRONIZED = 1,
  /// The device indicate the timstamp may be unsynchronized
  UNSYNCHRONIZED = 2
};


}

#endif
