#!/bin/bash

echo "Enter IP Address"
read ipAddress

curl http://$ipAddress/config.json > ./main/data/config.json
