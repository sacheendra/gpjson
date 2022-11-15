#!/bin/bash

# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command exited with exit code $?."' EXIT

NODE_PATH="$NODE_DIR/bin"
GRAAL_PATH="$GRAAL_DIR/bin"
WARMUP=5
REPEAT=10

# Read command parameters
noGPU=false
while getopts gw:r: OPT
do
    case "$OPT" in
        g) noGPU=true ;;
        w) WARMUP=${OPTARG} ;;
        r) REPEAT=${OPTARG} ;;
    esac
done

echo "Starting benchmarks"
echo "engine,dataset,query,time,stddev,results,warmup,repeat" > batch1.csv

# gpjson
if [ "$noGPU" = false ]
then
    echo "running gpjson"
    cd gpjson
    $GRAAL_PATH/node --jvm --polyglot batch1.js warmup=$WARMUP repeat=$REPEAT >> ../batch1.csv
    cd ..
fi

# javajsonpath
echo "running javajsonpath"
cd javajsonpath
mvn package > /dev/null 2>&1
java -cp target/javajsonpath-1.0-SNAPSHOT-jar-with-dependencies.jar it.polimi.Batch1 warmup=$WARMUP repeat=$REPEAT >> ../batch1.csv
cd ..

# nodejsonpath
echo "running nodejsonpath"
cd nodejsonpath
$NODE_PATH/npm install jsonpath > /dev/null 2>&1
$NODE_PATH/node batch1.js warmup=$WARMUP repeat=$REPEAT >> ../batch1.csv
cd ..

# nodejsonpathplus
echo "running nodejsonpathplus"
cd nodejsonpathplus
$NODE_PATH/npm install jsonpath-plus > /dev/null 2>&1
$NODE_PATH/node batch1.js warmup=$WARMUP repeat=$REPEAT >> ../batch1.csv
cd ..

# nodemanual
echo "running nodemanual"
cd nodemanual
$NODE_PATH/node batch1.js warmup=$WARMUP repeat=$REPEAT >> ../batch1.csv
cd ..

# nodesimdjson
echo "running nodesimdjson"
cd nodesimdjson
$NODE_PATH/npm install simdjson > /dev/null 2>&1
$NODE_PATH/node batch1.js warmup=$WARMUP repeat=$REPEAT >> ../batch1.csv
cd ..

# pison
echo "running pison"
cd pison
make batch1 > /dev/null 2>&1
./bin/batch1 warmup=$WARMUP repeat=$REPEAT >> ../batch1.csv
cd ..

# rapidjson
echo "running rapidjson"
cd rapidjson
make batch1 > /dev/null 2>&1
./bin/batch1 warmup=$WARMUP repeat=$REPEAT >> ../batch1.csv
cd ..

# simdjson
echo "running simdjson"
cd simdjson
make batch1 > /dev/null 2>&1
./bin/batch1 warmup=$WARMUP repeat=$REPEAT >> ../batch1.csv
cd ..

echo "Benchmarks done"