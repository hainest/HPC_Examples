use strict;
use warnings;

die "Run make first!\n" unless(-f 'sort_c' && -f 'sort_cpp');

my @files_to_remove = glob("*.c.out *.cpp.out");

if (@files_to_remove) {
	print "Remove files? ";
	chomp(my $ans = <STDIN>);
	if($ans =~ /y[es]/i){
		unlink @files_to_remove;
	} else {
		exit;
	}
}

for my $n (1000000,10000000,50000000) {
	print "Running $n...\ni = ";
	for (1..10) {
		print "$_ ";
		`./sort_c numbers$n $n >> ${n}.c.out`;
		`./sort_cpp numbers$n $n >> ${n}.cpp.out`;
	}
	print "\n";
}