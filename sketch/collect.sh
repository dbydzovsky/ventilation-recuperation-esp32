#!/bin/bash

echo "Enter IP Address"
read ipAddress

curl http://$ipAddress/a/settings/ | jq . > ./kolektor2/data/settings.json
curl http://$ipAddress/a/conf/ | jq . > ./kolektor2/data/config.json
