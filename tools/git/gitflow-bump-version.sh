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

echo "Identified version as '$version'."

echo "Updating 'unix/VERSION'..."
echo "$version" >unix/VERSION
git add "unix/VERSION"

echo "Updating 'source/base/branch.h'..."
sed 's:\(#define BRANCH_VERSION          "\)[^"]*:\1'"$version"':g' source/base/branch.h > source/base/branch.h~
mv -f source/base/branch.h~ source/base/branch.h
git add "source/base/branch.h"

echo "Creating commit..."
git commit -m"preparing v$version"

echo Done.
