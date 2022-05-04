#!/bin/bash

echo "Enter IP Address"
read ipAddress

function load() {
  echo "Parsing $1"
  file_content=$(curl -s "http://$ipAddress/$1")
  file_target="./kolektor2/data/$1"
  [[ -f "$file_target" ]] && rm $file_target
  echo $file_content | jq -e . >/dev/null 2>&1
  if [[ $? -eq 0 ]]; then
    echo "file $1 is a JSON"
    echo $file_content > "$file_target"
  fi
}
echo "Parsing settings.json"
curl -s http://$ipAddress/a/settings/ | jq . > ./kolektor2/data/settings.json
echo "Parsing config.json"
curl -s http://$ipAddress/a/conf/ | jq . > ./kolektor2/data/config.json

load "blockvent"
load "blockrecu"
load "ventilator.json"
load "recuperation.json"
