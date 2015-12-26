#!/bin/bash
curl $1/?play_game=true --data "{\"name\":\"$2\",\"board\":\"rrrrrrrrr_rr_r______bbbbbbbbbbbb\"}"
echo \"\"
sleep 0.1
curl  $1/?play_game=true --data "{\"name\":\"$2\",\"board\":\"rrrrrrrrr_rr_r____b_bb_bbbbbbbbb\"}"
echo \"\"
sleep 0.1
curl  $1/?play_game=true --data "{\"name\":\"$2\",\"board\":\"rrrrrrrrr_rr____r_b_bb_bbbbbbbbb\"}"
echo \"\"
sleep 0.1
curl  $1/?play_game=true --data "{\"name\":\"$2\",\"board\":\"rrrrrrrrr_rrb_____b_b__bbbbbbbbb\"}"
echo \"\"
