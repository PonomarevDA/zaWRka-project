#!/bin/bash

HEAD_ADDR=$NUC_ADDR
HEAD_USER=user-sau-nuc
rsync -avzPc ../wr8_gui_server/ $HEAD_USER@$HEAD_ADDR:~/catkin_ws/src/wr8_gui_server/
