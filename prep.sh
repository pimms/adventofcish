#!/bin/bash

i=$1

if [ ! -d dec$i ];
then
    mkdir dec$i

    sed s/TEMPLATE/dec$i.c/g run > dec$i/run
    chmod +x dec$i/run

    cp templ.c dec$i/dec$i.c
fi
