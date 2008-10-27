set DB;
set READDATA;
set WRITEDATA;

param dbrepcnt{d in DB};
param N_r{d in DB, r in READDATA union WRITEDATA};
param N_w{d in DB, w in READDATA union WRITEDATA};
param A_mm := 90;
param t_spm := 1;
param MAXSIZE;

var alloc_spm{d in DB, i in READDATA union WRITEDATA} binary;

maximize most_costly:
sum {d in DB, i in READDATA union WRITEDATA} alloc_spm[d,i] * A_mm *(N_r[d,i] + N_w[d,i]);

/*+ ((1-alloc_spm[d,i])*N_read[d,i]*t_spm)); */
subject to meet_numtoalloc: MAXSIZE >=sum{d in DB, i in READDATA union WRITEDATA } alloc_spm[d,i];
subject to unique{i in READDATA union WRITEDATA}: sum{d in DB} alloc_spm[d,i] <= 1;

