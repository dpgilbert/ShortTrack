#!/bin/bash

for dir in output_*/incl; do
    hadd -f ${dir}/ttsl.root ${dir}/ttsl_from*
done