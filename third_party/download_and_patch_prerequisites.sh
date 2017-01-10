#!/bin/bash 

# Check if the prerequisite bash programs exist on the system.
for cmd in curl tar; do
  echo -n "Checking if ${cmd} is present ..."
  if ! hash ${cmd} 2>/dev/null; then
    echo ""
    echo "ERROR: Program ${cmd} is not installed on the system."
    exit 1
  else 
    echo "ok"
  fi
done

THIRD_PARTY_DIR=`pwd`
if [ "${PWD##*/}" != "third_party" ]; then
  echo "ERROR: This script can be run only from the third party directory"
  exit 1
fi

THIRD_PARTY_SRC_DIR=${THIRD_PARTY_DIR}/src 
if [ ! -d "$THIRD_PARTY_SRC_DIR" ]; then  
  mkdir -p ${THIRD_PARTY_SRC_DIR}
fi

PATCH_DIR=${THIRD_PARTY_DIR}/patches
cd src/

third_party_dir_names=("benchmark"
                       "gflags"
                       "googletest"
                       "linenoise"
                       "re2"
                       "gperftools"
                       )
                       
third_party_lib_urls=("https://github.com/google/benchmark/archive/v1.1.0.tar.gz"
                      "https://github.com/gflags/gflags/archive/v2.2.0.tar.gz"
                      "https://github.com/google/googletest/archive/release-1.8.0.tar.gz"
                      "https://github.com/antirez/linenoise/archive/1.0.tar.gz"
                      "https://github.com/google/re2/archive/2017-01-01.tar.gz"
                      "https://github.com/gperftools/gperftools/releases/download/gperftools-2.5/gperftools-2.5.tar.gz"
                      )
                      
downloaded_archive_names=("v1.1.0.tar.gz"
                          "v2.2.0.tar.gz"
                          "release-1.8.0.tar.gz"                          
                          "1.0.tar.gz"
                          "2017-01-01.tar.gz"
                          "gperftools-2.5.tar.gz"
                          )
                          
tar_options=("-xzf"
             "-xzf" 
             "-xzf"
             "-xzf"
             "-xzf"
             "-xzf"
             )

for ((lib_index=0; lib_index < ${#third_party_dir_names[*]}; lib_index++))
do
  # If the third party directory is not present, create it.
  if [ ! -d ${third_party_dir_names[lib_index]} ]; then
    mkdir ${third_party_dir_names[lib_index]}
  fi

  # Downaload the compressed archive for the third party library. 
  curl_cmd="curl -L -O ${third_party_lib_urls[lib_index]}"
  echo "Downloading from ${third_party_lib_urls[lib_index]} ..."  
  echo ${curl_cmd}
  eval ${curl_cmd}
  if [ -f ${downloaded_archive_names[lib_index]} ]; then
    echo "File ${downloaded_archive_names[lib_index]} downloaded successfully"

    # Uncompress the archive to its designated directory. 
    # The strip-components option will ensure that all the files directly end up
    # in the desired directory, without any intermediate hierarchy level. 
    tar_cmd="tar ${tar_options[lib_index]} ${downloaded_archive_names[lib_index]} -C ${third_party_dir_names[lib_index]} --strip-components=1"
    echo ${tar_cmd}
    echo "Extracting from ${downloaded_archive_names[lib_index]} ..."
    eval ${tar_cmd}

    # Delete the compressed archive. 
    rm -rf ${downloaded_archive_names[lib_index]}
  else
    echo "Error downloading file ${downloaded_archive_names[lib_index]} from ${third_party_lib_urls[lib_index]}"
  fi
done

# Apply patches now. 
cp ${PATCH_DIR}/linenoise/CMakeLists.txt ${THIRD_PARTY_SRC_DIR}/linenoise

# Apply re2 patch. 
cd ${THIRD_PARTY_SRC_DIR}/re2
patch -p0 < ${PATCH_DIR}/re2/re2CMake.patch
cd ${THIRD_PARTY_SRC_DIR}

# Apply benchmark patches.
cd ${THIRD_PARTY_SRC_DIR}/benchmark
patch -p0 < ${PATCH_DIR}/benchmark/benchmarkCMake.patch
cd ${THIRD_PARTY_SRC_DIR}/benchmark/src
patch -p0 < ${PATCH_DIR}/benchmark/benchmarkSrcCMakeLists.patch

# Back to the third_party directory. 
cd ${THIRD_PARTY_DIR}
