#!/bin/sh

# Run this script after creating a new GitFlow release branch.
# The branch name must be "release/v<VERSION>", where <VERSION> is the actual version string.

branch=`git branch | sed -n 's:\* ::p'`

vversion=`echo "$branch" | sed -n 's:^release/::p'`
if [ x"$vversion" == x"" ]
then
  echo "Current branch is not a release branch: '$branch'."
  exit 1
fi

version=`echo "$vversion" | sed -n 's:^v::p'`
if [ x"$version" == x"" ]
then
  echo "Improper release branch name format: '$vversion'."
  exit 1
fi

rversion=`echo "$version" | sed -n 's:-.*::p'`
if [ x"$rversion" == x"$version" ]
then
  pversion=""
  echo "Identified version as '$version' release."
else
  pversion=`echo "$version" | sed -n 's:[^-]*-::p'`
  echo "Identified version as '$rversion' prerelease '$pversion'."
fi

echo "Updating 'unix/VERSION'..."
echo "$version" >unix/VERSION
git add "unix/VERSION"

echo "Updating 'source/base/branch.h'..."
sed -i 's:\(#define BRANCH_VERSION *"\)[^"]*:\1'"$rversion"':g' source/base/branch.h >/dev/null 2>/dev/null
sed -i 's:\(#define BRANCH_PRERELEASE *"\)[^"]*:\1'"$pversion"':g' source/base/branch.h >/dev/null 2>/dev/null
git add "source/base/branch.h"

echo "Creating commit..."
git commit -m"preparing v$version"

echo Done.
