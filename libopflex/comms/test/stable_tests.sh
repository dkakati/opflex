#!/bin/bash

CATEGORY=${1:-STABLE}
ulimit -c unlimited

# Timeout.
declare -i timeout=20
# Interval between checks if the process is still alive.
declare -i interval=5
# Delay between posting the SIGTERM signal and destroying the process by SIGKILL.
declare -i delay=2

function watcher() {
    ((t = timeout))

    while ((t > 0)); do
        sleep $interval
        kill -0 $$ || exit 0
        ((t -= interval))
    done

    # Be nice, post SIGTERM first.
    # The 'exit 0' below will be executed if any preceeding command fails.
    kill -s SIGTERM $$ && kill -0 $$ || exit 0
    sleep $delay
    kill -s SIGKILL $$
}

(watcher) &> /dev/null &
BG_WATCHER=$!

./comms_test --catch_system_errors=no --run_test=asynchronous_sockets/${CATEGORY}_test_'*'
RET_VAL=$?
kill -9 ${BG_WATCHER} &> /dev/null

# FIXME: try to parse core pattern
find . -name '*core*' -type f -exec echo Found possible core file: '{}' \;

exit ${RET_VAL}
