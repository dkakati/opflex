/* -*- C++ -*-; c-basic-offset: 4; indent-tabs-mode: nil */
/*!
 * @file MockOpflexServer.h
 * @brief Interface definition file for MockOpflexServer
 */
/*
 * Copyright (c) 2014 Cisco Systems, Inc. and others.  All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/epl-v10.html
 */

#include <vector>
#include <utility>

#include "opflex/modb/ModelMetadata.h"

#pragma once
#ifndef OPFLEX_TEST_MOCKOPFLEXSERVER_H
#define OPFLEX_TEST_MOCKOPFLEXSERVER_H

namespace opflex {

namespace engine {
namespace internal {

class MockOpflexServerImpl;

} /* namespace internal */
} /* namespace engine */

namespace test {

/**
 * An opflex server we can use for mocking interactions with a real
 * Opflex server
 */
class MockOpflexServer {
public:
    /**
     * a pair of a role bitmask and connectivity string
     */
    typedef std::pair<uint8_t, std::string> peer_t;

    /**
     * A vector of peers
     */
    typedef std::vector<peer_t> peer_vec_t;

    /**
     * Construct a new mock opflex server
     *
     * @param port listen port for the server
     * @param roles the opflex roles for this server
     * @param peers a list of peers to return in the opflex handshake
     * @param md the model metadata for the server
     */
    MockOpflexServer(int port, uint8_t roles, peer_vec_t peers,
                     const modb::ModelMetadata& md);

    /**
     * Destroy the opflex server
     */
    ~MockOpflexServer();

    /**
     * Start the server
     */
    void start();

    /**
     * Stop the server
     */
    void stop();

    /**
     * Read policy into the server from the specified file.  Note that
     * this will not automatically cause updates to be sent to
     * connected clients.
     *
     * @param file the filename to read in
     */
    void readPolicy(const std::string& file);

    /**
     * Get the peers that this server was configured with
     *
     * @return a vector of peer pairs
     */
    const peer_vec_t& getPeers() const;

    /**
     * Get the port number that this server was configured with
     *
     * @return the port number
     */
    int getPort() const;

    /**
     * Get the roles that this server was configured with
     * 
     * @param a bitmask containing the server roles
     */
    uint8_t getRoles() const;

private:
    engine::internal::MockOpflexServerImpl* pimpl;
};

} /* namespace test */
} /* namespace opflex */

#endif /* OPFLEX_TEST_MOCKOPFLEXSERVER_H */

