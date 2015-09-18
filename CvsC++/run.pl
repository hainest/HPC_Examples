use strict;
use warnings;

die "Run make first!\n" unless(-f 'test_c' && -f 'test_cpp' && -f 'testD_cpp');

my @files_to_remove = glob("*.c.out *.cpp.out *.cppD.out");

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
		`./sortD_cpp numbers$n $n >> ${n}.cppD.out`;
	}
	print "\n";
}