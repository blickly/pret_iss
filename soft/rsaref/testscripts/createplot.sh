if [ $# -ne 2 ]; then
    echo "Usage: createplot.sh <datain> <psout>";
    exit;
fi
datetime=`date +%Y%m%d%H%M%S`

echo "set term postscript" > plot_$datetime.gnuplot
echo "set output '$2'" >> plot_$datetime.gnuplot
echo "set yrange [100000:700000]" >> plot_$datetime.gnuplot
echo "plot '$1'" >> plot_$datetime.gnuplot

gnuplot plot_$datetime.gnuplot
rm plot_$datetime.gnuplot

