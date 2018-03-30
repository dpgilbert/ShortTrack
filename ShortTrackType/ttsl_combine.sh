#!/bin/bash

for dir in output*/*Lep*; do
    hadd -f ${dir}/ttsl.root ${dir}/ttsl_from*
done