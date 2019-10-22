#!/bin/bash

# copy the localizationCommon.h file to the determineWorld code (and any others)
rsync -av localizationUtils/localizationCommon.h determineWorld/
rsync -av localizationUtils/localizationCommon.h movements/

# copy the determineWorld.h into the appropriate paths
rsync -av determineWorld/determineWorld.h movements/

rsync -av ultrasonic/ultrasonicCommon.h determineWorld/
rsync -av ultrasonic/ultrasonicCommon.h movements/

# copy the sparki constants into the associated directories
rsync -av SparkiCommon/sparkiCommon.h determineWorld/
rsync -av SparkiCommon/sparkiCommon.h movementTest/
rsync -av SparkiCommon/sparkiCommon.h sensorTest/
rsync -av SparkiCommon/sparkiCommon.h localizationUtils/
rsync -av SparkiCommon/sparkiCommon.h movements/
rsync -av SparkiCommon/sparkiCommon.h ultrasonic/
