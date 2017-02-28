# Profile for Releasing Quickstep
#
# This script is for release managers. It is to be used in conjunction with 
# the release_cmds script.
#
# Usage: Fill out this file with your apache details. Then source this file.
#        Sourcing the release_cmds file will automatically source this.
#

# you must be registered with apache
export APACHE_USERNAME=

# quickstep version, Major.Minor.Increment
export VERSION=

# release candidate number. For a new version, this starts at 0
export CANDIDATE=

# folders corresponding to SVN repos where we keep our release artifacts
export SVN_DEV=svn-quickstep-dev
export SVN_REL=svn-quickstep-rel

# you probably will not need to change things below this line
export PROJECT_NAME=apache-quickstep-incubating

# path to apache repo with quickstep releases

export SVN_DEV_URL=https://dist.apache.org/repos/dist/dev/incubator/quickstep
export SVN_REL_URL=https://dist.apache.org/repos/dist/rel/incubator/quickstep