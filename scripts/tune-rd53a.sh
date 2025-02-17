#!/bin/bash

if [ "$#" -lt 6 ]; then
    echo "Usage: $0 <first_target_threshold> <second_target_threshold> <target_charge> <target_tot> <controller_config.json> <config_file.json> [<config_file2.json> ..] " >&2
    exit 1
fi

./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/std_digitalscan.json -p -m 1
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/std_analogscan.json -p
#./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/std_thresholdscan.json -p -m
#./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/std_totscan.json -p -m 0 -t $3

# Tune diff FE
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/diff_tune_globalthreshold.json -t $2
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/diff_tune_globalpreamp.json -t $3 $4
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/diff_tune_globalthreshold.json -t $2
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/diff_tune_pixelthreshold.json -t $2 -p
#./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/diff_tune_finepixelthreshold.json -t $2 -p
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/diff_analogscan.json -p -m 1
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/diff_noisescan.json -p

# Tune lin FE
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_tune_globalthreshold.json -t $1
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_tune_pixelthreshold.json -t $1
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_retune_globalthreshold.json -t $2
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_retune_pixelthreshold.json -t $2
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_tune_globalpreamp.json -t $3 $4
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_retune_pixelthreshold.json -t $2 -p
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_tune_finepixelthreshold.json -t $2 -p
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_analogscan.json -p -m 1
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_noisescan.json -p

# Tune sync FE
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/syn_tune_globalthreshold.json -t $1
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/syn_tune_globalpreamp.json -t $3 $4
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/syn_tune_globalthreshold.json -t $1 -p
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/syn_analogscan.json -p -m 1
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/syn_noisescan.json -p

# Finals threshold & tot scans
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/std_analogscan.json -p -m 1
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/syn_totscan.json -p -t $3
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_totscan.json -p -t $3
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/diff_totscan.json -p -t $3
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/syn_thresholdscan.json  -p
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/diff_thresholdscan.json -p
./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/lin_thresholdscan.json  -p

# Create final mask for operation
#./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/std_digitalscan.json -p -m 1
#./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/std_analogscan.json -p
#./bin/scanConsole -r $5 -c ${@:6} -s configs/scans/rd53a/std_noisescan.json -p
