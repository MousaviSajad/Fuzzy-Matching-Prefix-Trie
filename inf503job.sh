#!/bin/bash
#SBATCH --job-name=inf503                        # the name of your job
#SBATCH --output=/scratch/sm3276/output.txt	# this is the file your output and errors go to
#SBATCH --time=26:00:00				# 26 hours, shorter time, quicker start, max run time 
#SBATCH --workdir=/scratch/sm3276		# your work directory
#SBATCH --mem=80000                              # 80GB of memory

# load a module, for example
module load gcc/6.2.0

# run your application, precede the application command with srun
# a couple example applications ...
gcc /scratch/sm3276/main.cpp -lstdc++ -o main
/scratch/sm3276/main
srun date



