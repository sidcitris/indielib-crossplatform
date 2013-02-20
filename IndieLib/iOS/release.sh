#bin/sh

TUTORIALSDIR="../tutorials/iOS"
DESTDIR="../../../"

echo $1
if [ -z "$1" ] 
then
echo "EXPECTED ARCHIVE VERSION INPUT PARAMETER"
exit -1
fi

ARCHIVENAME="Indielib_OSX_$1"
echo $ARCHIVENAME

RUNDIR=`pwd`

echo "WORKING FROM: $RUNDIR"
echo "TUTORIALS RELATIVE DIR: $TUTORIALSDIR"
echo "DESTINATION DIRECTORY: $DESTDIR"

./buildDependencies.sh

#clean all targets first
cd $RUNDIR

echo "************CLEAN ALL TARGETS*******************"
ALLSCHEMES=$(xcodebuild -workspace IndielibiOS.xcworkspace -list)

for SCHEME in $ALLSCHEMES
do
xcodebuild -workspace IndielibiOS.xcworkspace -scheme $SCHEME clean
done

#build indielib
echo"*************BUILD INDIELIB**********************"
xcodebuild -workspace IndielibiOS.xcworkspace -scheme IndieLib -configuration release

#no tutorials built. Packaged as source code

#package distributable files in zip
echo"*************PACKAGE FILES************************"
cd $DESTDIR
rm $ARCHIVENAME.zip

#-X Option not to add hidden files .* in OSX
zip -r -X $ARCHIVENAME.zip Indielib/bin/mac
zip -r -X $ARCHIVENAME.zip Indielib/common/include
zip -r -X $ARCHIVENAME.zip Indielib/resources
zip -r -X $ARCHIVENAME.zip Indielib/tutorials --exclude "Indielib/tutorials/win/*" "Indielib/tutorials/linux/*"
zip -r -X $ARCHIVENAME.zip Indielib/docs --exclude "Indielib/docs/Doxygen/Doxyfile" "Indielib/docs/wiki/*"
#zip $ARCHIVENAME.zip Indielib/LICENSE
