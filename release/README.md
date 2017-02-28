## Scripts to help release Quickstep

### Preqs

You must fill out `release_manager.profile` first.

You'll need:
  * md5sum
  * shasum
  * svn
  * gpg
  * tar
  * gzip

This is of course in addition to the regular build tools.

### Usage Overview

```
# Test a candidate
./release_cmds.sh test

# Create candidate artifacts
./release_cmds.sh create

# Publish candidate artifacts (after you've created them)
./release_cmds.sh publish
```

* A full guide to releases can be found on [confluence][cwiki-howto].
* PGP key generation is discussed [here][pgp-keys]

[cwiki-howto]: https://cwiki.apache.org/confluence/display/QUICKSTEP/How+To+Release
[pgp-keys]: http://quickstep.apache.org/release-signing/