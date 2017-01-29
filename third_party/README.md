# Third-Party Libraries

This directory includes various open-source third-party code that is used by
Quickstep. Here's the description of the files:

`download_and_patch_prerequisites.sh` - Downloads the third party library source codes from their respective repositories and applies Quickstep specific patches.<br/>
`reset_third_party_dir.sh` - Removes the downloaded and patched third party and resets the `third_party` directory.<br/>
`patches/` - Contains the patch files applied on the original third party source code files.<br/>
`src/` - Contains the patched third party source code.<br/>

With the exception of the code in the `tmb`, `iwyu`, and the `protobuf_cmake`
directories (which are part of the Quickstep project itself), all other libraries
belong to their original authors and are governed by their respective licenses.
