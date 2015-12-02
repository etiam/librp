#!/usr/bin/env python

import os, sys

def appendenv(var, val):
    orig = os.environ[var]
    os.environ[var] = '%s:%s' % (orig, val)

def appendpkgconfig(val):
    global home, builddir
    path = os.path.join(workspacedir, val,  builddir, 'install/lib/pkgconfig')
    appendenv('PKG_CONFIG_PATH', path)
    print 'appended %s to PKG_CONFIG_PATH' % (path)

workspacedir = os.path.dirname(os.getcwd())
projectdir = os.getcwd()
build = sys.argv[1]
arch = os.environ['ARCH']
home = os.environ['HOME']
pkg_config_path = os.environ['PKG_CONFIG_PATH']

builddir = 'build-%s-%s' % (build, arch)
if (not(os.path.exists(builddir))):
    os.makedirs(builddir)

prefix = os.path.join(projectdir, builddir, 'install')

options  = '--enable-maintainer-mode --enable-silent-rules '
options += ' '.join(sys.argv[2:])

cmd  = 'cd {builddir}; ../configure -C CXX="ccache-clang++" CC="ccache-clang" \
        --prefix={prefix} {options} '.format(**globals())

cmd += '; cd ..'

appendpkgconfig('libut')

os.system(cmd)
