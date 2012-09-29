#!/bin/bash
#-----------------------------------------------------------------------------
#
# \brief		Helper script to create git version information header file
#
#				The ouput is written to stdout.
#				Examples of the generated defines:
#
#						#define GIT_LONG "v0.4.2-7-gedf2fd7-dirty+"
#						#define GIT_TAG "v0.4.2"
#						#define GIT_SHA  "edf2fd7"
#						#define GIT_SHORT  "v0.4.2-7+"
#						#define GIT_REV  "v0.4.2-7-gedf2fd7-dirty"
#						#define GIT_BRANCH  "310-implement_phonybl"
#						#define VERSION_CODE 0x0004020F
#						// MAJOR: 0; MINOR: 4; PATCH: 2; DIST: 7; DISTCODE: 15
#
#				The macro "VERSION_CODE" is an integer that can be stored to an
#				EEPROM in order to detect firmware updates in MCUs.
#
#				The script also iterates through all submodules and generates
#				defines like 'GIT_LONG_<submodule>'.
#
# \author	Maximilian Seesslen <mes@seesslen.net>
#
#-----------------------------------------------------------------------------


if [ ! -e ".git" ]; then
   cd ..
fi


helpExit()
{
   echo "usage: $0 [-t]   outputs git repositories versions"
   exit
}

subDirectory()
{
   submodule=$1
   project=$2
   
   pushd $submodule &> /dev/null
   
      DIRTY="$(git diff --quiet --exit-code || echo +)"
      TAG="$(git describe --tags  --abbrev=0 --match 'v[0-9]*.[0-9]*.[0-9]*')"
      SHA="$(git log --pretty=format:'%h' -n 1)"
      DISTANCE="$(git rev-list HEAD ^${TAG} --count)"
      
      if [ "$format" == "text" ]; then
         echo "$submodule: \"$(git describe --tags --dirty --long --match 'v[0-9]*.[0-9]*.[0-9]*')$CHANGE\""
      else
         calculateVersionCode
         echo "#define GIT_LONG_$project \"$(git describe --tags --dirty --long --match 'v[0-9]*.[0-9]*.[0-9]*')$CHANGE\""
         echo "#define GIT_SHA_$project \"$(git log --pretty=format:'%h' -n 1)\""
         echo "#define GIT_SHA_INT_$project  0x$(git log --pretty=format:'%h' -n 1)"
         echo "#define GIT_SHORT_$project \"${TAG}-${DISTANCE}${DIRTY}\""
         echo "#define GIT_REV_$project \"$(git describe --tags --dirty --match 'v[0-9]*.[0-9]*.[0-9]*')\""
         echo "#define GIT_BRANCH_$project \"$(git symbolic-ref --short HEAD)\""
         echo "#define VERSION_CODE_$project ${VERSION_CODE}"
         echo
      fi
      
   popd &> /dev/null
}

calculateVersionCode()
{
   V="${TAG#v*}"
   MAJOR="${V%%.*}"
   V="${V#*.}"
   MINOR="${V%%.*}"
   V="${V#*.}"
   PATCH="${V%%-*}"
   V="${V#*-}"
   
   ADDDIRTY=0
   git diff --quiet --exit-code || ADDDIRTY=1
   if [ ! -z "$DISTANCE" ]; then
      DISTCODE=$(($DISTANCE<<2))
      #if [ -z "$DISTCODE" ]; then
      #   DISTCODE="0xF0"
      #fi
      let MAX_DISTCODE=0xF0
      if [ "${DISTCODE}" -gt "${MAX_DISTCODE}" ]; then
         echo "#warning Error: Distance to version too high. Consider taging a new version ($PWD)"
         echo "// TAG: ${TAG}"
         echo "// DISTCODE: ${DISTCODE}"
         DISTCODE="${MAX_DISTCODE}"
      fi
   else
      MAJOR=0
      MINOR=0
      PATCH=0
      DISTCODE=0xFC
   fi
   
   let DISTCODE+=ADDDIRTY
   VERSION_CODE=$(printf "0x%02X%02X%02X%02X" $MAJOR $MINOR $PATCH $DISTCODE)
}


format=""

while [ "$#" -ge "1" ]; do
    case "$1" in
    -h)
        helpExit
        ;;
    -t)
        format="text"
        ;;
    --)
        shift
        break
        ;;
     *)
         echo "Unknown argument: $1"
         errorExit
         break
         ;;
    esac
    shift
done


DIRTY="$(git diff --quiet --exit-code || echo +)"
TAG="$(git describe --tags  --abbrev=0 --match 'v[0-9]*.[0-9]*.[0-9]*')"
SHA="$(git log --pretty=format:'%h' -n 1)"
SHORT_SHA="${SHA:0:4}"
DISTANCE="$(git rev-list HEAD ^${TAG} --count)"
PROJECT="$(basename $PWD)"
PROJECT="${PROJECT//-/_}"
PROJECT_CODE="0x$(echo -n ${PROJECT} | shasum | head -c 8)"

if [ "$format" == "text" ]; then
   echo "${PROJECT}: \"$(git describe --tags --dirty --long --match 'v[0-9]*.[0-9]*.[0-9]*')\""
else
   echo "#define GIT_LONG \"$(git describe --tags --dirty --long --match 'v[0-9]*.[0-9]*.[0-9]*')\""
   echo "#define GIT_TAG \"${TAG}\""
   echo "#define GIT_SHA  \"$(git log --pretty=format:'%h' -n 1)\""
   echo "#define GIT_SHA_INT  0x$(git log --pretty=format:'%h' -n 1)"
   echo "#define GIT_SHORT  \"${TAG}-${DISTANCE}${DIRTY}\""
   echo "#define GIT_SEMI   \"${TAG}-${DISTANCE}-g${SHORT_SHA}${DIRTY}\""
   echo "#define GIT_SEMI_TAIL   \"${DISTANCE}-g${SHORT_SHA}${DIRTY}\""
   echo "#define GIT_REV  \"$(git describe --tags --dirty --match 'v[0-9]*.[0-9]*.[0-9]*')\""
   echo "#define GIT_BRANCH  \"$(git symbolic-ref --short HEAD)\""
   echo "#define GIT_PROJECT  \"${PROJECT}\""
   if [ ! -z "${DIRTY}" ]; then
      echo "#define GIT_DIRTY 1"
   fi
   echo "#define GIT_PROJECT  \"${PROJECT}\""
fi

calculateVersionCode

if [ "$format" == "text" ]; then
   echo -n
else
   echo "#define VERSION_CODE $VERSION_CODE"
   echo "#define VERSION_CODE_${PROJECT} $VERSION_CODE"
   echo "#define PROJECT_CODE_${PROJECT} $PROJECT_CODE	// SHA over project name."
   echo "// MAJOR: $MAJOR; MINOR: $MINOR; PATCH: $PATCH; DIST: $DISTANCE; DISTCODE: $DISTCODE"
   
   if [ "$DISTCODE" == "0" ]; then
       echo "#define GIT_COND GIT_REV"
   else
       echo "#define GIT_COND GIT_LONG"
   fi
   echo
fi

subDirectory . ${PROJECT}

BASE_PROJECT=$PWD
submodules=$(git submodule foreach --quiet 'basename $PWD')
for submodule in $submodules
do
   if [ ! -e "$submodule" ]; then
      echo "Submodule not found: $submodule" >&2
      echo "Current directory: $PWD" >&2
      echo "Base project diry: $BASE_PROJECT" >&2
      exit 22
   fi
   subDirectory $submodule $submodule
done


#---fin-----------------------------------------------------------------------
