if [ $# -ne 2 ]; then
    echo "Usage: runtest.sh <# of data points> <plotout>";
    exit;
fi
CURDIR=`pwd`
RDEMODIR="$CURDIR/../src/rdemo"
KEYDIR="$CURDIR/../src/bin/keys"
PRETDIR="$CURDIR/.."
datetime=`date +%Y%m%d%H%M%S`

echo "Data" > data_$datetime.raw

#Create encrypt.c
for (( i = 0 ; i < $1 ; i++ ))
do
  cp $RDEMODIR/encryptbase.c $RDEMODIR/encrypt.c
  cat $KEYDIR/key0512-$i >> $RDEMODIR/encrypt.c 
  cd $PRETDIR
  make
  ./run.sh pret >> $CURDIR/data_$datetime.raw
  touch $CURDIR/progress$i
done

