grep "Cycle" csimoutput > cyclelines
awk -F": " '{print $2}' cyclelines > cycletimes
while read line
do
 read line2
 echo `expr $line2 - $line`;
done < cycletimes


