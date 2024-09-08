#!/bin/sh

rm levels/*.json

for json in RawLevel/*; do
    ./gmap $json
done