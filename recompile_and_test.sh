#! /usr/bin/env bash
rm LinearTree.so

gcc -shared -o LinearTree.so LinearTree.c

python3 tests.py
