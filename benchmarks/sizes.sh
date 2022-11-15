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
while getopts g OPT
do
    case "$OPT" in
        g) noGPU=true ;;
    esac
done

echo "Starting benchmarks"
echo "engine,dataset,query,time,stddev,results,warmup,repeat" > sizes.csv

# gpjson
if [ "$noGPU" = false ]
then
    echo "running gpjson"
    cd gpjson
    $GRAAL_PATH/node --jvm --polyglot sizes.js warmup=$WARMUP repeat=$REPEAT >> ../sizes.csv
    cd ..
fi

# javajsonpath
echo "running javajsonpath"
cd javajsonpath
mvn package > /dev/null 2>&1
java -cp target/javajsonpath-1.0-SNAPSHOT-jar-with-dependencies.jar it.polimi.Sizes warmup=$WARMUP repeat=$REPEAT >> ../sizes.csv
cd ..

# nodejsonpath
echo "running nodejsonpath"
cd nodejsonpath
$NODE_PATH/npm install jsonpath > /dev/null 2>&1
$NODE_PATH/node sizes.js warmup=$WARMUP repeat=$REPEAT >> ../sizes.csv
cd ..

# nodejsonpathplus
echo "running nodejsonpathplus"
cd nodejsonpathplus
$NODE_PATH/npm install jsonpath-plus > /dev/null 2>&1
$NODE_PATH/node sizes.js warmup=$WARMUP repeat=$REPEAT >> ../sizes.csv
cd ..

# nodemanual
echo "running nodemanual"
cd nodemanual
$NODE_PATH/node sizes.js warmup=$WARMUP repeat=$REPEAT >> ../sizes.csv
cd ..

# nodesimdjson
echo "running nodesimdjson"
cd nodesimdjson
$NODE_PATH/npm install simdjson > /dev/null 2>&1
$NODE_PATH/node sizes.js warmup=$WARMUP repeat=$REPEAT >> ../sizes.csv
cd ..

# pison
echo "running pison"
cd pison
make sizes > /dev/null 2>&1
./bin/sizes warmup=$WARMUP repeat=$REPEAT >> ../sizes.csv
cd ..

# rapidjson
echo "running rapidjson"
cd rapidjson
make sizes > /dev/null 2>&1
./bin/sizes warmup=$WARMUP repeat=$REPEAT >> ../sizes.csv
cd ..

# simdjson
echo "running simdjson"
cd simdjson
make sizes > /dev/null 2>&1
./bin/sizes warmup=$WARMUP repeat=$REPEAT >> ../sizes.csv
cd ..

echo "Benchmarks done"