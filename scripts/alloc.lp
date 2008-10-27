set DB;
set INST;

param dbrepcnt{d in DB};
param db_missed{d in DB, i in INST};
param t_mm := 90;
param t_spm := 1;
param ALLOCINST;

var alloc_spm{d in DB, i in INST} binary;

maximize most_costly:
sum {d in DB, i in INST} (alloc_spm[d,i] * db_missed[d,i] * t_mm);

subject to meet_numtoalloc: ALLOCINST >= sum{d in DB, i in INST} alloc_spm[d,i];
subject to unique{i in INST}: sum{d in DB} alloc_spm[d,i] <= 1; 
