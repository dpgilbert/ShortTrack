#!/bin/bash

for dir in output*/*; do
    hadd -f ${dir}/ttsl.root ${dir}/ttsl_from*
done