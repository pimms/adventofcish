#!/bin/bash

for i in {1..25}
do
    if [ ! -d dec$i ];
    then
        mkdir dec$i

        sed s/TEMPLATE/dec$i.c/g run > dec$i/run
        chmod +x dec$i/run

        cp templ.c dec$i/dec$i.c
    fi
done
