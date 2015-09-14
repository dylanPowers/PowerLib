#!/bin/bash

if [ ! -e lib/gtest/.git ]; then
  mkdir lib
  git clone https://chromium.googlesource.com/external/googletest lib/gtest
else
  cd lib/gtest && git pull
fi
