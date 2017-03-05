## Release Helper Scripts

A full guide to releases can be found on [confluence][cwiki-howto]. In addition, you'll need to understand key signing and publish a PGP key using your apache email. PGP key generation is discussed [here][pgp-keys].

Non managers can use the script to run a test of the release artifacts.

### Preq's For Script Usage

You must fill out `release_manager.profile` first.

You'll need these tools:
  * md5sum
  * shasum
  * svn
  * gpg
  * tar
  * gzip

This is of course in addition to the regular build tools.

### Script Overview

**For non-release managers**

```bash
# Test a candidate
# You will run this if you are a non-release manager testing a release.
./release_cmds.sh test
```

**For release managers**

```bash
# Create candidate artifacts
# (did you set the .profile file?)
./release_cmds.sh create

# Publish candidate artifacts
# (did you first create and test the release artifacts?)
./release_cmds.sh publish
```

### Dockerfile

We've included a Dockerfile which can be used as a platform standard for testing. That is, many of us develop on Mac, so it's convenient to be able to test on Ubuntu without leaving your machine. This is for purely for convenience. You can test on any supported platform.

Basic docker usage is shown below.
```bash
# cd to release directory

# build the image from the Dockerfile
docker build -t quickstep-dev .

# enter a fresh docker container
docker run -ti quickstep-dev bash

# now you are in the container. Here you can clone quickstep and run the release scripts for testing

```

[cwiki-howto]: https://cwiki.apache.org/confluence/display/QUICKSTEP/How+To+Release
[pgp-keys]: http://quickstep.apache.org/release-signing/