#!/bin/sh

# Run this script from time to time to update revision.txt.

branch=`git branch | sed -n 's:\* ::p'`

# first, generate a list of version tags (including old "beta" branch)
betaBranch="v1.37.0.0-beta.3"
releases=`git log --decorate --date=short --pretty=format:"%d" master $betaBranch | sed -n 's/^.*tag: \(v1[.]3[^,)]*\).*$/\1/p'`

# generate list of relevant commits
echo -n "Fetching revision data from Git repo"
current="HEAD"
rm revision.txt
for prev in $releases
do
  echo -n "."
  git log --decorate --date=short --pretty=format:"%d%n%nCommit %h on %cd by %cn %n%n%n%w(0,4,4)%B%n" "$current" "^$prev" >> revision.txt
  current="$prev"
done
git log --decorate --date=short --pretty=format:"%d%n%nCommit %h on %cd by %cn %n%n%n%w(0,4,4)%B%n" "$current" >> revision.txt
echo "Done."

date=`sed -n 's/Commit [0-9a-zA-Z]* on \([-0-9]*\) .*/\1/p' revision.txt | head -n1`

# split up ref name lists into single lines
echo -n "Isolating ref names"
while grep -e '^ ([^,)]*,' revision.txt >/dev/null
do
  echo -n "."
  sed 's/^ (\([^,)]*\),\s/ (\1)\n (/g' revision.txt > revision.txt~
  mv -f revision.txt~ revision.txt
done
echo "Done."

# prettify the tag ref names
divider='------------------------------------------------------------------------------'
sed 's/^ (tag:\s\(v1\.3[^)]*\))/'$divider'\nUberPOV \1\n'$divider'/g' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt
sed 's/^ (tag:\s\(v3\.[^)]*\))/'$divider'\nPOV-Ray \1\n'$divider'/g' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt
sed 's/^ (tag:\s\([^)]*\))/'$divider'\n??? \1\n'$divider'/g' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt

# eliminate any other ref names
sed 's/^ ([^)]*)$//g' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt

# collapse consecutive empty lines
sed -r ':a; /^\s*$/ {N;ba}; s/( *\n){2,}/\n/' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt

# remove trailing whitespace
sed 's/\s*$//g' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt

# identifying known authors
sed 's/by chris20$/by Chris Cason (chris20)/g' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt
sed 's/by c-lipka$/by Christoph Lipka (c-lipka)/g' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt

# word wrap after 100 characters
echo -n "Performing word wrapping"
maxText="...................................................................................................."
sed '/^    / s/^\('"$maxText"'\)/\1:|:/g' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt
while grep -e ":|:" revision.txt >/dev/null
do
  echo -n "."
  sed 's/^[ ~]\([ -]*[^ ]*\):|:/ \1/g' revision.txt > revision.txt~
  mv -f revision.txt~ revision.txt
  sed '/^[ ~]   [^ -]/ s/ \([^ ]*\):|:/\n~   \1/g' revision.txt > revision.txt~
  mv -f revision.txt~ revision.txt
  sed '/^[ ~]   - [^ ]/ s/ \([^ ]*\):|:/\n~     \1/g' revision.txt > revision.txt~
  mv -f revision.txt~ revision.txt
  sed '/^[ ~]     [- ] [^ ]/ s/ \([^ ]*\):|:/\n~       \1/g' revision.txt > revision.txt~
  mv -f revision.txt~ revision.txt
  sed '/^~/ s/^\('"$maxText"'\)/\1:|:/g' revision.txt > revision.txt~
  mv -f revision.txt~ revision.txt
  sed 's/^~/ /g' revision.txt > revision.txt~
  mv -f revision.txt~ revision.txt
done
echo "Done."

# add header
(
  echo $divider
  echo "UberPOV Revision History"
  echo $divider
  cat revision.txt
) > revision.txt~
mv -f revision.txt~ revision.txt
