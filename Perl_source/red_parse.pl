use 5.010 ;
use strict ;
use warnings ;
use List::Util qw(max) ;

my $html_file = $ARGV[0] ;
my $file_TD = "../TD.txt" ;
my $file_amphi = "../Amphi.txt" ;

open(my $fread, '<:encoding(UTF-8)', $html_file) or die "Couldn't open file $html_file : $!" ;
open(my $fhTD, '>', $file_TD) or die "Couldn't open file $file_TD : $!" ;
open(my $fhAmphi, '>', $file_amphi) or die "Couldn't open file $file_amphi : $!" ;

my $row = "" ;

#Skip the header
next_line () ;

#Getting dept size
$row = <$fread> ;
my @lanieres ;
while($row !~ /<\/tr>/i) {
	$row =~ /<td.*? colspan=(\d+).*?>(?:<.*?>)*(.+?)</i ;
	my $n_gps = $1/2 ;
	my $laniere = $2 ;
	$laniere =~ s/Lani.*re/Laniere/ ;
	foreach my $i (1..($n_gps)) {
	push(@lanieres, $laniere)
	}
	$row = <$fread> ;
}
$row = <$fread> ;

# Getting group names
my @groupnames ;
$row = <$fread> ;
while ($row !~ /<\/tr>/i) {
	$row =~ />(\d+)</ ;
	push(@groupnames, $1) ;
	$row = <$fread> ;
}
$row = <$fread> ;
#Skip the semester line
next_line () ;

#Useful variables
my @rows_left = (0)x(2*$#groupnames+2) ;
my $cur_row = 0 ;
my $cur_col = 0 ;
my @days = qw(Lundi Mardi Mercredi Jeudi Vendredi) ;

#Main loop
while (($row = <$fread>)) { ;
	parse_line($row) ;	
}

#Closing file handles
close $fread ;
close $fhAmphi ;
close $fhTD ;

system "perl Perl_source/cleanup.pl" ;

sub next_line {
	my $local = "" ;
	while ($local !~ /<tr>/i) {
		$local = <$fread> ;
	}
}

sub parse_line {
	my $data = $_[0] ;	
	
	#Treatment for each new line
	if ($data =~ /<tr>/i) {
	#Decreasing rows_left
	foreach my $i (0..$#rows_left) {
		$rows_left[$i]-- ;
	}
	$cur_col = next_col(-1) ;
	$cur_row++ ;
	#say "@rows_left		$#rows_left		$cur_row" ;
	#<STDIN> ;
	}
	
	#Treatment for each data cell
	if ($data =~ /<td (?:style=".*?" )?(?:colspan=(\d))?\s?(?:rowspan=(\d))?.*?(?:bgcolor="(#[0-9a-f]{6})")?>(.*)<\/td>/i) {
		
		my $colspan = $1 ;
		my $rowspan = $2 ;
		my $color = $3 ;
		my $content = $4 ;
		
		#If not specified, cell dimensions are 1 and background is white
		if (!defined $colspan) {
		$colspan = 1 ;
		}
		if (!defined $rowspan) {
		$rowspan = 1 ;
		}
		if (!defined $color) {
		$color = "#FFFFFF"
		}
		
		#Saving row cell size
		foreach my $shift (0..($colspan-1)) {
			$rows_left[$cur_col+$shift] = $rowspan ;
		}

		#Writing maths schedule
		if ($color eq "#FF0000") {
			my $group ;
			
			if ($content =~ /amphi/i) {
				$group = $lanieres[int($cur_col/2)] ;
				my $sem = ($cur_col%2) ;
				say $fhAmphi "$group,$sem,$cur_row".','.($cur_row+$rowspan) ;
				if ($colspan > 1) {
				say $fhAmphi "$group,1,$cur_row".','.($cur_row+$rowspan) ;
				}
				
			} else {
				foreach my $col ($cur_col..($cur_col+$colspan-1)) {
					$group = $groupnames[int($col/2)] ;
					my $sem = ($col%2) ;
					say $fhTD "$group,$sem,$cur_row".','.($cur_row+$rowspan) ;
				}
			}
		}

		#Writing OMSI schedule
		if ($color eq "#0070C0" and $content !~ /INFO/i) {
			foreach my $col ($cur_col..($cur_col+$colspan-1)) {
					my $group = $groupnames[int($col/2)] ;
					my $sem = ($col%2)+1 ;
					say $fhTD "O$group,$sem,$cur_row".','.($cur_row+$rowspan) ;
				}
		}
		
		#Updating current column
		$cur_col = next_col($cur_col) ;
		
	}
	
	
}

sub next_col {
	my $c = $_[0] ;
	while(++$c <= $#rows_left and $rows_left[$c] != 0) {}
	return $c ;
}

sub hour_min {
	my $time = $_[0] ;
	my $hour = 8+int($time%21/2) ;
	my $min = ($time%21%2)?"30":"" ;
	
	return $hour.'h'.$min ;
}
