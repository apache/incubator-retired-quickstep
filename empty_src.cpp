/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

// On OSX, ranlib complains if a static library archive contains no symbols,
// so we export a dummy global variable.
#ifdef __APPLE__
namespace quickstep { extern constexpr int kDarwinGlobalDummy = 0; }
#endif
