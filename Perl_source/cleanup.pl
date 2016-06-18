use 5.010 ;
use warnings ;
use strict ;

my $filename = "Amphi_sort.txt" ;
my $filewrite = "Amphi.txt" ;

system "sort ../Amphi.txt > Amphi_sort.txt" ;

open(my $fread, '<', $filename) or die "Couldn't open file $filename : $!" ;
open(my $fwrite, '>', $filewrite) or die "Couldn't open file $filewrite : $!" ;

my $cur_l = "" ;
my $prev_l = "" ;

while ($cur_l = <$fread>) {
	if ($cur_l ne $prev_l) {
		printf $fwrite $cur_l ;
	}
	$prev_l = $cur_l ;
}

close $fread ;
close $fwrite ;

system "rm -f $filename" ;
