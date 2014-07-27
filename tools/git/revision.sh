#!/bin/sh

# Run this script from time to time to update revision.txt.

branch=`git branch | sed -n 's:\* ::p'`

# generate list of relevant commits
git log --decorate --date=short --pretty=format:"%d%n%nCommit %h on %cd by %cn %n%n%n%w(0,4,4)%B%n" > revision.txt

date=`sed -n 's/Commit [0-9a-zA-Z]* on \([-0-9]*\) .*/\1/p' revision.txt | head -n1`

# split up ref name lists into single lines
while grep -e '^ ([^,)]*,' revision.txt >/dev/null
do
  echo -n "."
  sed 's/^ (\([^,)]*\),\s/ (\1)\n (/g' revision.txt > revision.txt~
  mv -f revision.txt~ revision.txt
done
echo

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

# add header
(
  echo $divider
  echo "UberPOV \"$branch\" Branch Revision History - $date"
  echo $divider
  echo "Note: the following list is sorted purely chronologically; changes listed"
  echo "below a certain version tag were developed before that version's release, but"
  echo "may or may not have been included in it. All changes listed are, however,"
  echo "included in this particular version."
  echo $divider
  cat revision.txt
) > revision.txt~
mv -f revision.txt~ revision.txt

# collapse consecutive empty lines
sed -r ':a; /^\s*$/ {N;ba}; s/( *\n){2,}/\n/' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt

# remove trailing whitespace
sed 's/\s*$//g' revision.txt > revision.txt~
mv -f revision.txt~ revision.txt

echo Done.
