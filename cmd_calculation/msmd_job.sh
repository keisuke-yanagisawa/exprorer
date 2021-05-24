#! /bin/bash

#### init ####

. setting/initialize.sh

if [ $do_reorder"A" == "A" ] ; then
    do_reorder="Y"
fi

#### utility functions ####

. script/bash_functions.sh

#### calculation functions ####

preparation(){

    local i=$1
    local do_reorder=$2 # Y or otherwise (N)

    if [ `is_calculated $OUTPUTDIR $i ${map_prefix}_nVH.dx` == 1 ]; then
        logging_info "$TARGET_NAME $i : skipped because it has been already calculated"
        continue
    fi

    rm -r $OUTPUTDIR/prep$i # remove existing files
    mkdir -p $OUTPUTDIR/prep$i/input

    logging_debug "protein_param_file $protein_param_file"
    logging_debug "probe_param_file $probe_param_file"

    cp $INPUTDIR/$protein_param_file $OUTPUTDIR/prep$i/input/protein.conf
    cp $INPUTDIR/$probe_param_file   $OUTPUTDIR/prep$i/input/probe.conf
    cd $OUTPUTDIR/prep$i

    cosolvent_ID=`get_ini_variable $OUTPUTDIR/prep$i/input/probe.conf Cosolvent cid`
    $PYTHON $WORKDIR/script/cosolvent_box_generation.py \
	-prot_param $OUTPUTDIR/prep$i/input/protein.conf \
	-cosolv_param $OUTPUTDIR/prep$i/input/probe.conf \
	-tin   $WORKDIR/script/template_leap.in \
	-oprefix $OUTPUTDIR/prep$i/$TARGET_NAME \
        --packmol $PACKMOL \
	--tleap   $TLEAP \
	-no-rm-temp

    $PYTHON $WORKDIR/script/convert_amber_to_gromacs.py \
	   -iprefix $OUTPUTDIR/prep$i/${TARGET_NAME} \
	   -oprefix $OUTPUTDIR/prep$i/${TARGET_NAME}_GMX

    # add virtual interaction between cosolvents
    $PYTHON $WORKDIR/script/addvirtatom2top.py \
	-i $OUTPUTDIR/prep$i/${TARGET_NAME}_GMX.top \
	-o $OUTPUTDIR/prep$i/${TARGET_NAME}_tmp.top \
	-cname ${cosolvent_ID} \
	-ovis $OUTPUTDIR/prep$i/virtual_repulsion.top 
    $PYTHON $WORKDIR/script/addvirtatom2gro.py \
	-i $OUTPUTDIR/prep$i/${TARGET_NAME}_GMX.gro \
	-o $OUTPUTDIR/prep$i/$TARGET_NAME.gro \
	-cname ${cosolvent_ID} \
	-vname "VIS"

    # gen position restraint files
    $PYTHON $WORKDIR/script/add_posredefine2top.py \
	-v -res WAT Na+ Cl- CA MG ZN ${cosolvent_ID} \
	-target protein \
	-gro $OUTPUTDIR/prep$i/$TARGET_NAME.gro \
	-i $OUTPUTDIR/prep$i/${TARGET_NAME}_tmp.top \
	-o $OUTPUTDIR/prep$i/$TARGET_NAME.top \


    $GMX make_ndx -f $OUTPUTDIR/prep$i/$TARGET_NAME.gro << EOF
! "Water_and_ions"
q
EOF

    $GMX trjconv -s $OUTPUTDIR/prep$i/$TARGET_NAME.gro -f $OUTPUTDIR/prep$i/$TARGET_NAME.gro -o $OUTPUTDIR/prep$i/$TARGET_NAME.pdb <<EOF
0
EOF

    $PYTHON $WORKDIR/script/gromacs_preparation.py \
	DUMMY.conf \
	$WORKDIR/script/template_mdrun.sh \
	-v General:input_dir=$OUTPUTDIR/prep$i \
	-v General:output_dir=$OUTPUTDIR/system$i \
	-v General:name=$TARGET_NAME \
	-v ProductionRun:steps_pr=$steps_pr \
	-v ProductionRun:snapshot_interval=$snapshot_interval
}

start_mdrun(){
    local TARGET_NAME=$1
    local i=$2
    local ncpus=$3
    
    if [ `is_calculated $OUTPUTDIR $i ${map_prefix}_nVH.dx` == 1 ]; then
        logging_info "$TARGET_NAME $i : skipped because it has been already calculated"
        return 0
    fi

    cd $OUTPUTDIR/system$i
    GMX=$GMX bash mdrun.sh $ncpus
    echo $PYTHON $WORKDIR/script/gen_pmap.py \
	-p top/$TARGET_NAME.top \
	-y pr/$TARGET_NAME.xtc \
	-c top/$TARGET_NAME.gro \
	--start $begin_snapshot_for_pr \
	--cpptraj $CPPTRAJ \
        --outprefix $map_prefix \
	$OUTPUTDIR/prep$i/input/protein.conf \
	$OUTPUTDIR/prep$i/input/probe.conf
    $PYTHON $WORKDIR/script/gen_pmap.py \
	-p top/$TARGET_NAME.top \
	-y pr/$TARGET_NAME.xtc \
	-c top/$TARGET_NAME.gro \
	--start $begin_snapshot_for_pr \
	--cpptraj $CPPTRAJ \
        --outprefix $map_prefix \
	$OUTPUTDIR/prep$i/input/protein.conf \
	$OUTPUTDIR/prep$i/input/probe.conf
}


#### output environments ####

logging_info "which $PYTHON: `which $PYTHON`"

#### main routine ####

if [ $# != 1 ]; then
    logging_debug "$# != 1"
    logging_error "Invalid usage has been detected!"
    logging_info  "Usage: bash $0 CONFIG.sh"
    exit 1
fi

CONFIG_FILE=$1
source $CONFIG_FILE

#### preparation ####

WORKDIR=`pwd`
unset OMP_NUM_THREADS
NCPUS=${NCPUS:-`get_ncpus`}

gpus=( `get_gpu_list` )
echo $gpus
if [ ${#gpus[@]} == "0" ]; then
    logging_error "There is no available GPU according to the CUDA_VISIBLE_DEVICES variable."
    logging_error "This code assume at least 1 GPU. Please set the variable correctly."
    logging_error "Exit."
    exit 1
fi

NCPUS_PER_GPU=$(( $NCPUS / ${#gpus[@]} ))

iter_ed=$(( $iter + $iter_st ))

# parallel preparation
for i in `seq $iter_st $(( $iter_ed - 1 ))`
do
    preparation $i $do_reorder &
done
wait

echo parallel execution
# parallel execution
i=$iter_st
while [ $i -lt $iter_ed ]
do
    for j in ${gpus[@]}
    do
	echo $i $iter_st $iter_ed "(" ${gpus[@]} ")" $j $NCPUS_PER_GPU
	export CUDA_VISIBLE_DEVICES="$j"
	start_mdrun $TARGET_NAME $i $NCPUS_PER_GPU &
	
	i=$(( i + 1 ))
	if [ $i -ge $iter_ed ]; then
	    break
	fi
    done
    wait
done

