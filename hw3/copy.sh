PERM=('kij' 'jik' 'ikj' 'kji')
 
for order in "${PERM[@]}"
do
  cp mm_jki.hpp mm_$order.hpp
  cp mm_jki.cpp mm_$order.cpp
done