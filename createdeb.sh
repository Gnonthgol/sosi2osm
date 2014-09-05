#!/bin/bash

PACKAGENAME=sosi2osm
VERSION=20140707git634fc65
rm -rf createdeb/
mkdir createdeb
#rm -rf createdeb/$PACKAGENAME-$VERSION
mkdir createdeb/$PACKAGENAME-$VERSION
git archive master | tar -x -C createdeb/$PACKAGENAME-$VERSION
cd createdeb
#tar -xvf $PACKAGENAME\_$VERSION.orig.tar.gz
tar -czvf $PACKAGENAME\_$VERSION.orig.tar.gz $PACKAGENAME-$VERSION
cp -r -v ../debian $PACKAGENAME-$VERSION/
cd $PACKAGENAME-$VERSION
dpkg-buildpackage
