#!/usr/bin/perl

use strict;

while( my $line = <>) {
    chomp $line;
    my $fix = $line;

    if( $line =~ /\(at [-+]?[0-9]*\.?[0-9]+ [-+]?[0-9]*\.?[0-9]+ [-+]?[0-9]*\.?[0-9]+\)/) {
	my( $x, $y, $a) = $line =~ /\(at ([-+]?[0-9]*\.?[0-9]+) ([-+]?[0-9]*\.?[0-9]+) ([-+]?[0-9]*\.?[0-9]+)\)/;
	#	print "x=$x y=$y a=$a\n";
	if( $a % 90) {
	    $fix = "  (at $x $y 0)";
	}
    }
    print "$fix\n";
}
