reset
set xlabel 'test'
set ylabel 'time(microsec)'
set title 'perfomance comparison(add)'
set term png enhanced font 'Verdana,10'
set output 'addruntime.png'
set format x "%10.0f"
set xtic 100
set xtics rotate by 45 right

plot [:500][:]'cpy.txt'  title 'cpy_ add',\
'ref.txt'  title 'ref_ add'
