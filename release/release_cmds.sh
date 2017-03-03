#!/usr/bin/env bash

# Functions for Releasing Quickstep
# Usage: Fill out your details in release_manager.profile.
#        ./release_cmds [create|publish|test]
#
# Note: This script is a scratch pad with most of the relevent commands. Modify 
#       it to see fit. We'll finalize it when we understand the process better.
#       
#       Everything in this script assumes that you are running in the release/ 
#       folder of your quickstep directory.
#
# PREREQS:
#  - full details on how to do a release is in a confluence article: How to Release
#  - You must have previously created a pgp key using your apache username
#    and uploaded it to a keyserver [http://quickstep.apache.org/release-signing/]
#

### Helper functions

create_artifacts() {
  # Updates submodules, archives everything, signs it using your key (requires 
  # user input), makes checksums, moves artifacts to release/ folder
  #

  export RELEASE_DIR=`pwd`
  cd ..
  export BASE_DIR=`pwd`

  # need the submodules to be included for the compile to work.
  # likely, this will be a no-op
  git submodule init
  git submodule update

  # you'll need to push this if you want the tag to be visible to committers
  # the tag is necesary for the archiving to work correctly
  git tag -a rc-$VERSION -m 'release candidate $VERSION'
  git archive --format "tar" --prefix=$PROJECT_NAME-$VERSION/ -o $PROJECT_NAME-$VERSION.tar rc-$VERSION
  git submodule foreach --recursive 'git archive --verbose --prefix=$PROJECT_NAME-$VERSION/$path/ --format tar master --output $BASE_DIR/submodule-$sha1.tar'
  if [[ $(ls | grep submodule-*.tar | wc -l) != 0  ]]; then
    # combine all archives into one tar
    tar --concatenate --file $PROJECT_NAME-$VERSION.tar submodule-*.tar
    # remove sub tars
    rm -rf submodule-*.tar
  fi

  # gzip final tar
  gzip --force --verbose $PROJECT_NAME-$VERSION.tar

  # Make the signature. This requires human input
  gpg -u $APACHE_USERNAME@apache.org --armor --output $PROJECT_NAME-$VERSION.tar.gz.asc --detach-sign $PROJECT_NAME-$VERSION.tar.gz
  # Make hashes
  md5sum $PROJECT_NAME-$VERSION.tar.gz > $PROJECT_NAME-$VERSION.tar.gz.md5
  sha1sum $PROJECT_NAME-$VERSION.tar.gz > $PROJECT_NAME-$VERSION.tar.gz.sha


  # Make sure these three artifacts are good
  gpg --verify $PROJECT_NAME-$VERSION.tar.gz.asc
  md5sum --check $PROJECT_NAME-$VERSION.tar.gz.md5
  sha1sum --check $PROJECT_NAME-$VERSION.tar.gz.sha

  mv $PROJECT_NAME-$VERSION.tar.gz* $RELEASE_DIR

  cd $RELEASE_DIR
}

publish_candidate() {
  # push a RC to subversion. We use SVN sparse directories so as not to pull
  # every artifact ever. Assumes that you have already created the artifacts
  # and they reside in the release/ folder.
  # directory layout is x.y.z/RCw, where w is the release candidate number
  #

  if [ ! -d "$SVN_DEV" ]; then
    svn checkout --depth immediates $SVN_DEV_URL $SVN_DEV
  fi

  BASE_DIR=`pwd`
  cd $SVN_DEV

  if [ ! -d "$VERSION" ]; then
    mkdir $VERSION
    cd $VERSION
  else
    cd $VERSION
    svn update --set-depth immediates .
  fi

  # now in $VERSION folder

  RCFOLDER=RC$CANDIDATE
  mkdir $RCFOLDER
  cd $RCFOLDER
  cp $BASE_DIR/$PROJECT_NAME-$VERSION.tar.gz* ./
  cd ../..

  svn add $VERSION/$RCFOLDER
  svn commit --username=$APACHE_USERNAME -m "Quickstep-$VERSION RC$CANDIDATE"

  cd $BASE_DIR
}

test_candidate() {
  # This is best run in /tmp
  # tries checking out a release candidate and building
  #

  svn checkout --depth immediates $SVN_DEV_URL $SVN_DEV
  cd $SVN_DEV

  # add the KEYS file from quickstep to your gpg key ring
  gpg --import KEYS

  cd $VERSION
  svn update --set-depth infinity RC$CANDIDATE
  cd RC$CANDIDATE

  tar -xzf $PROJECT_NAME-$VERSION.tar.gz

  # verify artifacts
  gpg --verify $PROJECT_NAME-$VERSION.tar.gz.asc
  md5sum --check $PROJECT_NAME-$VERSION.tar.gz.md5
  sha1sum --check $PROJECT_NAME-$VERSION.tar.gz.sha

  # now build the release artifacts
  cd $PROJECT_NAME-$VERSION

  # first download third_party, then do your normal build
  cd third_party/
  ./download_and_patch_prerequisites.sh
  cd ..
  cd build
  cmake ..
  make
}

usage() {
  echo "usage: $0 [create|publish|test]"
  exit $1
}

### main

source release_manager.profile
if [ -z $APACHE_USERNAME ] || [ -z $VERSION ] || [ -z $CANDIDATE ]; then 
  echo "please set release_manager.profile"
  exit 1
fi

alias svn="svn --non-interactive"

set -e

if [ -z $1 ] ; then
  usage 0
fi

echo "user: $APACHE_USERNAME"
echo "version: $VERSION"
echo "candidate: $CANDIDATE"

if [ "create" == "$1" ] ; then
  echo "creating artifacts ..."
  create_artifacts
elif [ "publish" == "$1" ] ; then
  echo "publishing candidate artifacts ..."
  publish_candidate 
elif [ "test" == "$1" ] ; then
  echo "testing candidate artifacts ..."
  test_candidate
else
  usage 0
fi
