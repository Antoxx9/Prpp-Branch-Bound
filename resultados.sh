#!/bin/bash

search_dir=$1
for entry in "$search_dir"/*
do 
  echo "$line" | timeout 4500s ./Branch2 $entry &

  [ $(jobs | wc -l) -ge 7 ] && wait
  
done