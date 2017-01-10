# This is a regular python file which is loaded by the helper script. It then
# looks for the CONFIG object. CONFIG object is a dict, which can optionally
# contain three entries which are lists: 'system-includes', 'user-includes',
# 'mappings', and one entry 'ignore-defaults' if set to True will make the help
# ignore the default system includes, user includes, and mappings in the helper
# and will use the ones defined here (otherwise these configurations will be
# added to default configuration).

# Paths can be relative to quickstep directory (starting with './'), or
# absolute (starting with '/').
CONFIG = {
  # List of system includes. IWYU does not pick up system includes correctly
  # some times.
  'system-includes': [
      '/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include',
      '/home/user/local/include',
  ],

  # List of user includes. IWYU does not pick up system includes correctly
  # some times.
  'user-includes': [
      './build/third_party/protobuf/include',
  ],

  # List of IWYU mappings to use. See include-what-you-use documentation for
  # format and need for mappings.
  'mappings': [
      'obscure-platform.imp',
  ],

  # List of custom compiler args like defines.
  'args': [
      '-DQUICKSTEP_DEBUG',
  ],

  # Boolean (defaults to False) make the helper ignore default configurations.
  'ignore-defaults': True,
}
