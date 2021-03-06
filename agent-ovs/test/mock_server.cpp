/* -*- C++ -*-; c-basic-offset: 4; indent-tabs-mode: nil */
/*
 * Mock server standalone
 *
 * Copyright (c) 2014 Cisco Systems, Inc. and others.  All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/epl-v10.html
 */
#include <unistd.h>
#include <signal.h>

#include <string>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/assign/list_of.hpp>

#include <modelgbp/dmtree/Root.hpp>
#include <modelgbp/metadata/metadata.hpp>
#include <opflex/test/MockOpflexServer.h>
#include <opflex/ofcore/OFFramework.h>
#include <opflex/ofcore/OFConstants.h>

#include "logging.h"
#include "cmd.h"
#include "Policies.h"

using std::string;
using std::make_pair;
using boost::assign::list_of;
namespace po = boost::program_options;
using opflex::test::MockOpflexServer;
using opflex::ofcore::OFConstants;
using namespace ovsagent;

void sighandler(int sig) {
    LOG(INFO) << "Got " << strsignal(sig) << " signal";
}

#define SERVER_ROLES \
        (OFConstants::POLICY_REPOSITORY |     \
         OFConstants::ENDPOINT_REGISTRY |     \
         OFConstants::OBSERVER)
#define LOCALHOST "127.0.0.1"

int main(int argc, char** argv) {
    // Parse command line options
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Print this help message")
        ("log", po::value<string>()->default_value(""), 
         "Log to the specified file (default standard out)")
        ("level", po::value<string>()->default_value("info"), 
         "Use the specified log level (default info)")
        ("sample", po::value<string>()->default_value(""), 
         "Output a sample policy to the given file then exit")
        ("daemon", "Run the agent as a daemon")
        ("policy,p", po::value<string>()->default_value(""),
         "Read the specified policy file to seed the MODB")
        ;

    bool daemon = false;
    std::string log_file;
    std::string level_str;
    std::string policy_file;
    std::string sample_file;

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).
                  options(desc).run(), vm);
        po::notify(vm);
    
        if (vm.count("help")) {
            std::cout << "Usage: " << argv[0] << " [options]\n";
            std::cout << desc;
            return 0;
        }
        if (vm.count("daemon")) {
            daemon = true;
        }
        log_file = vm["log"].as<string>();
        level_str = vm["level"].as<string>();
        policy_file = vm["policy"].as<string>();
        sample_file = vm["sample"].as<string>();

    } catch (po::unknown_option e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (daemon)
        daemonize();

    initLogging(level_str, log_file);

    try {
        if (sample_file != "") {
            opflex::ofcore::MockOFFramework mframework;
            mframework.setModel(modelgbp::getMetadata());
            mframework.start();
            Policies::writeBasicInit(mframework);
            Policies::writeTestPolicy(mframework);
            
            mframework.dumpMODB(modelgbp::dmtree::Root::CLASS_ID,
                                sample_file);
            
            mframework.stop();
            return 0;
        }

        MockOpflexServer server(8009, SERVER_ROLES, 
                                list_of(make_pair(SERVER_ROLES, LOCALHOST":8009")),
                                modelgbp::getMetadata());

        if (policy_file != "") {
            server.readPolicy(policy_file);
        }

        server.start();
        signal(SIGINT, sighandler);
        pause();
        server.stop();
    } catch (const std::exception& e) {
        LOG(ERROR) << "Fatal error: " << e.what();
        return 2;
    } catch (...) {
        LOG(ERROR) << "Unknown fatal error";
        return 3;
    }
}
