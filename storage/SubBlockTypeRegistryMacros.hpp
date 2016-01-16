/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_SUB_BLOCK_TYPE_REGISTRY_MACROS_HPP_
#define QUICKSTEP_STORAGE_SUB_BLOCK_TYPE_REGISTRY_MACROS_HPP_

#define QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED(classname)    \
namespace registry { extern const bool classname##_registered; }  \
struct sub_block_type_registry_dummy  /* NOLINT(build/class) */

#define QUICKSTEP_FORCE_SUB_BLOCK_REGISTRATION(classname)                     \
namespace registry {                                                          \
extern const bool classname##_registered;                                     \
bool classname##_check_registered_force() { return classname##_registered; }  \
}                                                                             \
struct sub_block_type_registry_dummy  /* NOLINT(build/class) */

#endif  // QUICKSTEP_STORAGE_SUB_BLOCK_TYPE_REGISTRY_MACROS_HPP_
