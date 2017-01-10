THIRD_PARTY_DIR=`pwd`
if [ "${PWD##*/}" != "third_party" ]; then
  echo "ERROR: This script can be run only from the third party directory"
  exit 1
fi

THIRD_PARTY_SRC_DIR=${THIRD_PARTY_DIR}/src
cd ${THIRD_PARTY_SRC_DIR}

third_party_dir_names=("benchmark"
                       "gflags"
                       "googletest"
                       "linenoise"
                       "re2"
                       "gperftools"
                       )

for ((lib_index=0; lib_index < ${#third_party_dir_names[*]}; lib_index++))
do
  rm -rf ${third_party_dir_names[lib_index]}/
done
