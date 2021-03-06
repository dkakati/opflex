/*
 * Definition of FlowReader class.
 *
 * Copyright (c) 2014 Cisco Systems, Inc. and others.  All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/epl-v10.html
 */

#ifndef FLOWREADER_H_
#define FLOWREADER_H_

#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>

#include "ovs.h"
#include "TableState.h"
#include "SwitchConnection.h"

namespace ovsagent {

/**
 * @brief Class that allows you to read flow and group tables from a switch.
 */
class FlowReader : public opflex::enforcer::MessageHandler {
public:
    /**
     * Default constructor
     */
    FlowReader();
    virtual ~FlowReader();

    /**
     * Register all the necessary event listeners on connection.
     * @param conn Connection to register
     */
    void installListenersForConnection(
        opflex::enforcer::SwitchConnection *conn);

    /**
     * Unregister all event listeners from connection.
     * @param conn Connection to unregister from
     */
    void uninstallListenersForConnection(
        opflex::enforcer::SwitchConnection *conn);

    /**
     * Callback function to process a list of flow-table entries.
     */
    typedef boost::function<void
        (const opflex::enforcer::flow::FlowEntryList&, bool)> FlowCb;

    /**
     * Get the flow-table entries for specified table.
     *
     * @param tableId ID of flow-table to read
     * @param cb Callback function to invoke when flow-entries are
     * received
     * @return true if request for getting flows was sent successfully
     */
    virtual bool getFlows(uint8_t tableId, const FlowCb& cb);

    /**
     * Callback function to process a list of group-table entries.
     */
    typedef boost::function<void
        (const opflex::enforcer::flow::GroupEdit::EntryList&, bool)> GroupCb;

    /**
     * Get the group-table entries from switch.
     *
     * @param cb Callback function to invoke when group-entries are
     * received
     * @return true if request for getting groups was sent successfully
     */
    virtual bool getGroups(const GroupCb& cb);

    /* Interface: MessageHandler */
    void Handle(opflex::enforcer::SwitchConnection *c, ofptype msgType,
        ofpbuf *msg);

private:
    /**
     * Create a request for reading all entries of specified table.
     *
     * @param tableId ID of flow-table to read
     * @return flow-table read request
     */
    ofpbuf *createFlowRequest(uint8_t tableId);

    /**
     * Create a request for reading all entries of group-table.
     *
     * @return group-table read request
     */
    ofpbuf *createGroupRequest();

    /**
     * Send specified read request on the connection and update
     * internal structures to track replies.
     *
     * @param msg Read request to send
     * @param cb Callback to invoke when replies are received
     * @param reqMap Map to track requests and callbacks
     * @return true if request was sent successfully
     */
    template <typename U, typename V>
    bool sendRequest(ofpbuf *msg, const U& cb, V& reqMap);

    /**
     * Process the reply message received for a read request
     * and invoke the callback registered for the request.
     *
     * @param msgType Type of message received
     * @param msg The reply message
     * @param reqMap Map to track requests and callbacks
     */
    template<typename T, typename U, typename V>
    void handleReply(ofptype msgType, ofpbuf *msg, V& reqMap);

    /**
     * Extract flow/group entries from the received message.
     *
     * @param msg The received reply message
     * @param recv Container to put the extracted entries into
     * @param replyDone Set to true if no more replies are
     * expected for this request
     */
    template<typename T>
    void decodeReply(ofpbuf *msg, T& recv, bool& replyDone);

    opflex::enforcer::SwitchConnection *swConn;

    boost::mutex reqMtx;

    typedef boost::unordered_map<ovs_be32, FlowCb> FlowCbMap;
    FlowCbMap flowRequests;
    typedef boost::unordered_map<ovs_be32, GroupCb> GroupCbMap;
    GroupCbMap groupRequests;
};

}   // namespace ovsagent



#endif /* FLOWREADER_H_ */
