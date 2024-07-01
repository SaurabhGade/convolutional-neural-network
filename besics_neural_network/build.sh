#!/bin/sh
set -xe   ##Enable tracing , so it will show what is running

# gcc -Wall -Wextra singleNeuron.c  -o singleNeuron -lm -g #Wall -- show warning   -Wextra -- show all extra warnings.
# ./singleNeuron


# gcc -Wall -Wextra besic_gate_model.c  -o besic_gate_model -lm -g #Wall -- show warning   -Wextra -- show all extra warnings.
# ./besic_gate_model

gcc -Wall -Wextra universal_logic_gate_model.c -o universal_logic_gate_model -lm -g #Wall -- show warning   -Wextra -- show all extra warnings.
./universal_logic_gate_model



