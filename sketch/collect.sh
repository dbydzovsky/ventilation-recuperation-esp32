#!/bin/bash

echo "Enter IP Address"
read ipAddress

curl http://$ipAddress/a/settings/ > ./kolektor2/data/settings.json
curl http://$ipAddress/a/conf/ > ./kolektor2/data/config.json
