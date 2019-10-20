#!/bin/bash

# copy the localizationCommon.h file to the determineWorld code (and any others)
rsync -av localizationUtils/localizationCommon.h determineWorld/

# copy the sparki constants into the associated directories
rsync -av SparkiCommon/sparkiCommon.h determineWorld/
rsync -av SparkiCommon/sparkiCommon.h movementTest/
rsync -av SparkiCommon/sparkiCommon.h sensorTest/
rsync -av SparkiCommon/sparkiCommon.h localizationUtils/
