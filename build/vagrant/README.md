Vagrant Development Environments for Quickstep
==============================================

Vagrant is a tool for setting up development environments in a VM. It
automatically takes care of fetching a base OS image, installing additional
software, and setting it up so you can easily ssh into a VM and get to work.
It also handles sharing directories (like source repos) between the host and
the VM so that you can, for instance, edit code using your preferred editor on
your machine as usual, and compile inside the Vagrant-managed VM without having
to do any additional steps to sync code. Cool!

The main reason to use Vagrant on a project like Quickstep is to have
well-known OS environments to build and test code in during development. For
instance, if you are developing on a Mac but are working on features related to
libnuma on Linux, you can use the Debian Vagrant box which gives you a full
Linux OS and toolchain with libnuma included.

Getting Started
---------------

1.   Download and install VirtualBox: https://www.virtualbox.org/wiki/Downloads
     VirtualBox is used as the hypervisor for Vagrant VMs (technically, Vagrant
     can use several different providers, but it's easiest to get up and
     running with VirtualBox).
2.   Download and install Vagrant: https://www.vagrantup.com/downloads.html
     Just install the Vagrant software itself. You do not need to go through
     any of the guides to set up a Vagrant box (we already have several
     pre-made for you, see "Quickstep Boxes" below).
3.   Bring up a Vagrant box. Just cd into the directory for the box and type
     `vagrant up`. Several boxes are provided for Quickstep development (see
     "Quickstep Boxes" below). The `debian-jessie-amd64` box is a good default
     choice. Running `vagrant up` may take a few minutes the first time you
     bring up a box, because vagrant has to first download a base OS image,
     then run scripts to install additional software in it. You can expect the
     boot process to go much faster after the first time.
4.   Type `vagrant ssh` to ssh into your VM. The Vagrant boxes for quickstep
     should have all the dev tools you need (compilers, cmake, protobufs, flex,
     bison, and debuggers). The quickstep source repo will be shared and
     mounted at /quickstep in the VM.
5.   When you are finished using the VM, exit from your ssh session and type
     `vagrant halt` to shut it down. This will gracefully shut down the VM and
     keep the full VM image around so that you can boot back into it quickly
     the next time you run `vagrant up`. If you want to completely eliminate
     the VM from your host and clean up everything, you can type
     `vagrant destroy`. You can still recreate the box by doing `vagrant up`,
     but it will start from scratch and take a few minutes.

Configuring VMs
---------------

The Vagrantfiles for our boxes are set up to give 4 CPU cores and 8 GB of RAM
to the VM. If your host machine is under or over-powered relative to that (and
keep in mind that your host OS needs some RAM, too), you can edit the
Vagrantfile to change the resources given to the VM. Look for a code section
like the following:

    config.vm.provider "virtualbox" do |vb|
      vb.memory = 8192
      vb.cpus = 4
    end

`vb.memory` indicates the VM RAM in megabytes, `vb.cpus` indicates the number
of CPU cores. Adjust to your liking.

Quickstep Boxes
---------------

### Debian Jessie

The debian-jessie-amd64 directory contains a vagrant box for Debian GNU/Linux 8
"Jessie". It has GCC 4.9, Clang 3.5, GDB, LLDB, CMake, protobufs, flex, bison,
and git installed.

### FreeBSD 10.2

The freebsd-10.2-amd64 directory contains a vagrant box for FreeBSD
10.2-STABLE. It has Clang 3.4 (the default system compiler) and Clang 3.7
(installed under `/usr/local/bin/clang37`), as well as LLDB, CMake, protobufs,
flex, bison, and git installed.

The FreeBSD box uses NFS to share files with the host. This means you need
administrator (i.e. sudoer) permissions on the host to export the shared
directories. Vagrant will automate setting this up for you, but it may ask
for a password at some point when you run `vagrant up`.

Note that the FreeBSD image auto-updates itself on boot before the SSH server
comes up, so do not be alarmed if you get a bunch of "remote connection
disconnect" warnings when you run `vagrant up`, it should eventually succeed.

### Ubuntu Precise

The ubuntu-precise-amd64 directory contains a vagrant box for Ubuntu 12.04 LTS
"Precise". This is the same distro as is used in Travis Linux workers for our
continuous integration setup, and the installed development packages should be
the same as on Travis. This box is intended mainly for debugging problems with
Travis. For regular Linux development, it's recommended to use the Debian
Jessie box instead, as it has a more recent and complete set of tools.
