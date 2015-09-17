use strict;
use warnings;
use PDL;
use PDL::NiceSlice;
use PDL::Graphics2D;
use PGPLOT;
use List::Util;

my $cpp = zeroes(10,3);
my $c = zeroes(10,3);

my @n = (1000000,10000000,50000000);
my $n_pdl = log10(pdl(@n));

for my $i(0..2) {
	$cpp(:,$i) .= rcols("$n[$i].cpp.out",[1],{'COLSEP'=>','});
	$c(:,$i) .= rcols("$n[$i].c.out",[1],{'COLSEP'=>','});
}

my $speedup;
{
	my $med_c = medover($c);
	my $med_cpp = medover($cpp);
	my $midpoints = ($med_c + $med_cpp) / 2;
	$speedup = $med_c / $med_cpp;
}

$cpp .= log10($cpp) - 6;
$c .= log10($c) - 6;

my $win = PDL::Graphics2D->new('PGPLOT',{'device'=>'timings.ps/vcps', 'WindowWidth'=>6});

my ($xMin,$xMax) = minmax($n_pdl);
my @tmp = (minmax($c), minmax($cpp));
my $yMin = List::Util::min(@tmp);
my $yMax = List::Util::max(@tmp);

$win->env($xMin,$xMax,$yMin,$yMax,{
	'border' => {'type' => 'rel', 'value' => 0.08},
	'axis' => 'LOGXY',
});
my @colors = ('blue','orange');
my $colorIndex = 0;

$win->hold();
for my $x ($cpp, $c) {
	my (undef, undef, $med, $min, $max) = statsover($x);
	$win->errb($n_pdl, $med, undef, undef, $med-$min, $max-$med,{
		'color' => $colors[$colorIndex++],
		'symbol' => 'square',
		'linewidth' => 4,
		'term' => 2
	});
}
$win->release();

my $xPos = 0.8*($xMax-$xMin) + $xMin;
my $yPos = 0.4*($yMax-$yMin) + $yMin; 
$win->legend(['C++','C'],$xPos,$yPos,{
	'color'=>\@colors,
	'linewidth'=>[4,4],
	'textfraction' => 0.6
});

my $midpoints;
{
	my $med_c = medover($c);
	my $med_cpp = medover($cpp);
	$midpoints = ($med_c + $med_cpp) / 2;
}

my $justify = 0.0;
for my $i (0..2) {
	$justify = 1.0 if $i == 2;
	$win->text(sprintf('\gG = %5.3f',$speedup(($i))),
		$n_pdl(($i)),$midpoints(($i)),
		{'charsize'=>1.3,'justification'=>$justify});
}

pgsch(1.5);
pgmtxt('L',2.5,0.5,0.5,'time (s)');
pgmtxt('B',2.5,0.5,0.5,'N');