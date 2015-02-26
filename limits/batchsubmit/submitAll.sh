#!/bin/bash

for i in `ls condor*`; do condor_submit $i; done
