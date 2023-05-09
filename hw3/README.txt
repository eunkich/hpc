source hw3.cmpl
./main | tee default.csv
./main-o3 | tee o3.csv
./main-ffast-math | tee ffast-math.csv

source strassen.sh
./strassen_test | tee strassen.csv
