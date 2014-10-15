/*
 * Copyright (c) 2014 Cisco Systems, Inc. and others.  All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/epl-v10.html
 */

#include <opflex/rpc/rpc.hpp>
#include <opflex/rpc/methods.hpp>
#include <opflex/rpc/internal/meta.hpp>

namespace meta = opflex::rpc::internal::meta;

namespace opflex { namespace rpc {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmultichar"


MethodName const *
MessageFactory::lookupMethod(char const * method) {
    LOG(DEBUG) << method;
#undef  PERFECT_RET_VAL
#define PERFECT_RET_VAL(method) \
    &method
#include <opflex/rpc/method_lookup.hpp>
}

#pragma GCC diagnostic pop

}}
