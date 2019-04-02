#!/usr/bin/python
import os, sys

ch = ['', 'jecup', 'jecdown', 'jerup', 'jerdown',
      'hdampup', 'hdampdown', 'TuneCP5up', 'TuneCP5down',]

for chs in ch:
  if not os.path.exists( "2017/root_" + chs ):
    os.makedirs( "2017/root_" + chs )
  if not os.path.exists( "2017/hdf_" + chs ):
    os.makedirs( "2017/hdf_" + chs )
  if not os.path.exists( "2018/root_" + chs ):
    os.makedirs( "2018/root_" + chs )
  if not os.path.exists( "2018/hdf_" + chs ):
    os.makedirs( "2018/hdf_" + chs )


def nfile(dirname):
  tmp = []
  allfiles = os.listdir(dirname)
  for files in allfiles:
    if files.endswith(".root") or files.endswith(".h5"): tmp.append(files)
  print dirname + ": " + str(len(tmp)).rjust(30-len(dirname))

for chs in ch:
  nfile("2017/root_" + chs)
for chs in ch:
  nfile("2017/hdf_" + chs)
for chs in ch:
  nfile("2018/root_" + chs)
for chs in ch:
  nfile("2018/hdf_" + chs)